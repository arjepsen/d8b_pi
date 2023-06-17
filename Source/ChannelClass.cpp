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
	for (const std::string& channelStripID : associatedChannelStrips[LINE_BANK])
	{
		// Subscribe to Fader Events
		eventBus.bankEventSubscribe
		(
			LINE_BANK,		
			FADER_EVENT, 	// The event type.
			channelStripID, 	// ID for the channel strip controling this channel, when line bank is chosen.
			[this](const std::string& valueString) { this->setVolume(valueString);},	// The callback function.
			true	// This is the channel class, not the UI channel strip class.
		);

		eventBus.bankEventSubscribe
		(
			LINE_BANK,
			VPOT_EVENT, 	// The event type.
			channelStripID, 	// ID for the channel strip controling this channel, when line bank is chosen.
			[this](const std::string& valueString) { this->setVolume(valueString);},	// The callback function.
			true	// This is the channel class, not the UI channel strip class.
		);

		eventBus.bankEventSubscribe
		(
			LINE_BANK,
			BUTTON_EVENT, 	// The event type.
			channelStripID, 	// ID for the channel strip controling this channel, when line bank is chosen.
			[this](const std::string& valueString) { this->setVolume(valueString);},	// The callback function.
			true	// This is the channel class, not the UI channel strip class.
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

void Channel::channelStripFaderEvent(std::string& faderValue)
{
	// 1 - Send dsp command.
	// 2 - update internal volume member.
	// 3 - Post a new associateChStrip event, pass a set of associated chStripID's WITHOUT the one that started all this nonsense.


    // Construct DSP command, and send it.
    std::string volumeCommand = channelID + "cX" + faderValue + "Q";
    if (!mute)
        write(dspDescriptor, volumeCommand.c_str(), volumeCommand.length());

    // Update volume member
    volume = faderValue;

	// Make a new Set without the current channelstrip.... but here we dont know what strip was called......


	// Send an event post, for associated channelstrips to be updated


}