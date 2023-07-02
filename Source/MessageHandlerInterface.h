/*
  ==============================================================================

    MessageHandlerInterface.h
    Created: 29 May 2023 7:12:33pm
    Author:  anders

  ==============================================================================
*/

#pragma once

//#include "BankEnum.h"
#include "EventBusClass.h"
#include <functional>
#include <string>

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
    typedef std::function<void(const MessageData &)> CallbackFunction;

	MessageHandler() : eventBus(EventBus::getInstance()) {}

    virtual ~MessageHandler() {}
    virtual void handleMessage(const std::string &message) = 0;
    // virtual void setComDescriptors(int brainDescriptor, int dspDescriptor) = 0;

    // Virtual method to set the callback function
    virtual void setCallbackFunction(CallbackFunction callback) = 0;

protected:
	EventBus &eventBus;     // Reference to EventBus singleton.
};