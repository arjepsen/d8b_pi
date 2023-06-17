/*
  ==============================================================================

    EventBusClass.cpp
    Created: 11 Jun 2023 8:59:37pm
    Author:  anders

  ==============================================================================
*/

#include "EventBusClass.h"
#include <stdexcept>

EventBus::EventBus() {}
EventBus::~EventBus() {}

// void EventBus::bankEventSubscribe(EventType eventType, const std::string& channelStripID, std::function<void(const std::string &)> callback, bool isChannel)


void EventBus::bankEventSubscribe(Bank bank,
                                  BankEventType eventType,
                                  const std::string &channelStripID,
                                  std::function<void(const std::string &, Bank, const std::string &)> callback,
								  std::function<void(Bank, const std::string &)> removeSubscriptionCallback)
{
    if (channelStripID.length() != 2)
    {
        printf("WRONG LENGTH OF CHANNEL STRIP ID\n");
        exit(1);
    }

    auto &callbackMap =
        (bank == LINE_BANK)      ? lineBankCallbacks[eventType]
        : (bank == TAPE_BANK)    ? tapeBankCallbacks[eventType]
        : (bank == EFFECTS_BANK) ? effectsBankCallbacks[eventType]
        : (bank == MASTERS_BANK) ? mastersBankCallbacks[eventType]
                                 : (throw std::runtime_error("ERROR IN DETERMINING BANK FOR EVENT CALLBACK\n"));

    // SO - first we need some way to let the previous channel know that it no longer controls this channel strip.
    // This kinda means we need yet another callback for removing that subscription from the channel.

    // Check if the specified key (channelstripID) exists, before calling removal for old subscription.
    if (callbackMap.find(channelStripID) != callbackMap.end())
    {
        callbackMap[channelStripID].removeChannelSubscriptionCallback(bank, channelStripID);
    }

	// Old associatied channel should be updated. Now put in the new callbacks.
	callbackMap[channelStripID].channelObjectCallback = callback;
	callbackMap[channelStripID].removeChannelSubscriptionCallback = removeSubscriptionCallback;


    // if (isChannel)
    // {
    //     callbackMap[channelStripID].channelObjectCallback = callback;
	// 	callbackMap[channelStripID].removeChannelSubscriptionCallback = removeSubscriptionCallback;
    // }
    // else
    //     callbackMap[channelStripID].channelStripComponentCallback = callback;
}


void EventBus::lineBankEventPost(BankEventType eventType, const std::string &channelStripID, const std::string &eventValue)
{
    // Assume the command callback is registered... otherwise we might have an issue....
    // But we skip checking for the sake of speed :-D
    // Maybe if it is already effecient enough we can implement safechecks.

    printf("EVENT BUS POST. Calling mr. lineBankCallback");

    // Calling mr. callback.
    // lineBankCallbacks[eventType][ChannelStripID](eventValue);

    try // Guard to protect from exceptions
    {
        lineBankCallbacks[eventType][channelStripID].channelObjectCallback(eventValue, LINE_BANK, channelStripID);
        //lineBankCallbacks[eventType][channelStripID].channelStripComponentCallback(eventValue);
    }
    catch (const std::exception &e)
    {
        std::string err = e.what();
        printf("Exception in EventBusClass::lineBankPost (crap!): %s", err.c_str());
    }
}



// void EventBus::tapeBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue) {}
// void EventBus::effectsBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue) {}
// void EventBus::mastersBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue) {}