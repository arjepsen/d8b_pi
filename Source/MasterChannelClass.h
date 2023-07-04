/*
  ==============================================================================

    MasterChannel.h
    Created: 11 Jun 2023 12:14:52pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include "BrainComClass.h"
#include "DspComClass.h"
#include "EventBusClass.h"
#include <string>

class MasterChannel
{
private:
    // Private constructor, since this is a singleton
    MasterChannel();
    //~MasterChannel();

    EventBus &eventBus;
    BrainCom &brainCom;
    DspCom &dspCom;

    const std::string MASTER_STRIP_ID = "18";

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    MasterChannel(const MasterChannel &) = delete;
    MasterChannel &operator=(const MasterChannel &) = delete;

    std::string masterVolume;

public:
    static MasterChannel &getInstance(); // Returns a reference to the instance.

    void masterStripFaderEventCallback(const std::string faderValue, Bank bank, const std::string &channelStripID, EventSource source);
    void masterStripVpotEventCallback(const std::string vpotValue, const Bank bank, const std::string &channelStripID, EventSource source);
    void masterStripButtonEventCallback(const std::string buttonID, const Bank bank, const std::string &channelStripID, EventSource source);

    void removeMasterStripAssociationCallback(const Bank bank, const std::string channelStripID);

    void setMasterVolume(std::string volumeString, int dspDescriptor);
};

// Singleton modifications
inline MasterChannel &MasterChannel::getInstance()
{
    static MasterChannel instance;
    return instance;
}