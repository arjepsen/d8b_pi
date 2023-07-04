/*
  ==============================================================================

    FaderValueLookup.h
    Created: 4 Jul 2023 8:17:17pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <unordered_map>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>

class FaderValueLookup
{

private:
    FaderValueLookup()
    {
        // Precompute the array of logarithms the first time this class is instantiated.
        // This is done to avoid slow log10 computations during runtime, so we can look up the result in an array.
        if (precomputedLog10Values[0] != -90.0) // Check if array has already been computed. (then first element would be -90)
        {
            const float logFactor = 9.0 / 255; // Factor used in linear byte to fader log scale conversion.
            for (int i = 0; i <= 0xFF; ++i)
            {
                precomputedLog10Values[i] = static_cast<float>(log10((i * logFactor) + 1) * 100 - 90);
            }
        }

        // Precompute the map of fader/vpot values and their corresponding DSP hex strings:
        for (int i = -900; i <= 100; i++)
        {
            float faderValue = i / 10.0f;
            int dspValue = static_cast<int>((pow(10, (faderValue + 90) / 100.0) - 1) / 9.0 * 255);

            // Convert dspValue to 2-digit uppercase hex string, and store in map
            std::stringstream hexStream;
            hexStream << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << dspValue;
            dspHexLookupMap[faderValue] = hexStream.str();
        }
    }


public:
    // Get the single instance of FaderValueLookup
    static FaderValueLookup& getInstance();

    // Delete copy constructor and assignment operator
    FaderValueLookup(const FaderValueLookup&) = delete;
    FaderValueLookup &operator=(const FaderValueLookup &) = delete;

    std::array<float, 256> precomputedLog10Values;   // Array for the 256 precomputed logarithmic values that faders and vpots can send.
    std::unordered_map<float, std::string> dspHexLookupMap;   // Map for correlating the possible UI fader values to their dsp hex values.
};

// Singleton modifications
inline FaderValueLookup &FaderValueLookup::getInstance()
{
    static FaderValueLookup instance;
    return instance;
}

