/*
  ==============================================================================

    SharedDataStructures.h
    Created: 25 Apr 2024 7:00:10pm
    Author:  anders

  ==============================================================================
*/

#pragma once

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
    CH_STRIP_COUNT
};

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


enum ButtonAction : char
{
    BTN_PRESS = 's',
    BTN_RELEASE = 'u'
};

// Lookup table for converting hexadecimal digits to their integer values
static const unsigned char hex_values[256] = {
    ['0'] = 0, ['1'] = 1, ['2'] = 2, ['3'] = 3, 
    ['4'] = 4, ['5'] = 5, ['6'] = 6, ['7'] = 7, 
    ['8'] = 8, ['9'] = 9, ['A'] = 10, ['B'] = 11, 
    ['C'] = 12, ['D'] = 13, ['E'] = 14, ['F'] = 15,
    // All other values can remain 0, as input is always valid uppercase hex.
};

/***************************************************************
 * @brief This function is used to convert a 2-char hex string
 *        like "F7" to an integer, using the lookup table.
 * 
 * @param hexString 
 * @return int 
 ***************************************************************/
inline int hexToInt(const char (&hexString)[2])
{
    return (hex_values[(unsigned char)hexString[0]] << 4) |
            hex_values[(unsigned char)hexString[1]];
}

/**************************************************************
 * @brief This function is used to convert a 3-char hex string
 *        like "2F3" to an integer.
 * 
 * @param hexString 
 * @return int 
 **************************************************************/
inline int hex3ToInt(const char (&hexString)[3]) {
    return (hex_values[(unsigned char)hexString[0]] << 8) |
           (hex_values[(unsigned char)hexString[1]] << 4) |
            hex_values[(unsigned char)hexString[2]];
}



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
    { // Private constructor
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



enum ButtonType
{
    // ChannelStrip buttons
    MUTE_BTN,
    SOLO_BTN,
    SELECT_BTN,
    WRITE_BTN,
    ASSIGN_BTN,
    REC_RDY_BTN,
    NUMBER_OF_CHSTRIP_BUTTONS
};

MAKE THE CALLBACK ARRAY IN EVENT BUS, WHICH THIS WILL BE USED TO INDEX