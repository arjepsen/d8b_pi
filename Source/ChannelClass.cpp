/*
  ==============================================================================

    ChannelClass.cpp
    Created: 14 Apr 2023 9:48:22pm
    Author:  anders

  ==============================================================================
*/

#include "ChannelClass.h"
#include "ChannelIDMap.h"
#include <iomanip>
#include <sstream>
#include <unistd.h>

// UNCOMMENT TO ENABLE DEBUG MESSAGES.
#define CHANNEL_DEBUG_MESSAGES

#ifdef CHANNEL_DEBUG_MESSAGES
#define DEBUG_MSG(format, ...) printf("CHNL_DBG: " format, ##__VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) ((void)0) // do {} while (0)
#endif

// Set first channel ID. This will increment with every channel object constructed.
uint8_t Channel::nextChannelNumber = 0;


// ############################### CONSTRUCTOR #########################################
// Channel Constructor. Set up channel number and get channel dsp ID from channelIDMap.h
// Convert channel number to channelStripID for the associate map.
// #####################################################################################
Channel::Channel()
    : channelID{dspChannelIDs[nextChannelNumber]}, // Get unique channel ID for dsp commands.
      channelNumber{nextChannelNumber},            // Set unique channel number.
      eventBus(EventBus::getInstance()),           // Get the singleton instances
      brainCom(BrainCom::getInstance()),
      dspCom(DspCom::getInstance())
{
    // Set up variables for initial channel strip event subscription
    int initialAssociateStripNumber = channelNumber;
    Bank initialAssociateBank = LINE_BANK;

    // Adjust for channels above 24.
    if (channelNumber > 23)
    {
        initialAssociateStripNumber -= 23;
        initialAssociateBank = TAPE_BANK;
    }

    // Convert Channel Number to a corresponding channel strip ID.
    std::stringstream stream;
    stream << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << initialAssociateStripNumber;
    std::string initialAssociateChannelStripID = stream.str();

    // Put the initial channel strip ID in the associate map, for the initial bank:
    std::unordered_set<std::string> initialStripSet {initialAssociateChannelStripID};
    associatedChannelStrips[initialAssociateBank] = initialStripSet;


    // Subscribe to events on the given bank and strip.
    eventBus.bankEventSubscribe
    (
        initialAssociateBank,
        //FADER_EVENT,
        initialAssociateChannelStripID,
        [this](const std::string &faderValue, const Bank bank, const std::string &channelStripID)
        { this->channelStripFaderEventCallback(faderValue, bank, channelStripID); },
        [this](const std::string &vpotValue, const Bank bank, const std::string &channelStripID)
        { this->channelStripVpotEventCallback(vpotValue, bank, channelStripID); },
        [this](const std::string &buttonValue, const Bank bank, const std::string &channelStripID)
        { this->channelStripVpotEventCallback(buttonValue, bank, channelStripID); },
        [this](const Bank bank, const std::string &channelStripID)
        { this->removeChStripAssociationCallback(bank, channelStripID); }
    );


    // INITIAL SETTINGS - WE CAN SET THEM HERE ARBITRARILY, OR USE A SPECIFIC CALL??
    mute = false;

    // solo = false;

    // pan = 127;  // Center (0x7F)

    // assignments[0] = true;  // Assign to L-R. All others should default to false.

    // Initialize other member variables to default values...

    // Increment the nextChannelNumber for the next channel construction.
    nextChannelNumber++;
}

// Implement other member functions...


// #######################################################################################
// This method updates the channel object's volume member, and sends a command to the DSP
// #######################################################################################
void Channel::setVolume(std::string volumeValue)
{
    // Update volume member
    volume = volumeValue;

    // Construct DSP command, and send it.
    std::string volumeCommand = channelID + "cX" + volumeValue + "Q";
    if (!mute)
        //write(*dspDescriptorPtr, volumeCommand.c_str(), volumeCommand.length());
        dspCom.send(volumeCommand);
}

// ###########################################################################################################################
// This is a callback function that is fired by the EventBus, when a fader is moved.
// First it sends a DSP command to update the volume. Then it will iterate over all associalted channelstrips
// on the console, and update them. Finaly it will post another event, which will update the channelStripComponents in the UI
// ###########################################################################################################################
void Channel::channelStripFaderEventCallback(const std::string faderValue, const Bank bank, const std::string &channelStripID)
{

    // TODO: THIS MAY NEED TO CHANGE A BIT, IF WE CONTINUE TO USE THE CHANNEL CLASS FOR EFFECTS, MIDI, BUS, GROUPS, ETC.

    printf("fader event callbacker\n");

    // Construct DSP volume command, and send it.
    std::string volumeCommand = channelID + "cX" + faderValue + "Q";

    if (!mute)
        //write(*dspDescriptorPtr, volumeCommand.c_str(), volumeCommand.length());
        dspCom.send(volumeCommand);

    // Update volume member
    volume = faderValue;

    // Retrieve the set of associated channel strips on the current bank.
    std::unordered_set<std::string> associateStrips = associatedChannelStrips[bank];

    // Remove the calling channelStripID (it was moved by hand...)
    associateStrips.erase(channelStripID);

    // Iterate through the set, send move command
    for (auto &stripID : associateStrips)
    {
        std::string faderCommand = stripID + faderValue + "f";
        //write(*brainDescriptorPtr, faderCommand.c_str(), volumeCommand.length());
        brainCom.send(faderCommand);
    }

    // Now make an event post, for the UI strips to get updated
    eventBus.associateChStripEventPost(associatedChannelStrips[bank], FADER_EVENT, faderValue);
}

void Channel::channelStripVpotEventCallback(const std::string vpotValue, const Bank bank, const std::string &channelStripID)
{
    // The supplied value is likely not a specific placement, but rather a number of how fast/far the pot was turned.
    // So the procedure here is:
    // 1 - DEDUCE WHAT THE POT IS CONTROLING
    // 2 - CALCULATE NEW VALUE FROM OLD VALUE ON SUPPLIED CHANGE-VALUE
    // 3 - SEND DSP (OR BRAIN?) COMMAND TO UPDATE WHAT THE VPOT IS CHOSEN TO CONTROL
    // 4 - UPDATE PRIVATE MEMBER
    // 5 - SEND AN ASSOCIATE EVENT POST FOR UPDATING THE UI
}



void Channel::channelStripButtonEventCallback(const std::string buttonID, const Bank bank, const std::string &channelStripID)
{
    // SOME WORK TO BE DONE HERE...
}

// ####################################################################################################
// This method is a callback for removing the supplied channel strip from the set of associated strips.
// This is called from the event bus, when an association is updated.
// ####################################################################################################
void Channel::removeChStripAssociationCallback(const Bank bank, const std::string channelStripID)
{
    associatedChannelStrips[bank].erase(channelStripID);
}

std::string Channel::getID()
{
    return channelID;
}

// void Channel::linkDspDescriptor(int *dspDescriptor)
// {
//     printf("############============= LIIIINKIIIIING ==============##########################\n");
//     dspDescriptorPtr = dspDescriptor;
//     if (!dspDescriptor)
//         printf("passed pointer invalid\n");
//     if (!dspDescriptorPtr)
//         printf("link invalid\n");
// }

