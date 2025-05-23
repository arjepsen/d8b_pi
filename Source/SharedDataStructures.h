/*
  ==============================================================================

    SharedDataStructures.h
    Created: 25 Apr 2024 7:00:10pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include "Debug.h"
#include <cstring>
#include <stdio.h>

static constexpr int CHANNEL_COUNT = 96;
constexpr int CHANNEL_STRIP_COUNT = 24;   // Number of channel strips, excluding the master strip.
constexpr int BRAIN_FADER_CMD_LENGTH = 5; // Max brain fader command length. i.e. "22ABf" excluding the null terminator
const int MASTER_CH_STRIP = 24;

enum EventSource
{
    CONSOLE_EVENT,
    UI_EVENT
};

enum Bank
{
    LINE_BANK,
    TAPE_BANK,
    EFFECTS_BANK,
    MASTERS_BANK,
    NUMBER_OF_BANKS
};

// Enumeration of the channelstrips
enum ChStripID
{
    CH_STRIP1,
    CH_STRIP2,
    CH_STRIP3,
    CH_STRIP4,
    CH_STRIP5,
    CH_STRIP6,
    CH_STRIP7,
    CH_STRIP8,
    CH_STRIP9,
    CH_STRIP10,
    CH_STRIP11,
    CH_STRIP12,
    CH_STRIP13,
    CH_STRIP14,
    CH_STRIP15,
    CH_STRIP16,
    CH_STRIP17,
    CH_STRIP18,
    CH_STRIP19,
    CH_STRIP20,
    CH_STRIP21,
    CH_STRIP22,
    CH_STRIP23,
    CH_STRIP24,
    CH_STRIP_MASTER
};

// ================ Vpots =======================================
enum VpotFunction
{
    VPOT_PAN,
    VPOT_AUXSEND1,
    VPOT_AUXSEND2,
    VPOT_AUXSEND3,
    VPOT_AUXSEND4,
    VPOT_AUXSEND5,
    VPOT_AUXSEND6,
    VPOT_AUXSEND7,
    VPOT_AUXSEND8,
    VPOT_AUXSEND9_10,
    VPOT_AUXPAN9_10,
    VPOT_AUXSEND11_12,
    VPOT_AUXPAN11_12,
    VPOT_LVL2TAPE,
    VPOT_DIGITAL_TRIM,
    NUMBER_OF_VPOT_FUNCTIONS
};

// // ── Master-section v-pot raw IDs ───────────────────────────
// enum MasterVpotID : uint8_t
// {
//     MVPOT_LOW = 0x19,
//     MVPOT_LOW_MID = 0x1A,
//     MVPOT_HI_MID = 0x1B,
//     MVPOT_HI = 0x1C,
//     MVPOT_SOLO_STUDIO_LVL = 0x1D,
//     MVPOT_PHONES_1_LVL = 0x1E,
//     MVPOT_PHONES_2_LVL = 0x1F,
//     MVPOT_SPK_LEVEL = 0x20,
//     NUMBER_OF_MASTER_VPOTS
// };

// // ── High-level functions those v-pots *may* perform ─────────
// enum MasterVpotFunc : uint8_t
// {
//     MVPOTFUNC_EQ,        // all four EQ bands
//     MVPOTFUNC_COMP_GATE, // same four pots, different scaling
//     MVPOTFUNC_CR_LEVELS, // studio, phones A, phones B, speakers
//     MVPOTFUNC_OTHER,     // future
//     NUMBER_OF_MVPOTFUNCS
// };

using VpotHandler = void (*)(int /*delta-64…+63*/);

//extern const VpotHandler masterVpotDispatch[NUMBER_OF_MVPOTFUNCS][NUMBER_OF_MASTER_VPOTS];

// =========================== BUTTONS ========================================

enum ChStripButtonBase
{
    STRIP1_BTN_BASE = 0x051,
    STRIP2_BTN_BASE = 0x059,
    STRIP3_BTN_BASE = 0x041,
    STRIP4_BTN_BASE = 0x049,
    STRIP5_BTN_BASE = 0x031,
    STRIP6_BTN_BASE = 0x039,
    STRIP7_BTN_BASE = 0x021,
    STRIP8_BTN_BASE = 0x029,
    STRIP9_BTN_BASE = 0x011,
    STRIP10_BTN_BASE = 0x019,
    STRIP11_BTN_BASE = 0x001,
    STRIP12_BTN_BASE = 0x009,
    STRIP13_BTN_BASE = 0x179,
    STRIP14_BTN_BASE = 0x161,
    STRIP15_BTN_BASE = 0x169,
    STRIP16_BTN_BASE = 0x151,
    STRIP17_BTN_BASE = 0x159,
    STRIP18_BTN_BASE = 0x141,
    STRIP19_BTN_BASE = 0x149,
    STRIP20_BTN_BASE = 0x131,
    STRIP21_BTN_BASE = 0x139,
    STRIP22_BTN_BASE = 0x121,
    STRIP23_BTN_BASE = 0x129,
    STRIP24_BTN_BASE = 0x111
};

// ChannelStrip buttons
enum ButtonType
{
    MUTE_BTN,
    SOLO_BTN,
    SELECT_BTN,
    WRITE_BTN,
    ASSIGN_BTN,
    REC_RDY_BTN,
    NUMBER_OF_CHSTRIP_BUTTONS
};

enum ButtonAction : char
{
    BTN_PRESS = 's',
    BTN_RELEASE = 'u'
};

enum Aux
{
    AUX1,
    AUX2,
    AUX3,
    AUX4,
    AUX5,
    AUX6,
    AUX7,
    AUX8
};

class HexToIntLookup
{
  private:
    unsigned char hex_values[256];

    // Private constructor to prevent instantiation
    HexToIntLookup()
    {
        DEBUG_MSG("HEXTOINT CONSTRUCTOR\n");

        // Initialize hex_values with all zeros
        memset(hex_values, 0, sizeof(hex_values));

        // Set specific hex character values
        hex_values['0'] = 0;
        hex_values['1'] = 1;
        hex_values['2'] = 2;
        hex_values['3'] = 3;
        hex_values['4'] = 4;
        hex_values['5'] = 5;
        hex_values['6'] = 6;
        hex_values['7'] = 7;
        hex_values['8'] = 8;
        hex_values['9'] = 9;
        hex_values['A'] = 10;
        hex_values['B'] = 11;
        hex_values['C'] = 12;
        hex_values['D'] = 13;
        hex_values['E'] = 14;
        hex_values['F'] = 15;
        // Initialize additional characters if needed
    }

    // Private Copy constructor and assignment operator to prevent copying
    HexToIntLookup(const HexToIntLookup &);
    HexToIntLookup &operator=(const HexToIntLookup &);

  public:
    static HexToIntLookup &getInstance()
    {
        static HexToIntLookup instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    inline int hexToInt(const char (&hexString)[2])
    {
        return (hex_values[(unsigned char)hexString[0]] << 4) |
               hex_values[(unsigned char)hexString[1]];
    }

    inline int hex3ToInt(const char (&hexString)[3])
    {
        return (hex_values[(unsigned char)hexString[0]] << 8) |
               (hex_values[(unsigned char)hexString[1]] << 4) |
               hex_values[(unsigned char)hexString[2]];
    }
};

/**************************************************************************
 * @brief This is a simple singleton class that sets up an array for quick
 *      conversion between an integer and it's equivalent 2-char hex value.
 *      For simplicity's sake there's no error checking, so it's up to
 *      you to keep the provided integer between 0 and 255.
 **************************************************************************/
class IntToHexLookup
{
  private:
    char hexLookupArray[256][3]; // Hex strings + null terminator

    IntToHexLookup()
    {
        DEBUG_MSG("INTTOHEX CONSTRUCTOR\n");

        for (int i = 0; i < 256; ++i)
        {
            sprintf(hexLookupArray[i], "%02X", i);
        }
    }

  public:
    IntToHexLookup(const IntToHexLookup &) = delete;            // Prevent copying
    IntToHexLookup &operator=(const IntToHexLookup &) = delete; // Prevent assignment

    static IntToHexLookup &getInstance()
    {
        static IntToHexLookup instance; // Singleton instance
        return instance;
    }

    inline const char *getHexValue(int index) const
    {
        return hexLookupArray[index];
    }
};

// struct ChannelStripButtonIdx
// {
//     int muteBtnID;
//     int soloBtnID;
//     int selectBtnID;
//     int writeBtnID;
//     int assignBtnID;
//     int recRdyBtnID;
// };

// //
// struct ChannelStripButtonIdx channelStripButtonIDarray[]
// {
//     //mute  solo   select write  assign recRdy
//     {0x051, 0x052, 0x053, 0x054, 0x055, 0x056}, // Channel strip 1
//     {0x059, 0x05A, 0x05B, 0x05C, 0x05D, 0x05E}, // 2
//     {0x041, 0x042, 0x043, 0x044, 0x045, 0x046}, // 3
//     {0x049, 0x04A, 0x04B, 0x04C, 0x04D, 0x04E}, // 4
//     {0x031, 0x032, 0x033, 0x034, 0x035, 0x036}, // 5
//     {0x039, 0x03A, 0x03B, 0x03C, 0x03D, 0x03E}, // 6
// };
