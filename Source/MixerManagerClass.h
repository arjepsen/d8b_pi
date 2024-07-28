/*
  ==============================================================================

    MixerManagerClass.h
    Created: 14 Apr 2023 10:12:41pm
    Author:  anders

    Mixermanager is responsible for maintaining the comm receiver threads that
    receives messages from the console and then posts events in the eventbus.
    It is also responsible for running the mixer initialization/boot script.

  ==============================================================================
*/

#pragma once

#include "ChannelClass.h"
#include "ChannelStripClass.h"
#include "CircularBuffer.h"
#include "FXSlotClass.h"
#include "IOSlotClass.h"
#include "MixerInitScripts.h"
#include "SettingsClass.h"
#include <JuceHeader.h>
#include <array>
#include <cstdint>
#include <termios.h>
#include <unordered_map>
#include "ChannelIDMap.h"
#include "ChannelStripComponent.h"
#include "EventBusClass.h"
#include "BrainComClass.h"
#include "DspComClass.h"

class MixerManager
{
private:
    // References to singletons
    Settings &settings; 
    //MasterChannel &masterChannel;   
    EventBus &eventBus;     
    BrainCom &brainCom;
    DspCom &dspCom;
    CircularBuffer &circBuffer;
    HexToIntLookup &hexToIntLookup;

    // Declare the IO slot objects.
    IOSlot *ioSlotA;
    IOSlot *ioSlotB;
    IOSlot *ioSlotC;
    IOSlot *ioSlotAlt;
    IOSlot *ioSlotClock;
    IOSlot *ioSlotDigital;

    // Declare the FX slot objects.
    FXSlot *fxSlotA;
    FXSlot *fxSlotB;
    FXSlot *fxSlotC;
    FXSlot *fxSlotD;

    // Create a buffer with the defined width from the circular buffer.
    char msgBuffer[BUFFER_WIDTH];

	// Various members/variables
    bool isInitializing = false; // Flag for avoid starting multiple init threads.


    MixerManager();  // Constructor
    ~MixerManager(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    MixerManager(const MixerManager &) = delete;
    MixerManager &operator=(const MixerManager &) = delete;

    std::thread messageHandlerThread;

    void handleBufferMessage();

public:
    static MixerManager &getInstance(); // Returns a reference to the instance.

    bool setBrainPort(std::string deviceString);
    bool setDspPort(std::string deviceString);
    const std::string getBrainPort() const;
    const std::string getDspPort() const;

    const std::map<std::string, std::string> getUsbPortMap();
    bool getBrainBoostState();
    void setBrainBoostState(bool);

    void initIOSlot(IOSlot *ioSlotPtr, IOSlotID ioSlotID);
    void initFXSlot(FXSlot *fxSlotPtr, FXSlotID fxSlotID);

    void initMixer(juce::Button *initMixerBtn);

};

// Singleton modifications
inline MixerManager &MixerManager::getInstance()
{
    static MixerManager instance;
    return instance;
}

