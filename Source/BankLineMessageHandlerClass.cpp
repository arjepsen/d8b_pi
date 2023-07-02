/*
  ==============================================================================

    BankLineMessageHandlerClass.cpp
    Created: 29 May 2023 7:13:13pm
    Author:  anders

  ==============================================================================
*/

#include "BankLineMessageHandlerClass.h"
#include <unistd.h>
// #include <termios.h>

#include <iostream>
#include <string>


// MAYBE WE MOVE THE DIFFERENCE IN BANK HANDLER TO THE EVENT BUS?
// SO, THE MESSAGE HANDLER WILL BE SIMPLER, AND IN THE EVENT BUS, 
// THERE WILL INSTEAD BE VARIOUS POINTERS TO THE SPECIFIC METHOD,
// DEPENDING ON CHOSEN BANK?
// FOR NOW: KEEP THESE STRUCTURES HERE, BUT IMPLEMENT THE DIFFERENT BANKS IN EVENTBUS

void LineBankMessageHandler::handleMessage(const std::string &message)
{

    printf("LineBank recieved: %s\n", message.c_str());

    // Check last char for message Category:
    char msgCategory = message.back(); // Might need to make a check to ensure message is not empty....?

    switch (msgCategory)
    {
        case 'f': // FADER MOVED.
        {
            std::string channelStripID = message.substr(0, 2); // Get channel strip ID from message
            std::string value = message.substr(2, 2);       // Get fader position from message

            // eventBus.postEvent(FADER_EVENT, )
			// eventBus.lineBankEventPost(FADER_EVENT, channelStripID, value);


            // MessageData msgStruct;
            // msgStruct.channelStrip = channelStripID;
            // msgStruct.bank = LINE_BANK;
            // msgStruct.value = value;

            // callback(msgStruct);  // INSTEAD USE EVENT BUS POST
            break;
        }
        case 'v': // V-Pot turned
        {
            // Decipher which pot.
            // Send relevant DSP command.
            // Send relevant Brain command (led's)
            // update UI
            break;
        }
    }
};

// void LineBankMessageHandler::setComDescriptors(int brainDescriptor, int dspDescriptor)
// {
//     brain = brainDescriptor;
//     dsp = dspDescriptor;
// }


void LineBankMessageHandler::setCallbackFunction(std::function<void(const MessageData&)> callbackFunction)
{
	callback = callbackFunction;
}