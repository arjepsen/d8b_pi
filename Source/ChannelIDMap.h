/*
  ==============================================================================

    ChannelIDMap.h
    Created: 4 Jun 2023 7:09:29pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <string>
#include <unordered_map>
#include <array>

// Brain Message Lookup Table Conversion Map
extern std::unordered_map<std::string, std::string> lineBankChannelMap;
extern std::unordered_map<std::string, std::string> tapeBankChannelMap;
extern const char DSP_CH_ID_ARRAY[][3];
extern const char CH_STRIP_ID_ARRAY[24][3];