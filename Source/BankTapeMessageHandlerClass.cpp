/*
  ==============================================================================

    Bank2MessageHandlerClass.cpp
    Created: 29 May 2023 7:13:46pm
    Author:  anders

  ==============================================================================
*/

#include "BankTapeMessageHandlerClass.h"

void TapeBankMessageHandler::handleMessage(const std::string &message)
{

}

void TapeBankMessageHandler::setCallbackFunction(std::function<void(const MessageData&)> callbackFunction)
{
  callback = callbackFunction;
}

// void TapeBankMessageHandler::setComDescriptors(int brainDescriptor, int dspDescriptor)
// {
// 	brain = brainDescriptor;
// 	dsp = dspDescriptor;
// }