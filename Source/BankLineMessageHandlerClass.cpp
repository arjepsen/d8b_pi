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

void LineBankMessageHandler::handleMessage(const std::string &message)
{

    printf("LineBank recieved: %s\n", message.c_str());

    // Check last char for message Category:
    char msgCategory = message.back(); // Might need to make a check to ensure message is not empty....?

    switch (msgCategory)
    {
        case 'f': // FADER MOVED.
        {
            // Send a channel volume command to the DSP
            // The command has the form "xxcXyyQ", where xx is the channel ID, and yy is the value.
            // Note that yy can be a single digit, like 0. Max is FF.
            // Also note, that the master fader has a different command to send out (4Cc9XyyQAXyyQ) - likely this is stereo (9XyyQ / AXyyQ)

			// MUST RETHINK IT!
			// In each bank, a channelstrip can be assigned different channels!
			// Maybe make 2 or more arrays with pointers to the configured channel?

			// So here we would call a public method in mixermanager.....
			// something like chStripFaderMove(strip-index, value)... then this should pass that to the object?
			// Where should we then send the command from? Channel object? Mixermanager? all the way back here?

			// One way is, that the communication is kept in this class.....
			// So it will query mixermanager for the command info to send?
			// but... technically this object should not even knowthat mixermanager exists....


            std::string channel = message.substr(0, 2); // Get channel
            std::string value = message.substr(2, 2);

			MessageData msgStruct;
			msgStruct.channelStrip = 88;
			msgStruct.bank = LINE_BANK;
			msgStruct.value = 4398;

			callback(msgStruct);


            // // Build the full command in one go.
            // std::string faderDspCommand;
            // if (channel == "18") // Master fader
            // {
            //     faderDspCommand = channelMap[channel] + "c9X" + value + "QAX" + value + "Q";
            // }
            // else
            // {
            //     faderDspCommand = channelMap[channel] + "cX" + value + "Q";
            // }

            // // Send the command
            // write(dsp, faderDspCommand.c_str(), faderDspCommand.length());

            // update UI.


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