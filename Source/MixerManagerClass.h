/*
  ==============================================================================

    MixerManagerClass.h
    Created: 14 Apr 2023 10:12:41pm
    Author:  anders

    The MixerManager class is where the UI is tied together with the rest
    of the program. Everything should pass through here, so we have
    seperation of concerns.
    This should be a "singleton" class, so only one instance can exist.
  ==============================================================================
*/

#pragma once
#include "ChannelClass.h"
#include "IOSlotClass.h"
#include "FXSlotClass.h"
#include "SettingsClass.h"
#include <array>
#include <cstdint>
#include <JuceHeader.h> 
#include <termios.h>
#include "MixerInitScripts.h"
#include "CircularBuffer.h"

class MixerManager
{
private:
    static constexpr uint8_t CHANNEL_COUNT = 56;
    std::array<Channel, CHANNEL_COUNT> channels;

    Settings &settings; // Reference to the Settings singleton.

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

    // Create the circular buffer object.
    CircularBuffer circBuffer;

    bool isInitializing;  // Flag for avoid starting multiple init threads.

    MixerManager();  // Constructor
    ~MixerManager(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    MixerManager(const MixerManager &) = delete;
    MixerManager &operator=(const MixerManager &) = delete;

    // Communication threads
    std::thread brainReceiverThread;
    std::thread dspReceiverThread;
    std::thread bufferMessageHandlerThread;

    // Thread methods
    void brainMessageReceiver();
    void dspMessageReceiver();
    void bufferMessageHandler();

    // Other Methods
    int openSerialPort(const char *devicePath, speed_t baudRate);
    void heartBeatReceived();

    // Brain Message Lookup Table Conversion Map
    std::unordered_map<std::string, std::string> channelMap =
    {
      {"00", "06"}, // Channel strip 1
      {"01", "1E"}, // Channel strip 2 etc....
      {"02", "07"},
      {"03", "1F"},
      {"04", "08"},
      {"05", "20"},
      {"06", "09"},
      {"07", "21"},
      {"08", "0A"},
      {"09", "22"},
      {"10", "0B"},
      {"11", "23"},
      {"12", "00"}, // Here it changes... channel Strip 13
      {"13", "18"},
      {"14", "01"},
      {"15", "19"},
      {"16", "02"},
      {"17", "1A"},
      {"18", "03"},
      {"19", "1B"},
      {"20", "04"},
      {"21", "1C"},
      {"22", "05"},
      {"23", "1D"}
    };

public:
    static MixerManager &getInstance(); // Returns a reference to the instance.

    // Public methods for interacting with channels
    const Channel &getChannel(uint8_t id) const; // Ensure channels stay in their place in the array.

    // Public method for providing access to the Settings instance
    // const Settings &getSettings() const;

    bool setBrainPort(std::string deviceString);
    bool setDspPort(std::string deviceString);
    const std::string getBrainPort() const;
    const std::string getDspPort() const;

    const std::map<std::string, std::string> getUsbPortMap();
    bool getBrainBoostState();
    void setBrainBoostState(bool);

    void initIOSlot(IOSlot * ioSlotPtr, IOSlotID ioSlotID);
    void initFXSlot(FXSlot * fxSlotPtr, FXSlotID fxSlotID);

    void initMixer(juce::Button* initMixerBtn);


    // TODO: Add methods to handle communication with the Brain and DSP boards
};

// Singleton modifications
inline MixerManager &MixerManager::getInstance()
{
    static MixerManager instance;
    return instance;
}

//////////////////////////////////
// VARIOUS NOTES //

// Mixermanager object should be instantiated in MainComponent. (So it's accessible for everything else...)
// Remaining classes should be instantiated in mixermanager.
// So channels gets instantiated in mixermanager.
// But channelstripcomponent are also instantiated in maincomponent, since they are a gui element.
// Channels are data storage. When updated, the mixermanager also updates the channelstripcomponent.

// There are four banks - and the two first are channels.
// SO MAYBE THE CONNECTION BETWEEN CHANNELSTRIP AND CHANNEL LIES ENTIRELY IN MIXERMANAGER?

// This could make BANK selection easier, since it's simply an update in mixermanager, which then updates the
// channelstrip graphically.

// There should be a "ChangeBank" method here  in mixermanager, which is called, when a bank button is pressed
// (either physically, or though gui)

// There are several sections on the mixer - but some of them are maybe just purely variables?
// for example "Assignments" only contain buttons...

// SELECT button is probably a variable in MixerManager.