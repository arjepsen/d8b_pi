/*
  ==============================================================================

    MixerManagerClass.cpp
    Created: 14 Apr 2023 10:12:41pm
    Author:  anders

  ==============================================================================
*/

#include "MixerManagerClass.h"
#include <algorithm>
#include <fcntl.h>
#include <numeric>
#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>


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
      isInitializing(false),
      messageHandler(&lineBankMessageHandler)
{
    std::cout << "MixerManger Constructor" << std::endl;
    DEBUG_MSG("\n===================== MIXER MANAGER CONSTRUCTOR =======================\n");
    // std::array automatically initializes elements to default value -
    // so Channel constructor gets called automatically.
    // So the channel objects are ready for when we instantiate the map of channelstrips after mixer init script.

}

MixerManager::~MixerManager()
{
    //delete fxSlotA;
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
        settings.settingSetBrainPort(deviceString);
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
        settings.settingSetDspPort(deviceString);
        return true;
    }
    else
        return false;
}

const std::string MixerManager::getBrainPort() const
{
    return settings.settingsGetBrainPort();
}

const std::string MixerManager::getDspPort() const
{
    return settings.settingsGetDspPort();
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
    return settings.settingsGetBrainBoostState();
}

void MixerManager::setBrainBoostState(bool doWeWantTurbo)
{
    settings.settingsSetBrainBoostState(doWeWantTurbo);
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

                // ============= MIXER INITIALIZED ===============
                // Now we need to set up everything else

                // Set up the communication ports for further communication
                brainDescriptor = openSerialPort(getBrainPort().c_str(), getBrainBoostState() ? B230400 : B115200);
                dspDescriptor = openSerialPort(getDspPort().c_str(), B115200);

                // // Forward the descriptors to the handlers
                // lineBankMessageHandler.setComDescriptors(brainDescriptor, dspDescriptor);
                // tapeBankMessageHandler.setComDescriptors(brainDescriptor, dspDescriptor);
                // effectsBankMessageHandler.setComDescriptors(brainDescriptor, dspDescriptor);
                // mastersBankMessageHandler.setComDescriptors(brainDescriptor, dspDescriptor);

                // Start the communication threads.
                brainReceiverThread = std::thread(&MixerManager::brainMessageReceiver, this);
                dspReceiverThread = std::thread(&MixerManager::dspMessageReceiver, this);
                messageHandlerThread = std::thread(&MixerManager::handleBufferMessage, this);

                // Channels are already constructed by now, so they should have their default values already.
                // We need a way to send all the settings in a channel, and here do it for all channels.


                // // So now, add 24 objects to the channelstripmap, which will run their constructor.
                // // Set their KEY in the map as the hex code (00 - 17) corresponding to the brain messages.
                // for (int i = 0; i < CHANNEL_STRIP_COUNT; ++i)
                // {
                //     std::stringstream stream;
                //     stream << std::hex << std::setw(2) << std::setfill('0') << i;
                //     std::string hexCode = stream.str();
                    
                //     // Instantiate a ChannelStrip object as value to this key.
                //     channelStripMap[hexCode] = ChannelStrip();
                // }

                // DO THE ARRAY OR MAP INSTEAD

                // Run a for loop over all channels, and send their dsp values.
                for (auto channel : channels)
                {
                    uint8_t channelVolume = channel.getVolume();
                }
                // run a for loop over channelstrips. This is boot, so assign channel 1-24 to channelstrips, then load their settings to the strip.

                isInitializing = false;

                // All the way at the end of the thread, re-enable the init button.
                juce::MessageManager::callAsync([initMixerBtn]()
                                                {
                                                    initMixerBtn->setEnabled(true); // Re-enable the button
                                                });
            });
    }
}

// ###################################################################################
// Method that opens a serial port for communication. It returns a file descriptor.
// ###################################################################################
int MixerManager::openSerialPort(const char *devicePath, speed_t baudRate)
{
    DEBUG_MSG("Opening Serial port for %s @ %d\n", devicePath, baudRate);
    struct termios options;
    int fd = open(devicePath, O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        perror("Error opening serial device");
        // exit(1);
        return -1;
    }

    // Get current options
    tcgetattr(fd, &options);

    // Set baud rate
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);

    // Set terminal mode
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    options.c_oflag &= ~(OPOST | ONLCR);

    // Set data format
    options.c_cflag &= ~(PARENB | PARODD | CMSPAR | CSTOPB | CSIZE);
    options.c_cflag |= CS8;

    // Set input control options
    options.c_iflag &= ~(INPCK | IXOFF | IUCLC | IXANY | IMAXBEL | IUTF8);
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 5;

    // Apply the new settings
    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, &options);

    // Return the file descriptor for the port.
    return fd;
}

// ###############################################################################
// This method sets up the loop for the thread that reads messages from the Brain.
// The method assumes that ALL messages end with a lower case letter, and also
// assumes, that a message always is sent in full, and are NOT "split" by the
// heartbeats ('l / 'k').
// ###############################################################################
void MixerManager::brainMessageReceiver()
{
    // Clear screen before entering loop.
    write(brainDescriptor, "01u", 3);
    usleep(20000);

    char recvChar = '\0';
    std::string message = "";
    int result;

    // Clear com buffer for starting out
    tcflush(brainDescriptor, TCIOFLUSH);

    DEBUG_MSG("running brain message loop\n");
    // Run the infinite loop.
    while (true)
    {
        result = read(brainDescriptor, &recvChar, 1);

        if (result == 1) // One char was recevied.
        {
            message += recvChar;

            // A lower case letter means message complete.
            if (recvChar >= 'a' && recvChar <= 'z')
            {
                // printf("brain message: %s\n", message.c_str());

                if (recvChar == 'l' || recvChar == 'k')
                {
                    printf("hearbeat: %c\n", recvChar);
                    heartBeatReceived();
                }
                else
                    circBuffer.push(message.c_str()); // Push message to the circular buffer.
                message = "";                         // Reset message string.
            }
        }

        else if (result < 0)
        {
            perror("Error reading from file descriptor");
            exit(1);
        }
        else if (result == 0) // 0 chars recevied - EOF
        {
            // Should we add any special functionality here??
        }
    }

    // We probably shouldn't get here... but just in case.
    printf("\n\n########################## BRAIN LOOP EXITED!!!!!!! ##############################\n");
}

// ###############################################################################
// Like the brainMessageReceiver... same... but different...
// ###############################################################################
void MixerManager::dspMessageReceiver()
{
    // Set up the file descriptor for the DSP port.
    // const int DSP = openSerialPort(getDspPort().c_str(), B115200);

    char recvChar = '\0';
    std::string message = "";
    int result;

    // Clear com buffer for starting out
    tcflush(dspDescriptor, TCIOFLUSH);

    DEBUG_MSG("Running dsp message loop\n");
    while (true)
    {
        result = read(dspDescriptor, &recvChar, 1);

        if (result == 1)
        {
            message += recvChar;
            if (recvChar >= 'a' && recvChar <= 'z')
            {
                printf("dsp message: %s\n", message.c_str());
                // Lower case letter received, message complete. push to buffer.
                circBuffer.push(message.c_str());
                message = "";
            }
        }

        else if (result < 0)
        {
            perror("Error reading from file descriptor");
            exit(1);
        }
        else if (result == 0)
        {
            // EOF
            // should any handling happen here?
        }
    }

    // This should not happen
    printf("\n\n############### DSP READER THREAD EXITED!!!!\n");
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
        messageHandler->handleMessage(message);
    }
}

// ########################################################################################
// Heartbeat handler method - this is invoked when an "l" or "k" is received from the Brain
// ########################################################################################
void MixerManager::heartBeatReceived()
{
    // For now, do nothing, but maybe we can implement a timer/watchdog thingie?
}

// #########################################################################################
// This method is used for switching bank message handler.
// The Brain will send the same messages on various activity (fader move, etc..), regardless
// of which bank is selected, so we need to assign a specific message handler, depending on
// which bank is currently selected.
// #########################################################################################
void MixerManager::setBank(Bank bank)
{
    switch (bank)
    {
        case LINE_BANK:
            messageHandler = &lineBankMessageHandler;
            break;
        case TAPE_BANK:
            messageHandler = &tapeBankMessageHandler;
            break;
        case EFFECTS_BANK:
            messageHandler = &effectsBankMessageHandler;
            break;
        case MASTERS_BANK:
            messageHandler = &mastersBankMessageHandler;
            break;
        default:;
            // Handle invalid bank input.
    }
}

void MixerManager::messageHandlerCallback(const MessageData& messageData)
{
    DEBUG_MSG("Callback Function in MixerManager called.\n");

    // Handle the message data:

    // Read the channelstrip number, retrieve audio channel from map/array.
    // Use that info to figure out which channel object to use
    // Call the specific channels method for sending messages.

    // WHERE DO WE HANDLE THE UI CHANGES? HERE? IN CHANNEL? IN MSGHANDLER????
}