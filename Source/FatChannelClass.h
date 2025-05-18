/*
  ==============================================================================

    FatChannelClass.h
    Created: 17 May 2025 6:21:53pm
    Author:  anders

  ==============================================================================
*/

// Ok, here is the idea:
// The fatchannel class works a bit like the channelstrips.
// Instead of having a channelstrip array of pointers to channelstrip INTERFACES in the eventbus,
// we instead make a more general ControlInterface, that works for both channelstrips and other master section subsections.
// (maybe call it controlSectionInterface?)
// Anyway, we then instantiate ONE fatchannel object, and place pointers to this same object in all four elements of the
// array, corresponding to the id of those four vpots.
// so the fatchannel must implement a method: postVpotEvent.
// But contrary to the channelstrip objects, this will directly call the currently selected channelstrip's channel
// We will need to keep a pointer to this currently selected channel - probably in the eventbus, since other stuff might also need to look it up...??
// So: the fatchannel will pass the call directly to the channel, which will handle the DSP commands to send.
// Then - like the channelstrip - the fatchannel will handle the ring-leds.
// It should also inform the UI.


#include "ChannelClass.h"
#include "ChannelStripClass.h"

#pragma once

class FatChannel : inherit from controlinterface
{
  public:
    // clang-format off
    enum State { EQ, GATE, COMPRESSOR, PLUGIN, SETTINGS};
    // clang-format on

    FatChannel() : selectedChannelStrip(nullptr), selectedChannel(nullptr), state(EQ) {}

    void setState(State newState) { state = newState; };


    void postVpotEvent(ChStripID channelStripID, int vPotChangeValue, EventSource source);

  private:
    State state = EQ;
};