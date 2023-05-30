/*
  ==============================================================================

    MessageHandlerInterface.h
    Created: 29 May 2023 7:12:33pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include <string>

class MessageHandler
{
public:
    virtual ~MessageHandler() {}
    virtual void handleMessage(const std::string &message) = 0;
    virtual void setComDescriptors(int brainDescriptor, int dspDescriptor) = 0;
};