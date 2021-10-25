/**  
    @file pum_Generator.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    PUM is released under the MIT license.
*/

#pragma once

#include "pum_Types.hpp"

#include <cassert>
#include <cstdint>

//==============================================================================
//Function
namespace pum
{
/** Make USB-MIDI note on packet.
    @param noteNumber MIDI note number [0, 127]
    @param velocity MIDI note velocity [0, 127]
    @param channel MIDI channel [0, 15]
    @return midiEventPacket_t 
*/
inline UsbMidiEventPacket makeNoteOn (uint8_t noteNumber, uint8_t velocity, uint8_t channel)
{
    UsbMidiEventPacket p;
    p.header = CodeIndexNumber::NOTE_ON;
    p.byte1 = ChannelMessage::NOTE_ON | channel;
    p.byte2 = noteNumber;
    p.byte3 = velocity;
    return p;
}

/** Make USB-MIDI note off packet.
    @param noteNumber MIDI note number [0, 127]
    @param channel MIDI channel [0, 15]
    @return midiEventPacket_t 
*/
inline UsbMidiEventPacket makeNoteOff (uint8_t noteNumber, uint8_t channel)
{
    UsbMidiEventPacket p;
    p.header = CodeIndexNumber::NOTE_OFF;
    p.byte1 = ChannelMessage::NOTE_OFF | channel;
    p.byte2 = noteNumber;
    p.byte3 = 0; ///< velocity 0
    return p;
}
} // namespace pum

//==============================================================================
//Class
namespace pum
{
/// USB-MIDI packet generator.
class Generator
{
public:
    Generator (const uint8_t channel, const uint8_t cableNumber)
        : channel (channel), cableNumber (cableNumber)
    {
    }

    ~Generator() = default;

    /** Set MIDI channel.    
        @param newMIDIChannel [0, 15]
    */
    void setChannel (const uint8_t newMIDIChannel)
    {
        channel = newMIDIChannel;
    }

    /** MIDI note ON.
    
        @param packet 
        @param noteNumber [0, 127]
        @param velocity [0, 127]
    */
    void makeNoteOn (uint8_t packet[4], const uint8_t noteNumber, const uint8_t velocity)
    {
        assert (noteNumber < 128);
        assert (velocity < 128);
        packet[0] = makeHeader (CodeIndexNumber::NOTE_ON);
        packet[1] = ChannelMessage::NOTE_ON | channel;
        packet[2] = noteNumber;
        packet[3] = velocity;
    }

    /** MIDI note OFF.
    
        @param packet 
        @param noteNumber [0, 127]
    */
    void makeNoteOff (uint8_t packet[4], const uint8_t noteNumber)
    {
        assert (noteNumber < 128);
        packet[0] = makeHeader (CodeIndexNumber::NOTE_OFF);
        packet[1] = ChannelMessage::NOTE_OFF | channel;
        packet[2] = noteNumber;
        packet[3] = 0; ///< velocity 0
    }

private:
    uint8_t makeHeader (const uint8_t cin)
    {
        assert (cableNumber < 16);
        assert (cin < 16);
        return (cableNumber << 4) | (cin & 0xF);
    }

    uint8_t channel = 0;     //<< [0, 15]
    uint8_t cableNumber = 0; //<< [0, 15]
};
} // namespace pum