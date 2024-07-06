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
    // virtual void updateChannelVolume(Bank currentBank, const char (&faderValue)[2]) = 0;
    // virtual void setChannelAssociation(Bank activeBank, Bank associationBank, Channel *channelPtr = nullptr) = 0;
    // virtual void updateChStrip(Bank currentBank = LINE_BANK) = 0;
    // virtual void updateFaderPosition(Bank currentBank = LINE_BANK) = 0;

    virtual void faderMoveEventCallback(const char (&faderHexValue)[2]) = 0;
    virtual void activateEventListeners() = 0;
};