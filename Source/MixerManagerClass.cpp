/*
  ==============================================================================

    MixerManagerClass.cpp
    Created: 14 Apr 2023 10:12:41pm
    Author:  anders

  ==============================================================================
*/

#include "MixerManagerClass.h"
#include "SharedDataStructures.h"
#include <algorithm>
#include <cmath>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include "Debug.h"


#define WELCOME_STRING "90u44v38v42v94u36v2Ev30v20v28v61v6Cv70v68v61v29vCDu4Dv41v43v4Bv49v45vD4u44v49v47v49v54v41v4Cv"

// ############################ CONSTRUCTOR ####################################
MixerManager::MixerManager()
    : settings(Settings::getInstance()),
      eventBus(EventBus::getInstance()),
      //masterChannel(MasterChannel::getInstance()),
      brainCom(BrainCom::getInstance()),
      dspCom(DspCom::getInstance()),
      circBuffer(CircularBuffer::getInstance()),
      hexToIntLookup(HexToIntLookup::getInstance())
      //isInitializing(false)
{
    // We already have an array of channels.
    // TODO: maybe seperate class for FX, RET, MIDI, Grp and Bus?
    // TODO: should this be made inherited classes of a Channel interface? yes.


    // // Create the initial association between channel and channelstrip by writing
    // // in pointers to the channels.
    // for (int i = 0; i < CHANNEL_STRIP_COUNT; i++)
    // {
    //     channelStripArray[i].setChannelAssociation(LINE_BANK, &channelArray[i]);
    //     channelStripArray[i].setChannelAssociation(TAPE_BANK, &channelArray[i + CHANNEL_STRIP_COUNT]);
    //     channelStripArray[i].setChannelAssociation(EFFECTS_BANK, &channelArray[i + CHANNEL_STRIP_COUNT * 2]);
    //     channelStripArray[i].setChannelAssociation(MASTERS_BANK, &channelArray[i + CHANNEL_STRIP_COUNT * 3]);
    // }

    // // TODO: now, how do we handle the changing of association? Somehow we need
    // // to be able to get a hold of the pointers for the channels....?
    // FIX THIS NOW - HOW DO WE UPDATE ASSOCIATION? HOW DO WE GET THE POINTERS?
}

MixerManager::~MixerManager()
{
    // delete fxSlotA;
}

// ############################### METHODS ####################################
// const Channel &MixerManager::getChannel(uint8_t id) const
// {
//     if (id < CHANNEL_COUNT)
//         return channels[id];
//     throw std::out_of_range("Invalid channel ID");
// }

bool MixerManager::setBrainPort(std::string deviceString)
{
    std::string deviceStringStart = "/dev/ttyUSB";
    if (deviceString.substr(0, deviceStringStart.length()) == deviceStringStart)
    {
        settings.setBrainPort(deviceString);
        return true;
    }
    else
        return false;
}

bool MixerManager::setDspPort(std::string deviceString)
{
    std::string deviceStringStart = "/dev/ttyUSB";
    if (deviceString.substr(0, deviceStringStart.length()) == deviceStringStart)
    {
        settings.setDspPort(deviceString);
        return true;
    }
    else
        return false;
}

const std::string MixerManager::getBrainPort() const
{
    DEBUG_MSG("Manager getting brainport: %s\n", settings.getBrainPort().c_str());
    return settings.getBrainPort();
}

const std::string MixerManager::getDspPort() const
{
    DEBUG_MSG("Manager getting dspport: %s\n", settings.getDspPort().c_str());
    return settings.getDspPort();
}

const std::map<std::string, std::string> MixerManager::getUsbPortMap()
{
    // Tell settings to update the device map.
    settings.findUSBDevices();

    // Get the map of devices and return it.
    const std::map<std::string, std::string> usbDeviceMap = settings.getUSBDevices();
    return usbDeviceMap;
}

bool MixerManager::getBrainBoostState()
{
    return settings.getBrainBoostState();
}

void MixerManager::setBrainBoostState(bool doWeWantTurbo)
{
    settings.setBrainBoostState(doWeWantTurbo);
}

void MixerManager::initIOSlot(IOSlot *ioSlotPtr, IOSlotID ioSlotID)
{
    switch (ioSlotID)
    {
        case TAPEIO_SLOT_A:
            ioSlotA = ioSlotPtr;
            break;
        case TAPEIO_SLOT_B:
            ioSlotB = ioSlotPtr;
            break;
        case TAPEIO_SLOT_C:
            ioSlotC = ioSlotPtr;
            break;
        case TAPEIO_SLOT_ALTIO:
            ioSlotAlt = ioSlotPtr;
            break;
        case CLOCK_IO_SLOT:
            ioSlotClock = ioSlotPtr;
            break;
        case DIGITAL_IO_SLOT:
            ioSlotDigital = ioSlotPtr;
            break;

        default:
            printf("INVALID IO SLOT INIT");
            exit(1);
    }
}

void MixerManager::initFXSlot(FXSlot *fxSlotPtr, FXSlotID fxSlotID)
{
    switch (fxSlotID)
    {
        case FX_SLOT_A:
            fxSlotA = fxSlotPtr;
            break;
        case FX_SLOT_B:
            fxSlotB = fxSlotPtr;
            break;
        case FX_SLOT_C:
            fxSlotC = fxSlotPtr;
            break;
        case FX_SLOT_D:
            fxSlotD = fxSlotPtr;
            break;
        default:
            printf("INVALID SLOT INIT");
            exit(1);
    }
}

void MixerManager::initMixer(juce::Button *initMixerBtn)
{
    // The method will create a separate thread in which the mixer initialization script will be run.
    // This is to preserve a responsive UI, although the user shouldn't really mess around untill
    // inits are done.

    if (isInitializing)
        printf("INITIALIZATION SCRIPT IS ALREADY RUNNING!!!\n");
    else
    {
        isInitializing = true;

        // Run the init script within it's own thread.

        juce::Thread::launch(
            [this, initMixerBtn]()
            {
                // Perform Mixer initialization / boot.
                InitErrorType initResult = initializeMixer();

                // Check if an error returned
                if (initResult != INIT_SUCCESS)
                {
                    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                                                           "Mixer Initialization Error code",
                                                           juce::String(initResult),
                                                           "OK");
                }

                // ========================= MIXER INITIALIZED ===============================
                // Set up communication singletons for threaded communication.

                // Port should be set in settings class.
                brainCom.setPort(settings.getBrainPort());
                dspCom.setPort(settings.getDspPort());

                // Baud rate for BrainCom depends on boost state.
                brainCom.setBaudRate(settings.getBrainBoostState() ? B230400 : B115200);
                dspCom.setBaudRate(B115200);

                // Start the communication threads.
                brainCom.startReceiverThread();
                dspCom.startReceiverThread();
                messageHandlerThread = std::thread(&MixerManager::handleBufferMessage, this);

                // At this point the mixer is booted, and communication is 
                // initialized.
                // Now we need to send the initial settings to the console.
                
                // TODO: maybe adapt to accept filename?
                eventBus.loadSettings();

                // eventBus.initializeChannels();
                // eventBus.initializeUiStrips();
                //eventBus.initializeChannelStrips();


                // Run through the channel strips, and update settings.
                // This does nothing for now, but ,later when settings are saved and loaded
                // this will have an impact.
                // for (ChannelStrip chStrip : channelStripArray)
                // {
                //     chStrip.updateChStrip(LINE_BANK);
                // }


                isInitializing = false;

                // Tell eventBus to activate the UI listeners.
                eventBus.enableUiListeners();

                // All the way at the end of the thread, re-enable the init button.
                juce::MessageManager::callAsync([initMixerBtn]()
                                                {
                                                    initMixerBtn->setEnabled(true); // Re-enable the button
                                                });
            });
    }
}

// #################################################################################
// This method is a "dispatcher" sort of thing, which is called
// and run in a thread. It continually pops messages off the buffer, and
// sends them to the current selected handler (depending on which bank is selected.)
    // REMEMBER: ChStripID is an enumeration starting at "0" for channel "1"
// #################################################################################
void MixerManager::handleBufferMessage()
{
    printf("Running Reader Thread\n");

    // Run the message receiver thread loop
    while (true)
    {
        // Get the next message from the buffer
        size_t msgLength = circBuffer.pop(msgBuffer);

        // Check last char for message Category (-1 for correct indexing):
        char msgCategory = msgBuffer[msgLength - 1]; 

        // TODO: Might need to make a check to ensure message is not empty....?
        // TODO: How about console reset: "R"??


        switch (msgCategory)
        {
            case 'f': // Fader was moved, command has format: XXYYf
            {
                // No bounds checking - we assume that the messages ending
                // with 'f' and 'v' are always having the strip ID as the first 
                // two chars.

                // So: Convert the first two chars to an integer
                char hexValueString[2] = {msgBuffer[0], msgBuffer[1]};
                ChStripID channelStripID = static_cast<ChStripID>(hexToIntLookup.hexToInt(hexValueString));

                // Copy the fader value of the message (index 2 & 3)
                char faderValue[2] = {msgBuffer[2], msgBuffer[3]};

                // Post fader event in the eventbus
                eventBus.postFaderEvent(channelStripID, faderValue, CONSOLE_EVENT);
                break;
            }
            case 'v': // V-Pot turned
            {
                // Almost same as above, but because of buttons, it's faster to duplicate code.
                // Get the ID of the activated channel. Convert to ChStripID
                char hexIdString[2] = {msgBuffer[0], msgBuffer[1]};
                ChStripID channelStripID = static_cast<ChStripID>(hexToIntLookup.hexToInt(hexIdString));
                
                // Get value from message.
                char vPotValue[2] = {msgBuffer[2], msgBuffer[3]};
                int vPotChangeValue = static_cast<int8_t>(hexToIntLookup.hexToInt(vPotValue));

                // eventBus.postEvent(VPOT_EVENT, channelStripID, value, CONSOLE_EVENT);
                eventBus.postVpotEvent(channelStripID, vPotChangeValue, CONSOLE_EVENT);
                break;
            }
            case 's':   
            case 'u':   
            {
                // TODO: How to handle shift and other dual presses....
                // TODO: How to handle doubl-click (like select for fat channel...)

                // Button was pressed. "s" means pressed, "u" means released
                // std::string channelStripID = message.substr(0, 2); // Get channel strip ID from message
                // std::string value = message.substr(2, 2);       // Get fader position from message
                // eventBus.postEvent(BUTTON_EVENT, channelStripID, value, CONSOLE_EVENT);
                // So what to do.
                // how do we handle differentiation between channel buttons, and others

                // From manual:
                // DoubleClicking Select opens/closes fat channel on screeen.
                // Holding opens channel prefs in VFD display.
                // Holding two selects opens link options page in VFD
                // Holding Solo while selecting other solos ....
                // works also while holding ALT.

                ////////////////////////////////////////////////////////////////
                printf("button was pressed - mixermanager\n");

                // Convert button ID to an integer
                char hexValueString[3] = {msgBuffer[0], msgBuffer[1], msgBuffer[2]};
                int buttonID = hexToIntLookup.hex3ToInt(hexValueString);


                eventBus.postButtonEvent(buttonID, static_cast<ButtonAction>(msgCategory));

                break;
            }
            default:
                printf("OTHER MESSAGE: %s\n", msgBuffer);
                // TODO: handle other possible messages from the brain, but in particular also DSP
        }
    }
}

