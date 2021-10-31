/**  
    @file pum_Constants.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    PUM is released under the MIT license.
*/

#pragma once

#include <cstdint>

namespace pum
{
/** USB-MIDI CIN (Code Index Number).
    https://pcm1723.hateblo.jp/entry/20150109/1420819489
*/
namespace CodeIndexNumber
{
    inline constexpr uint8_t SYSCOMMON_2BYTES = 0x02; ///< データ長2Byte
    inline constexpr uint8_t SYSCOMMON_3BYTES = 0x03;
    inline constexpr uint8_t SYSEX_START_OR_CONTINUE = 0x04;
    inline constexpr uint8_t SYSEX_END_1BYTE = 0x05;  ///< SysEx最終パケット データ長1Byte
    inline constexpr uint8_t SYSEX_END_2BYTES = 0x06; ///< SysEx最終パケット データ長2Byte
    inline constexpr uint8_t SYSEX_END_3BYTES = 0x07; ///< SysEx最終パケット データ長3Byte
    inline constexpr uint8_t NOTE_OFF = 0x08;
    inline constexpr uint8_t NOTE_ON = 0x09;
    inline constexpr uint8_t POLYPHONIC_KEY_PRESSURE = 0x0A;
    inline constexpr uint8_t CONTROL_CHANGE = 0x0B;
    inline constexpr uint8_t PROGRAM_CHANGE = 0x0C;   ///< データ長2Byte
    inline constexpr uint8_t CHANNEL_PRESSURE = 0x0D; ///< データ長2Byte
    inline constexpr uint8_t PITCH_BEND = 0x0E;
    inline constexpr uint8_t SYS_REALTIME = 0x0F; ///< データ長1Byte
} // namespace CodeIndexNumber

/** MIDI SysEx.
    0xF0, 0x7D, 0x7F, ~~~~~ , 0xF7
    https://ja.wikipedia.org/wiki/MIDI#%E3%82%B7%E3%82%B9%E3%83%86%E3%83%A0%E3%82%A8%E3%82%AF%E3%82%B9%E3%82%AF%E3%83%AB%E3%83%BC%E3%82%B7%E3%83%96%E3%83%A1%E3%83%83%E3%82%BB%E3%83%BC%E3%82%B8
*/
namespace SysEx
{
    inline constexpr uint8_t BEGIN = 0xF0; ///< SysEx開始
    inline constexpr uint8_t END = 0xF7;   ///< SysEx終了

    namespace ManufacturerID
    {
        inline constexpr uint8_t NON_COMMERCIAL = 0x7D; ///< Universal SysEx Nonprofit
        inline constexpr uint8_t NON_REALTIME = 0x7E;   ///< Universal SysEx Non realtime
        inline constexpr uint8_t REALTIME = 0x7F;       ///< Universal SysEx Realtime
    }                                                   // namespace ManufacturerID

    namespace DeviceID
    {
        inline constexpr uint8_t ALL = 0x7F;  ///< 全端末
        inline constexpr uint8_t JADE = 0x76; ///< jade
    }                                         // namespace DeviceID
} // namespace SysEx

namespace ChannelMessage
{
    inline constexpr uint8_t NOTE_ON = 0x90;
    inline constexpr uint8_t NOTE_OFF = 0x80;
} // namespace ChannelMessage

///https://ja.wikipedia.org/wiki/MIDI#%E3%82%B7%E3%82%B9%E3%83%86%E3%83%A0%E3%83%AA%E3%82%A2%E3%83%AB%E3%82%BF%E3%82%A4%E3%83%A0%E3%83%A1%E3%83%83%E3%82%BB%E3%83%BC%E3%82%B8
namespace SystemRealtime
{
    inline constexpr uint8_t TIMING_CLOCK = 0xF8;
    inline constexpr uint8_t START = 0xFA;
    inline constexpr uint8_t CONTINUE = 0xFB;
    inline constexpr uint8_t STOP = 0xFC;
    inline constexpr uint8_t ACTIVE_SENSING = 0xFE;
    inline constexpr uint8_t SYSTEM_RESET = 0xFF;
} // namespace SystemRealtime
} // namespace pum
