/*
  ==============================================================================

    FaderValueLookup.h
    Created: 4 Jul 2023 8:17:17pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include "Debug.h"

/*******************************************************************************
 * @brief This class is used to optimize the conversion between the linear
 *      values of the physical fader, and the logarithmic scale shown in the ui.
 *      The console fader can only report 256 different values, so we pre-compute
 *      them and then make a quick lookup during runtime.
 ******************************************************************************/
class FaderValueLookup
{

  private:

    // Lookup array for converting ui value to console command value.
    char dspHexLookupArray[1001][3];

    // Lookup array for converting console value (0 - 255) to the logarithmic scale 
    // used by the faders.
    float preComputedLog10Values[256];


    FaderValueLookup()
    {
        DEBUG_MSG("FADERVALUE LOOKUP CONSTRUCTOR\n");

        // Precompute the array of logarithms the first time this class is instantiated.
        // This is done to avoid slow log10 computations during runtime, so we can look up the result in an array.
        if (preComputedLog10Values[0] != -90.0) // Check if array has already been computed. (then first element would be -90)
        {
            const float logFactor = 9.0 / 255; // Factor used in linear byte to fader log scale conversion.
            for (int i = 0; i <= 0xFF; ++i)
            {
                preComputedLog10Values[i] = static_cast<float>(log10((i * logFactor) + 1) * 100 - 90);
            }
        }



        // Precompute the map of fader/vpot values and their corresponding DSP hex strings:
        for (int i = 0; i <= 1000; ++i)
        {
            float faderValue = -90.0f + (i * 0.1f); // Convert 'i' to the values reported by the ui fader.
            int dspValue = static_cast<int>((pow(10, (faderValue + 90) / 100.0) - 1) / 9.0 * 255);
            sprintf(dspHexLookupArray[i], "%02X", dspValue);
        }
    }

  public:
    // Get the single instance of FaderValueLookup
    static FaderValueLookup &getInstance();

    // Delete copy constructor and assignment operator (singleton)
    FaderValueLookup(const FaderValueLookup &) = delete;
    FaderValueLookup &operator=(const FaderValueLookup &) = delete;

    // Getter method for converting a ui fader/vpot value to the 2-char hex string
    // used for the console commands.
    inline const char* getDspHexValue(float uiFaderValue) const
    {
        int index = static_cast<int>((uiFaderValue + 90.0) * 10);
        return dspHexLookupArray[index];
    }

    // Getter method for converting a console fader value (integer) to the
    // logarithmic value used by the fader representation of sound volume. (decibel)
    inline const float* getLog10Value(int consoleValue) const
    {
        return &preComputedLog10Values[consoleValue];
    }
};

// Singleton modifications
inline FaderValueLookup &FaderValueLookup::getInstance()
{
    static FaderValueLookup instance;
    return instance;
}
