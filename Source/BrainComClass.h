/*
  ==============================================================================

    BrainWriterClass.h
    Created: 25 Jun 2023 10:52:04pm
    Author:  anders

    This Class is used for sending commands to the Brain board, AFTER the mixer
    has been initialized.

  ==============================================================================
*/

#pragma once

#include "ComBaseClass.h"

class BrainCom : public ComBase
{
  private:
    BrainCom();  // Constructor
    ~BrainCom(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    BrainCom(const BrainCom &) = delete;
    BrainCom &operator=(const BrainCom &) = delete;

    // Create a lookup table for received chars
    uint8_t actionTable[256] = {0}; // Initialize all to ACTION_NONE

    bool brainBoostState = false;

    virtual void messageReceiver() override;

    void heartbeatReceived();

  public:
    static BrainCom &getInstance(); // Returns a reference to the instance.
};

// Singleton modifications
inline BrainCom &BrainCom::getInstance()
{
    static BrainCom instance;
    return instance;
}

// ##################################################################################
// This method is responsible for what happens when a heart beat (l or k) is recevied
// ##################################################################################
inline void BrainCom::heartbeatReceived()
{
    // TODO: empty for now - maybe send the L / 0???
}
