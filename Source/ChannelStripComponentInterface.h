/*
  ==============================================================================

    ChannelStripComponentInterface.h
    Created: 22 Jun 2024 5:51:13pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include "SharedDataStructures.h"

class ChannelStripComponentInterface
{
  public:
    virtual ~ChannelStripComponentInterface() {}
    virtual void activateEventListeners() = 0;
    virtual void faderMoveEventCallback(const char (&faderHexValue)[2]) = 0;
    virtual void vPotTurnEventCallback(int eventValue, VpotFunction currentVpotFunction) = 0;
};