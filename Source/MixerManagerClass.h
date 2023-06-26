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


#include "BankEffectsMessageHandlerClass.h"
#include "BankLineMessageHandlerClass.h"
#include "BankMastersMessageHandlerClass.h"
#include "BankTapeMessageHandlerClass.h"
#include "ChannelClass.h"
//#include "ChannelStripClass.h"
#include "CircularBuffer.h"
#include "FXSlotClass.h"
#include "IOSlotClass.h"
#include "MessageHandlerInterface.h"
#include "MixerInitScripts.h"
#include "SettingsClass.h"
#include <JuceHeader.h>
#include <array>
#include <cstdint>
#include <termios.h>
#include <unordered_map>
#include "BankEnum.h"
#include "ChannelIDMap.h"
#include "ChannelStripComponent.h"
#include "MasterChannelClass.h"
#include "EventBusClass.h"
#include "BrainWriterClass.h"
#include "DspWriterClass.h"

class MixerManager
{
private:
    // References to singletons
    Settings &settings; 
    MasterChannel &masterChannel;   
    EventBus &eventBus;     
    BrainWriter &brain;
    DspWriter &dsp;

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

    // MessageHandler objects
    MessageHandler *messageHandler;
    LineBankMessageHandler lineBankMessageHandler;
    TapeBankMessageHandler tapeBankMessageHandler;
    EffectsBankMessageHandler effectsBankMessageHandler;
    MastersBankMessageHandler mastersBankMessageHandler;

    // Channel and Channelstrip
    static constexpr uint8_t CHANNEL_COUNT = 48;
	static constexpr uint8_t CHANNEL_STRIP_COUNT = 24;
    std::array<Channel, CHANNEL_COUNT> channels;    // This will auto-instantiate channel objects
	std::unordered_map<std::string, Channel*> channelStripMap;

    // Component pointers
    ChannelStripComponent *chStripComponents;



	// Various members/variables
    bool isInitializing; // Flag for avoid starting multiple init threads.
	int brainDescriptor;
	int dspDescriptor;

    // Maybe this should be purely in the channelstrip component?
    // const float logFactor = 9.0 / 255;    // Factor used in linear byte to fader log scale conversion.


    MixerManager();  // Constructor
    ~MixerManager(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    MixerManager(const MixerManager &) = delete;
    MixerManager &operator=(const MixerManager &) = delete;

    // Communication threads
    std::thread brainReceiverThread;
    std::thread dspReceiverThread;
    std::thread messageHandlerThread;

    // Thread methods
    void brainMessageReceiver();
    void dspMessageReceiver();
    void handleBufferMessage();

    // Other Methods
    int openSerialPort(const char *devicePath, speed_t baudRate);
    void heartBeatReceived();
    double mapToSliderScale(std::string hexValue);

	// Callback function for handling the message structures from the message handlers
	// void messageHandlerCallback(const MessageData& messageData);
    // void faderMessageCallback(const MessageData& messageData);
    // void vPotMessageHandlerCallback(const MessageData& messageData);
    // void ButtonDwnMessageHandlerCallback(const MessageData& messageData);
    // void ButtonUpMessageHandlerCallback(const MessageData& messageData);

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

    void initIOSlot(IOSlot *ioSlotPtr, IOSlotID ioSlotID);
    void initFXSlot(FXSlot *fxSlotPtr, FXSlotID fxSlotID);

    void initMixer(juce::Button *initMixerBtn);

    void setBank(Bank bank);

    void setChannelStripComponentArray(ChannelStripComponent * chStripArray);
    // void handleUiFaderMove(std::string channelsTripComponentID, float newFaderValue);
    // void handleUiMasterFaderMove(float newMasterFaderValue);


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