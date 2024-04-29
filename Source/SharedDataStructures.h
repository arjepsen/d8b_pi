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

// Lookup table for converting hexadecimal digits to their integer values
static const unsigned char hex_values[256] = {
    ['0'] = 0,   ['1'] = 1,   ['2'] = 2,   ['3'] = 3,
    ['4'] = 4,   ['5'] = 5,   ['6'] = 6,   ['7'] = 7,
    ['8'] = 8,   ['9'] = 9,   ['A'] = 10,  ['B'] = 11,
    ['C'] = 12,  ['D'] = 13,  ['E'] = 14,  ['F'] = 15,
    // All other values can remain 0, as input is always valid uppercase hex.
};

inline int hexToInt(const char (&hexString)[2]) 
{
    return (hex_values[(unsigned char)hexString[0]] << 4) | hex_values[(unsigned char)hexString[1]];
}

// int kurt = hex_to_int("F9")


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
    {                 // Private constructor
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


