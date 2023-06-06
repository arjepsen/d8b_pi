/*
  ==============================================================================

    MessageHandlerInterface.h
    Created: 29 May 2023 7:12:33pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <string>
#include <functional>
#include "BankEnum.h"

struct MessageData
{
	std::string channelStrip;
	Bank bank;
	std::string value;
};

class MessageHandler
{
public:
	// Define the callback function type
	typedef std::function<void(const MessageData&)> CallbackFunction;

    virtual ~MessageHandler() {}
    virtual void handleMessage(const std::string &message) = 0;
    //virtual void setComDescriptors(int brainDescriptor, int dspDescriptor) = 0;

	// Virtual method to set the callback function
	virtual void setCallbackFunction(CallbackFunction callback) = 0;
};