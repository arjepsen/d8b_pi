/*
  ==============================================================================

    BankEffectsMessageHandlerClass.cpp
    Created: 29 May 2023 7:34:32pm
    Author:  anders

  ==============================================================================
*/

#include "BankEffectsMessageHandlerClass.h"


void EffectsBankMessageHandler::handleMessage(const std::string &message)
{

}

void EffectsBankMessageHandler::setCallbackFunction(std::function<void(const MessageData&)> callbackFunction)
{
  callback = callbackFunction;
}

// void EffectsBankMessageHandler::setComDescriptors(int brainDescriptor, int dspDescriptor)
// {
// 	brain = brainDescriptor;
// 	dsp = dspDescriptor;
// }