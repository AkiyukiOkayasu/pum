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

    uint8_t channel = 0;     //<< [0, 15]
    uint8_t cableNumber = 0; //<< [0, 15]
};
} // namespace pum