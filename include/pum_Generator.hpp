/**  
    @file pum_Generator.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    PUM is released under the MIT license.
*/

#pragma once

#include "pum_Types.hpp"

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
class Generator
{
public:
    Generator() = default;
    ~Generator() = default;

private:
    uint8_t channel = 0;     //<< [0, 15]
    uint8_t cableNumber = 0; //<< [0, 15]
};
} // namespace pum