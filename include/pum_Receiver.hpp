/**  
    @file pum_Receiver.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    PUM is released under the MIT license.
*/

#pragma once

#include "tusb.h"

#include <array>
#include <cstdint>

namespace pum
{
class Receiver
{
public:
    Receiver() = default;
    ~Receiver() = default;

private:
    void read()
    {
        while (tud_midi_available())
        {
            const bool is4Byte = tud_midi_packet_read (packet.data());
            if (is4Byte)
            {
            }
            else
            {
            }
        }
    }

private:
    std::array<uint8_t, 4> packet {};
};
} // namespace pum