/**  
    @file pum_Generator.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    PUM is released under the MIT license.
*/

#pragma once

#include "pum_Constants.hpp"

#include <cassert>
#include <cstdint>

namespace pum
{
/// USB-MIDI packet generator.
class Generator
{
public:
    Generator (const uint8_t channel, const uint8_t cableNumber)
        : channel (channel), cableNumber (cableNumber)
    {
        assert (cableNumber < 16);
    }

    ~Generator() = default;

    /** Set MIDI channel.    
        @param newMIDIChannel [0, 15]
    */
    void setChannel (const uint8_t newMidiChannel)
    {
        assert (newMidiChannel < 16);
        channel = newMidiChannel;
    }

    /** Generate USB-MIDI event packets with Note on.
        @param packet Array for storing USB-MIDI event packets.
        @param noteNumber [0, 127]
        @param velocity [0, 127]
    */
    void makeNoteOn (uint8_t packet[4], const uint8_t noteNumber, const uint8_t velocity)
    {
        assert (noteNumber < 128);
        assert (velocity < 128);
        packet[0] = makeHeader (CodeIndexNumber::NOTE_ON);
        packet[1] = ChannelMessage::ChannelVoiceMessage::NOTE_ON | channel;
        packet[2] = noteNumber;
        packet[3] = velocity;
    }

    /** Generate USB-MIDI event packets with Note off.
        @param packet Array for storing USB-MIDI event packets.
        @param noteNumber [0, 127]
    */
    void makeNoteOff (uint8_t packet[4], const uint8_t noteNumber)
    {
        assert (noteNumber < 128);
        packet[0] = makeHeader (CodeIndexNumber::NOTE_OFF);
        packet[1] = ChannelMessage::ChannelVoiceMessage::NOTE_OFF | channel;
        packet[2] = noteNumber;
        packet[3] = 0; ///< velocity 0
    }


    /** Generate USB-MIDI event packets with Program Change.
        @param packet Array for storing USB-MIDI event packets.
        @param programNumber [0, 127]
    */
    void makeProgramChange (uint8_t packet[4], const uint8_t programNumber)
    {
        assert (programNumber < 128);
        packet[0] = makeHeader (CodeIndexNumber::PROGRAM_CHANGE);
        packet[1] = ChannelMessage::ChannelVoiceMessage::PROGRAM_CHANGE | channel;
        packet[2] = programNumber;
    }

    /** Generate USB-MIDI event packets with Channel Pressure.
        @param packet Array for storing USB-MIDI event packets.
        @param pressure [0, 127]
    */
    void makeChannelPressure (uint8_t packet[4], const uint8_t pressure)
    {
        assert (pressure < 128);
        packet[0] = makeHeader (CodeIndexNumber::CHANNEL_PRESSURE);
        packet[1] = ChannelMessage::ChannelVoiceMessage::CHANNEL_PRESSURE | channel;
        packet[2] = pressure;
    }

    /** Generate USB-MIDI event packets with Pitch Bend.    
        @param packet Array for storing USB-MIDI event packets.
        @param bend [-8192, 8191]
    */
    void makePitchBend (uint8_t packet[4], const int16_t bend)
    {
        assert (bend <= 8191);
        assert (bend >= -8192);
        const uint8_t lsb = getLsb7bit (bend);
        const uint8_t msb = getMsb7bit (bend);
        packet[0] = makeHeader (CodeIndexNumber::PITCH_BEND);
        packet[1] = ChannelMessage::ChannelVoiceMessage::PITCH_BEND | channel;
        packet[2] = lsb;
        packet[3] = msb;
    }

private:
    /** Make Header (Byte0) of USB-MIDI event packet from Code index number and Cable number.
        @param cin CodeIndexNumber [0, 15]
        @return Header (Byte0) of USB-MIDI event packet
    */
    constexpr uint8_t makeHeader (const uint8_t cin) const
    {
        assert (cin < 16);
        return (cableNumber << 4) | (cin & 0xF);
    }

    /// Get LSB 7bit.
    constexpr uint8_t getLsb7bit (const int16_t v)
    {
        return static_cast<uint8_t> (0b00000000'01111111 & v);
    }

    /// Get MSB 7bit from 14bit value.
    constexpr uint8_t getMsb7bit (const int16_t v)
    {
        return static_cast<uint8_t> (0b00000000'01111111 & (v >> 7));
    }

    uint8_t channel = 0;     //<< [0, 15]
    uint8_t cableNumber = 0; //<< [0, 15]
};
} // namespace pum