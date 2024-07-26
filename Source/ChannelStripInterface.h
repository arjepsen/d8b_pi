/*
  ==============================================================================

    ChannelStripInterface.h
    Created: 19 Jun 2024 10:31:24pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include "SharedDataStructures.h"
#include "ChannelClass.h"



class ChannelStripInterface
{
    public:
        virtual ~ChannelStripInterface() {}
        virtual void updateChannelVolume(Bank currentBank, const char (&faderValue)[2]) = 0;
        virtual void setChannelAssociation(Bank activeBank, Bank associationBank, Channel *channelPtr = nullptr) = 0;
        virtual void updateChStrip(Bank currentBank = LINE_BANK) = 0;
        virtual void updateFaderPosition(Bank currentBank = LINE_BANK) = 0;

        virtual int vPotEventHandler(Bank currentBank, int vPotChangeValue, VpotFunction vPotFunc, EventSource source) = 0;
        virtual void updateVpotLeds(Bank bank, VpotFunction vPotFunc) = 0;
        virtual int getChannelStripVpotValue(Bank currentBank, VpotFunction currentVpotFunction) = 0;
};