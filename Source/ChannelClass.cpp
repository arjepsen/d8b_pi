/*
  ==============================================================================

    ChannelClass.cpp
    Created: 14 Apr 2023 9:48:22pm
    Author:  anders

  ==============================================================================
*/

#include "ChannelClass.h"
#include "ChannelIDMap.h"
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

// Constructor. Set up channel number and get channel dsp ID from channelIDMap.h
Channel::Channel()
    : channelID{dspChannelIDs[nextChannelNumber]},
      channelNumber{nextChannelNumber},
      eventBus(EventBus::getInstance())
{
    DEBUG_MSG("\n=============== CHANNEL CONSTRUCTOR =====================\n");

    // SUBSCRIBE TO EVENT BUS STUFF...

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
std::string Channel::getID()
{
    return channelID;
}

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
        write(dspDescriptor, volumeCommand.c_str(), volumeCommand.length());
}

// ##########################################################################
// This method subscribes to line bank events, using the Event Bus's method.
// ##########################################################################
void Channel::subscribeToLineBankEvents()
{
    // WHAT ABOUT NON-CHANNELSTRIP EVENTS? Maybe handled by different subscription?

    // Iterate through all associated channelstrips in the line bank.

    // NEED TO RETHINK....MORE ASSOCIATED CHANNEL STRIPS....DIFFERENT CALLBACK METHOD
    //     USE DIFFERENT METHODS FOR EACH EVENT TYPE

        for (const std::string &channelStripID : associatedChannelStrips[LINE_BANK])
    {
        // Subscribe to Fader Events
        eventBus.bankEventSubscribe(
            LINE_BANK,
            FADER_EVENT,    // The event type.
            channelStripID, // ID for the channel strip controling this channel, when line bank is chosen.
            [this](const std::string &valueString)
            { this->setVolume(valueString); }, // The callback function.
            true                               // This is the channel class, not the UI channel strip class.
        );

        eventBus.bankEventSubscribe(
            LINE_BANK,
            VPOT_EVENT,     // The event type.
            channelStripID, // ID for the channel strip controling this channel, when line bank is chosen.
            [this](const std::string &valueString)
            { this->setVolume(valueString); }, // The callback function.
            true                               // This is the channel class, not the UI channel strip class.
        );

        eventBus.bankEventSubscribe(
            LINE_BANK,
            BUTTON_EVENT,   // The event type.
            channelStripID, // ID for the channel strip controling this channel, when line bank is chosen.
            [this](const std::string &valueString)
            { this->setVolume(valueString); }, // The callback function.
            true                               // This is the channel class, not the UI channel strip class.
        );
    }
}

// void Channel::removeSubscription(BankEventType eventType, std::string channelStripID)
// {
// 	// Find and remove the association to the given channelstrip
// 	associatedChannelStrips[(BankID)eventType].erase(channelID);
// }

void Channel::removeSubscription(Bank bank, std::string channelStripIDtoRemove)
{
    // Check if there is a subscription to that ID:
    associatedChannelStrips[bank].erase(channelStripIDtoRemove);
}

void Channel::setChannelStrip(std::string stripID)
{
    channelStripID = stripID;
    subscribeToLineBankEvents();
}

void Channel::linkDspDescriptor(int &dspDescriptor)
{
    dspDescriptor = dspDescriptor;
}


// ###########################################################################################################################
// This is a callback function that is fired by the EventBus, when a fader is moved.
// First it sends a DSP command to update the volume. Then it will iterate over all associalted channelstrips 
// on the console, and update them. Finaly it will post another event, which will update the channelStripComponents in the UI
// ###########################################################################################################################
void Channel::channelStripFaderEvent(std::string &faderValue, Bank bank, std::string &channelStripID)
{

	// TODO: THIS MAY NEED TO CHANGE A BIT, IF WE CONTINUE TO USE THE CHANNEL CLASS FOR EFFECTS, MIDI, BUS, GROUPS, ETC.

    // TODO: Should we also use this for UI fader events?? - moving a fader in the UI? Then that fader shouldnt be removed from the set.....


    // Construct DSP volume command, and send it.
    std::string volumeCommand = channelID + "cX" + faderValue + "Q";
    if (!mute)
        write(dspDescriptor, volumeCommand.c_str(), volumeCommand.length());

    // Update volume member
    volume = faderValue;

	// Retrieve the set of associated channel strips on the current bank.
	std::unordered_set<std::string> associateStrips = associatedChannelStrips[bank];

    // Remove the calling channelStripID (it's already moved)
    associateStrips.erase(channelStripID);

    // Iterate through the set, send move command
    for (auto& stripID : associateStrips)
    {
        std::string faderCommand = stripID + faderValue + "f";
        write(brainDescriptor, faderCommand.c_str(), volumeCommand.length());
    }

    // Now make an event post, for the UI strips to get updated
	eventBus.associateChStripEventPost(associatedChannelStrips[bank], FADER_EVENT, faderValue);
}