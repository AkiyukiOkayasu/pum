/**  
    @file pum_Parser.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    PUM is released under the MIT license.
*/

#pragma once

#include "pum_Constants.hpp"
#include "pum_Types.hpp"

#include <array>
#include <cstdint>
#include <functional>

namespace pum
{
/// USB-MIDI event packet parser.
class Parser
{
public:
    Parser() = default;
    ~Parser() = default;

    void parse (const uint8_t packet[4])
    {
        const uint8_t codeIndex = extractCodeIndexNumber (packet[0]);
        const uint8_t cn = extractCableNumber (packet[0]);
        const uint8_t statusByte = packet[1];
        const uint8_t channel = extractMidiChannel (statusByte);

        switch (codeIndex)
        {
            case CodeIndexNumber::SYSCOMMON_2BYTES:
                break;
            case CodeIndexNumber::SYSCOMMON_3BYTES:
                break;
            case CodeIndexNumber::SYSEX_START_OR_CONTINUE:
                if (packet[1] == SysEx::BEGIN)
                {
                    //SysEx start
                    receivingSysEx = true;
                    sysExIndex = 0;
                    pushSysEx (packet[2]);
                    pushSysEx (packet[3]);
                }
                else
                {
                    //SysEx continue
                    pushSysEx (packet[1]);
                    pushSysEx (packet[2]);
                    pushSysEx (packet[3]);
                }
                break;
            case CodeIndexNumber::SYSEX_END_1BYTE:
                assert (packet[1] == SysEx::END);
                receivingSysEx = false;
                if (onSysEx)
                {
                    onSysEx (sysExData.data(), sysExIndex);
                }
                break;
            case CodeIndexNumber::SYSEX_END_2BYTES:
                assert (packet[2] == SysEx::END);
                pushSysEx (packet[1]);
                receivingSysEx = false;
                if (onSysEx)
                {
                    onSysEx (sysExData.data(), sysExIndex);
                }
                break;
            case CodeIndexNumber::SYSEX_END_3BYTES:
                assert (packet[3] == SysEx::END);
                pushSysEx (packet[1]);
                pushSysEx (packet[2]);
                receivingSysEx = false;
                if (onSysEx)
                {
                    onSysEx (sysExData.data(), sysExIndex);
                }

                break;
            case CodeIndexNumber::NOTE_OFF:
                if (onNoteOff)
                {
                    Note note = { packet[2], packet[3], channel };
                    onNoteOff (note);
                }
                break;
            case CodeIndexNumber::NOTE_ON:
            {
                const uint8_t noteNumber = packet[2];
                const uint8_t velocity = packet[3];
                const Note note = { noteNumber, velocity, channel };
                if (velocity == 0) // velocity 0 = noteOFF
                {
                    if (onNoteOff)
                    {
                        onNoteOff (note);
                    }
                    break;
                }

                if (onNoteOn)
                {
                    onNoteOn (note);
                }
                break;
            }
            case CodeIndexNumber::POLYPHONIC_KEY_PRESSURE:
                if (onPolyphonicKeyPressure)
                {
                    const PolyphonicKeyPressure pkp = { packet[2], packet[3], channel };
                    onPolyphonicKeyPressure (pkp);
                }
                break;
            case CodeIndexNumber::CONTROL_CHANGE:
                if (onControlChange)
                {
                    const ControlChange cc = { packet[2], packet[3], channel };
                    onControlChange (cc);
                }
                break;
            case CodeIndexNumber::PROGRAM_CHANGE:
                if (onProgramChange)
                {
                    const ProgramChange pgc = { packet[2], channel };
                    onProgramChange (pgc);
                }
                break;
            case CodeIndexNumber::CHANNEL_PRESSURE:
                if (onChannelPressure)
                {
                    const ChannelPressure cp = { packet[2], channel };
                    onChannelPressure (cp);
                }
                break;
            case CodeIndexNumber::PITCH_BEND:
                if (onPitchBend)
                {
                    const PitchBend pb = { compute14bitValue (packet[2], packet[3]), channel };
                    onPitchBend (pb);
                }
                break;
            case CodeIndexNumber::SYS_REALTIME:
                if (onSystemRealtime)
                {
                    onSystemRealtime (statusByte);
                }
                break;
            default:
                break;
        }
    }

    /** Note on callback.
        You can assign a lambda to this callback object to have it called when the
     MIDI note on is come.      
    */
    std::function<void (Note)> onNoteOn;

    /** Note off callback.
        You can assign a lambda to this callback object to have it called when the
     MIDI note off is come.      
    */
    std::function<void (Note)> onNoteOff;

    /** Control change callback.
      You can assign a lambda to this callback object to have it called when the
     MIDI control change is come.      
    */
    std::function<void (ControlChange)> onControlChange;

    /** SysEx callback.
      You can assign a lambda to this callback object to have it called when the
     MIDI SysEx is come.
      @param SysExByteArray
      @param SysExByteArraySize
    */
    std::function<void (const uint8_t[], const uint8_t)> onSysEx;

    /** SystemRealtime callback.
        @param byte SystemRealtime is 1byte message.      
    */
    std::function<void (uint8_t byte)> onSystemRealtime;

    /** Program change callback.
        You can assign a lambda to this callback object to have it called when the
     MIDI Program change is come.
    */
    std::function<void (ProgramChange)> onProgramChange;

    /** Channel pressure callback.
        You can assign a lambda to this callback object to have it called when the
     MIDI Channel pressure is come.
    */
    std::function<void (ChannelPressure)> onChannelPressure;

    /** Polyphonic key pressure callback.
        You can assign a lambda to this callback object to have it called when the
     MIDI Polyphonic key pressure is come.
    */
    std::function<void (PolyphonicKeyPressure)> onPolyphonicKeyPressure;

    /** Polyphonic key pressure callback.
        You can assign a lambda to this callback object to have it called when the
     MIDI Pitch bend is come.
    */
    std::function<void (PitchBend)> onPitchBend;

private:
    /** Extract MIDI channel from MIDI status byte.
        @param statusByte MIDI status byte
        @return constexpr uint8_t MIDI channel [0, 15]
    */
    constexpr uint8_t extractMidiChannel (const uint8_t statusByte) const
    {
        // MIDI StatusByte下位4bit: MIDI channel
        return statusByte & 0x0F;
    }

    /** Extract Cable Number from Header (Byte0) of USB-MIDI event packet.
        https://pcm1723.hateblo.jp/entry/20150109/1420819489
        
        @param header Byte0 of USB-MIDI event packet
        @return uint8_t USB-MIDI Cable Number [0, 15]
        @note USB-MIDIデバイスが複数使用されているときのインデックスとして利用する
    */
    constexpr uint8_t extractCableNumber (const uint8_t header) const
    {
        // Header上位4bit: CableNumber
        return (header >> 4) & 0x0F;
    }

    /** Extract Code Index Number from Header (Byte0) of USB-MIDI event packet.
        https://pcm1723.hateblo.jp/entry/20150109/1420819489
        @param header  Byte0 of USB-MIDI event packet
        @return uint8_t USB-MIDI Code Index Number []
    */
    constexpr uint8_t extractCodeIndexNumber (const uint8_t header) const
    {
        // Header下位4bit: CodeIndexNumber
        return header & 0x0F;
    }

    /** Add the incoming SysEx to the array.
        @param dataToAdd Incoming SysEx data byte
    */
    void pushSysEx (const uint8_t dataToAdd)
    {
        sysExData[sysExIndex] = dataToAdd;
        sysExIndex++;
    }

    /** Bitwise compute two MIDI bytes and return a 14-bit signed number.
        https://www.g200kg.com/jp/docs/dic/pitchbend.html        
        @param lsb 7bit [0, 127]
        @param msb 7bit [0, 127]        
        @return 14bit signed int [-8192, 8191]
    */
    constexpr int16_t compute14bitValue (const uint8_t lsb, const uint8_t msb) const
    {
        uint16_t v = (static_cast<uint16_t> (msb) << 9) | (static_cast<uint16_t> (lsb) << 2);
        return (static_cast<int16_t> (v) >> 2);
    }

    static constexpr uint8_t MAX_SYSEX_LENGTH = 32;
    bool receivingSysEx = false;
    uint8_t sysExIndex = 0;
    std::array<uint8_t, MAX_SYSEX_LENGTH> sysExData;
};
} // namespace pum