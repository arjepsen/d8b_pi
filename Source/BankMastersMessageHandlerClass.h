/*
  ==============================================================================

    BankMastersMessageHandlerClass.h
    Created: 29 May 2023 7:35:17pm
    Author:  anders

    Handler class for the Masters bank messages (Bank 4)
  ==============================================================================
*/

#pragma once

#include "MessageHandlerInterface.h"

class MastersBankMessageHandler : public MessageHandler
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