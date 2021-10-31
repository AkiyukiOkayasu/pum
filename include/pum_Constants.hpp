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

///https://ja.wikipedia.org/wiki/MIDI#%E3%83%81%E3%83%A3%E3%83%B3%E3%83%8D%E3%83%AB%E3%83%A1%E3%83%83%E3%82%BB%E3%83%BC%E3%82%B8
namespace ChannelMessage
{
    ///https://ja.wikipedia.org/wiki/MIDI#%E3%83%81%E3%83%A3%E3%83%B3%E3%83%8D%E3%83%AB%E3%83%9C%E3%82%A4%E3%82%B9%E3%83%A1%E3%83%83%E3%82%BB%E3%83%BC%E3%82%B8
    namespace ChannelVoiceMessage
    {
        inline constexpr uint8_t NOTE_ON = 0x90;
        inline constexpr uint8_t NOTE_OFF = 0x80;
        inline constexpr uint8_t POLYPHONIC_KEY_PRESSURE = 0xA0;
        inline constexpr uint8_t CONTROL_CHANGE = 0xB0;
        inline constexpr uint8_t PROGRAM_CHANGE = 0xC0;
        inline constexpr uint8_t CHANNEL_PRESSURE = 0xD0;
        inline constexpr uint8_t PITCH_BEND = 0xE0;
    } // namespace ChannelVoiceMessage

    ///https://ja.wikipedia.org/wiki/MIDI#%E3%83%81%E3%83%A3%E3%83%B3%E3%83%8D%E3%83%AB%E3%83%A2%E3%83%BC%E3%83%89%E3%83%A1%E3%83%83%E3%82%BB%E3%83%BC%E3%82%B8
    namespace ChannelModeMessage
    {
        inline constexpr uint8_t ALL_SOUND_OFF = 0x78;
        inline constexpr uint8_t RESET_ALL_CONTROLLER = 0x79;
        inline constexpr uint8_t LOCAL_CONTROL = 0x7A;
        inline constexpr uint8_t ALL_NOTE_OFF = 0x7B;

        /** 7CH、7DH、7EH、7FHの4つのチャンネルモードメッセージを使いオムニモード、発音数のオンオフを組み合わせることで、4種のMIDIモードを設定できる。
        オムニモード - 7CH オムニオン、7DH オムニオフで設定。MIDIチャンネルを区別するかしないか。オフの場合、チャンネルに関係なく全ての情報を受信し処理、発音する。
        発音数 - 7EH モノモードオン、7FH ポリモードオンで設定。どちらかを設定すると片方のモードは自動的にオフになる。単音しか出せないのか、和音が出せるのかを設定する。 
        
        MODE1 = OMNI_ON + POLY
            MIDIチャンネルを意識せず和音演奏ができるモード。
        MODE2 = OMNI_ON + MONO
            MIDIチャンネルに関わらず、常に1音のみ鳴らすモード。
        MODE3 = OMNI_OFF + POLY
            一般的な送受信モード。MIDIチャンネルを区別し、各チャンネル毎に和音を用いた演奏が可能なモード。
        MODE4 = OMNI_OFF + MONO
            チャンネルは区別するが、各チャンネル毎に1音しか出せないモード。たとえば6弦あるギターシンセサイザーの各弦を各チャンネルに割り当てる場合に使用する。この場合、単音で発声するチャンネルは6つとなるので、第2データバイトでは06Hを送信する。
        */
        namespace MidiModeConfig
        {
            inline constexpr uint8_t OMNI_ON = 0x7D;
            inline constexpr uint8_t OMNI_OFF = 0x7C;
            inline constexpr uint8_t POLY = 0x7F;
            inline constexpr uint8_t MONO = 0x7E;
        } // namespace MidiModeConfig
    }     // namespace ChannelModeMessage

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

/** SystemCommon message 
    @TODO implement
    https://ja.wikipedia.org/wiki/MIDI#%E3%82%B7%E3%82%B9%E3%83%86%E3%83%A0%E3%82%B3%E3%83%A2%E3%83%B3%E3%83%A1%E3%83%83%E3%82%BB%E3%83%BC%E3%82%B8
*/
namespace SystemCommonMessage
{

}
} // namespace pum
