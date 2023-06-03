/*
  ==============================================================================

    BankEffectsMessageHandlerClass.h
    Created: 29 May 2023 7:34:32pm
    Author:  anders

    Handler class for the Effects bank messages (Bank 3)
  ==============================================================================
*/

#pragma once

#include "MessageHandlerInterface.h"

class EffectsBankMessageHandler : public MessageHandler
{
public:
    void handleMessage(const std::string &message) override;
	void setCallbackFunction(std::function<void(const MessageData&)> callbackFunction) override;
    //void setComDescriptors(int brainDescriptor, int dspDescriptor) override;

private:
	std::function<void(const MessageData&)> callback;
    // int brain;
    // int dsp;
};