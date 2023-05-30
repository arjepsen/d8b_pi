/*
  ==============================================================================

    Bank1MessageHandlerClass.h
    Created: 29 May 2023 7:13:13pm
    Author:  anders


    Handler class for the Line bank (Bank 1)
  ==============================================================================
*/

#pragma once

#include "MessageHandlerInterface.h"
#include "unordered_map"

class LineBankMessageHandler : public MessageHandler
{
public:
    void handleMessage(const std::string &message) override;
    void setComDescriptors(int brainDescriptor, int dspDescriptor) override;

private:

	int brain;
	int dsp;

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
            {"0A", "0B"},
            {"0B", "23"},
            {"0C", "00"}, // Here it changes... channel Strip 13
            {"0D", "18"},
            {"0E", "01"},
            {"0F", "19"},
            {"10", "02"},
            {"11", "1A"},
            {"12", "03"},
            {"13", "1B"},
            {"14", "04"},
            {"15", "1C"},
            {"16", "05"},
            {"17", "1D"},	// Channelstrip 24
			{"18", "4C"}
		};
};
