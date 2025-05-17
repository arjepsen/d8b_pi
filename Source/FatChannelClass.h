/*
  ==============================================================================

    FatChannelClass.h
    Created: 17 May 2025 6:21:53pm
    Author:  anders

  ==============================================================================
*/

#include "ChannelClass.h"
#include "ChannelStripClass.h"

#pragma once

class FatChannel
{
  public:
    // clang-format off
    enum State { EQ, GATE, COMPRESSOR, PLUGIN, SETTINGS};
    // clang-format on

    FatChannel() : selectedChannelStrip(nullptr), selectedChannel(nullptr), state(EQ) {}

    void setState(State newState) { state = newState; };



  private:
    State state = EQ;
};