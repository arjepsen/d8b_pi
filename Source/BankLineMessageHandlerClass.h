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

class LineBankMessageHandler : public MessageHandler
{
public:
    void handleMessage(const std::string &message) override;
    //void setComDescriptors(int brainDescriptor, int dspDescriptor) override;
    void setCallbackFunction(std::function<void(const MessageData&)> callbackFunction) override;

private:

	std::function<void(const MessageData&)> callback;

	// int brain;
	// int dsp;


};
