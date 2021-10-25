/**  
    @file pum_Parser.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    PUM is released under the MIT license.
*/

#pragma once

#include <cstdint>

namespace pum
{
class Parser
{
public:
    Parser() = default;
    ~Parser() = default;

    /** Parse MIDI packet    
        @param packet 
    */
    void parse (const uint8_t (&packet)[4])
    {
    }

private:
};
} // namespace pum