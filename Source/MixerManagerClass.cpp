/*
  ==============================================================================

    MixerManagerClass.cpp
    Created: 14 Apr 2023 10:12:41pm
    Author:  anders

  ==============================================================================
*/

#include "MixerManagerClass.h"
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

// UNCOMMENT TO ENABLE DEBUG MESSAGES.
#define MANAGER_DEBUG_MESSAGES

#ifdef MANAGER_DEBUG_MESSAGES
#define DEBUG_MSG(format, ...) printf("MNGR_DBG: " format, ##__VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) ((void)0) // do {} while (0)
#endif

#define WELCOME_STRING "90u44v38v42v94u36v2Ev30v20v28v61v6Cv70v68v61v29vCDu4Dv41v43v4Bv49v45vD4u44v49v47v49v54v41v4Cv"

// ############################ CONSTRUCTOR ####################################
MixerManager::MixerManager()
    : settings(Settings::getInstance()),
      eventBus(EventBus::getInstance()),
      masterChannel(MasterChannel::getInstance()),
      brainCom(BrainCom::getInstance()),
      dspCom(DspCom::getInstance()),
      circBuffer(CircularBuffer::getInstance()),
      isInitializing(false)
      //messageHandler(&lineBankMessageHandler)   
{
    std::cout << "MixerManger Constructor" << std::endl;
    DEBUG_MSG("\n===================== MIXER MANAGER CONSTRUCTOR =======================\n");
    // std::array automatically initializes elements to default value -
    // so Channel constructor gets called automatically.
    // So the channel objects are ready for when we instantiate the map of channelstrips after mixer init script.

    // Pass the callback function to the message handlers.
    // MAKE DIFFERENT CALLBAKCS FOR EACH BRAIN MESSAGE (fader, vpot, btndwn, btnup)



    // Set up channelstrip map of pointers to channel objects. (initially Line bank, ch. 1-24)
    for (int i = 0; i < CHANNEL_STRIP_COUNT; i++)
    {
        // Create the 2-digit hex code for the channelstrip.
        std::stringstream stream;
        stream << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << i;
        std::string hexCode = stream.str();

        // Create an element in the map
        channelStripMap[hexCode] = &channels[i];
    }
}

MixerManager::~MixerManager()
{
    // delete fxSlotA;
}

// ############################### METHODS ####################################
const Channel &MixerManager::getChannel(uint8_t id) const
{
    if (id < CHANNEL_COUNT)
        return channels[id];
    throw std::out_of_range("Invalid channel ID");
}

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
    return settings.getBrainPort();
}

const std::string MixerManager::getDspPort() const
{
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
                // Perform Mixer initialization.
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

                isInitializing = false;

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
// #################################################################################
void MixerManager::handleBufferMessage()
{
    printf("Running Reader Thread\n");
    while (true)
    {
        std::string message = circBuffer.pop();

        // CHANGING TO HANDLE BANK IN EVENT BUS
        //messageHandler->handleMessage(message);

        printf("LineBank recieved: %s\n", message.c_str());

        // Check last char for message Category:
        char msgCategory = message.back(); // Might need to make a check to ensure message is not empty....?

        switch (msgCategory)
        {
            case 'f': // FADER MOVED.
            {
                std::string channelStripID = message.substr(0, 2); // Get channel strip ID from message
                std::string value = message.substr(2, 2);       // Get fader position from message

                
                eventBus.postEvent(FADER_EVENT, channelStripID, value, CONSOLE_EVENT);
                break;
            }
            case 'v': // V-Pot turned
            {
                printf("Pot Message: %s", message.c_str());
                // Decipher which pot.
                // Send relevant DSP command.
                // Send relevant Brain command (led's)
                // update UI
                break;
            }
        }
    }
}

// // ########################################################################################
// // Heartbeat handler method - this is invoked when an "l" or "k" is received from the Brain
// // ########################################################################################
// void MixerManager::heartBeatReceived()
// {
//     // For now, do nothing, but maybe we can implement a timer/watchdog thingie?
// }



// #########################################################################################
// Method to a pointer to the channel Strip Component array from the MainComponent.
// #########################################################################################
void MixerManager::setChannelStripComponentArray(ChannelStripComponent *chStripArray)
{
    chStripComponents = chStripArray;
}

// ###############################################################################
// Faders sends a hex value (byte) on a linear scale.
// This method calculates the corresponding value to show in the ui.
// ###############################################################################

// TODO MAYBE THIS BELONGS IN CHANNELSTRIPCOMPONENT??

// double MixerManager::mapToSliderScale(std::string hexValue)
// {
//     int decimalValue = std::stoi(hexValue, nullptr, 16);

//     double returnValue = log10((decimalValue * logFactor) + 1) * 100 - 90;

//     return returnValue;
// }

// ##########################################################################################
// This method gets called by MainComponent's callback function, when a fader is moved in UI
// It converts the value from the fader logarithmic scale to a byte. The value sent to the
// Channel object, which sends the command to the DSP board.
// Then a command is also sent to the Brain, to move the fader.

// WE STILL NEED TO EXPAND THIS TO TAKE SELECTED BANK INTO CONSIDERATION!!!!!!!!!!!!

// ###########################################################################################

// TODO MAYBE THIS BELONGS IN CHANNELSTRIPCOMPONENT??

// void MixerManager::handleUiFaderMove(std::string channelStripComponentID, float newFaderValue)
// {
//     // Change value from logarithmic fader scale, to linear 0-255 scale. (approximation)
//     float x = 28.3f * powf(10.0f, 0.01f * newFaderValue + 0.9f) - 28.3f;

//     // Round to integer
//     int roundedX = static_cast<int>(x + 0.5f);

//     // Convert to 2-digit hex string
//     std::stringstream stream;
//     stream << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << roundedX;
//     std::string faderHexValue = stream.str();

//     // Tell channel object to send volume command, and update it's record.
//     // IF BANK == LINE BANK:
//     channelStripMap[channelStripComponentID]->setVolume(faderHexValue, dspDescriptor);

//     // Send a fader move command to the Brain:
//     std::string brainCommand = channelStripComponentID + faderHexValue + "f";
//     write(brainDescriptor, brainCommand.c_str(), brainCommand.length());
// }

// void MixerManager::handleUiMasterFaderMove(float newMasterFaderValue)
// {
//     // Change value from logarithmic fader scale, to linear 0-255 scale. (approximation)
//     float x = 28.3f * powf(10.0f, 0.01f * newMasterFaderValue + 0.9f) - 28.3f;

//     // Round to integer
//     int roundedX = static_cast<int>(x + 0.5f);

//     // Convert to 2-digit hex string
//     std::stringstream stream;
//     stream << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << roundedX;
//     std::string faderHexValue = stream.str();

//     // Tell MasterChannel singleton to send volume command to dsp, and update it's record.
//     masterChannel.setMasterVolume(faderHexValue, dspDescriptor);

//     // Send a fader move command to the brain.
//     std::string brainCommand = "18" + faderHexValue + "f";
//     write(brainDescriptor, brainCommand.c_str(), brainCommand.length());
// }
