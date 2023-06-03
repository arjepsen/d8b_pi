/*
  ==============================================================================

    Bank2MessageHandlerClass.h
    Created: 29 May 2023 7:13:46pm
    Author:  anders

    Handler Class for the Tape bank messages (Bank 2)
  ==============================================================================
*/

#pragma once

#include "MessageHandlerInterface.h"

class TapeBankMessageHandler : public MessageHandler
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