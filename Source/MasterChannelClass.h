/*
  ==============================================================================

    MasterChannel.h
    Created: 11 Jun 2023 12:14:52pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include "BrainComClass.h"
#include "ChannelStripInterface.h"
#include "DspComClass.h"

#include "Debug.h"

constexpr int MASTER_VOL_DSP_CMD_LENGTH = 13;

class Channel; // Forward declaration of Channel - needed for setChannelAssociation.

class MasterChannel : public ChannelStripInterface
{
  private:
    // Private constructor, since this is a singleton
    MasterChannel();
    //~MasterChannel();

    // Bitmaps for maintaining current state of the LED's on the master strip.
    uint32_t ledOnBitmap = 0;
    uint32_t ledBlinkBitmap = 0;
    BrainCom &brainCom;
    DspCom &dspCom;

    const char MASTER_STRIP_ID[3] = "18";
    char faderMoveCmd[BRAIN_FADER_CMD_LENGTH] = {'1', '8', '0', '0', 'f'};                                                      // Initialize the master fader move braincommand.
    char dspMasterVolumeCommand[MASTER_VOL_DSP_CMD_LENGTH] = {'4', 'C', 'c', '9', 'X', '0', '0', 'Q', 'A', 'X', '0', '0', 'Q'}; // "4Cc9X--QAX--Q";

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    MasterChannel(const MasterChannel &) = delete;
    MasterChannel &operator=(const MasterChannel &) = delete;

    char masterVolume[3] = "00";

  public:
    static MasterChannel &getInstance(); // Returns a reference to the instance.

    void updateChannelVolume(Bank currentBank, const char (&faderValue)[2]) override;
    void updateChStrip(Bank currentBank = LINE_BANK) override;
    void updateFaderPosition(Bank currentBank = LINE_BANK) override;
    inline void setChannelAssociation(Bank activeBank, Bank associationBank, Channel *channelPtr = nullptr) override
    {
        DEBUG_MSG("The method should not get called on master channel strip...\n");
    }

    inline int vPotEventHandler(Bank currentBank, int vPotChangeValue, VpotFunction vPotFunc, EventSource source) override
    // inline void vPotEventHandler(Bank currentBank, const char (&vPotValue)[2], VpotFunction vPotFunc) override
    {
        // TODO
        return 127;
    }

    inline int getChannelStripVpotValue(Bank currentBank, VpotFunction currentVpotFunction) override
    {
        // TODO
        return 127;
    }

    void updateVpotLeds(Bank bank, VpotFunction vPotFunc) override;
};

// Singleton modifications
inline MasterChannel &MasterChannel::getInstance()
{
    static MasterChannel instance;
    return instance;
}
