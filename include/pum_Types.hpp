/**  
    @file pum_Types.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    PUM is released under the MIT license.
*/

#pragma once

#include <cstdint>

namespace pum
{
/// MIDI note
struct Note
{
    uint8_t noteNumber; ///< [0, 127]
    uint8_t velocity;   ///< [0, 127]
    uint8_t channel;    ///< [0, 15]
};

/// MIDI Control change
struct ControlChange
{
    uint8_t controlNumber; ///< [0, 119]
    uint8_t value;         ///< [0, 127]
    uint8_t channel;       ///< [0, 15]
};

/// MIDI Program change
struct ProgramChange
{
    uint8_t programNumber; ///< [0, 127]
    uint8_t channel;       ///< [0, 15]
};

/// MIDI Channel pressure
struct ChannelPressure
{
    uint8_t pressure; ///< [0, 127]
    uint8_t channel;  ///< [0, 15]
};

/// MIDI Polyphonic key pressure
struct PolyphonicKeyPressure
{
    uint8_t noteNumber; ///< [0, 127]
    uint8_t pressure;   ///< [0, 127]
    uint8_t channel;    ///< [0, 15]
};

/// MIDI Pitch bend
struct PitchBend
{
    int16_t bend;    ///< [-8192, 8191]
    uint8_t channel; ///< [0, 15]
};

} // namespace pum