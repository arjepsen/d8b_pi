/*
  ==============================================================================

    EventBusClass.cpp
    Created: 11 Jun 2023 8:59:37pm
    Author:  anders

  ==============================================================================
*/

#include "EventBusClass.h"
#include <stdexcept>

EventBus::EventBus()
{
    // Iterate over the array of eventtypes
    for (auto &callbackMap : chStripComponentCallbacks)
    {

        // Fill the ChannelStripComponent Callback lookup table with ID's
        for (int i = 0; i <= 0x17; ++i)
        {
            char buffer[3];
            snprintf(buffer, sizeof(buffer), "%02X", i); // Create the 2-digit Hex value for the channel strip ID
            std::string hexKey(buffer);                  // Casts to std::string
            callbackMap[hexKey];                         // Add the key with no defined value.
        }
    }
}

EventBus::~EventBus() {}

// ##############################################################################################################
// This method is used to "subscribe" to events on the different banks, by handing over two callback methods.
// The first method, is the callback that will be run when the event type happens on the given channel strip ID
// (like moving a fader on strip 2).
// The second callback, is used to remove the "old" callback, in cases where a channel "moves" it's subscription
// to a different channelstrip. (otherwise the channel would still respond to the old channelstrip.)
// NOTE: No channelstrip can be "empty" - there has to be a subscription.
//  (maybe this could be changed in the future?)
// ##############################################################################################################
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

    // Set a reference to the specific bank map of callbacks.
    auto &callbackMap =
        (bank == LINE_BANK)      ? lineBankCallbacks[eventType]
        : (bank == TAPE_BANK)    ? tapeBankCallbacks[eventType]
        : (bank == EFFECTS_BANK) ? effectsBankCallbacks[eventType]
        : (bank == MASTERS_BANK) ? mastersBankCallbacks[eventType]
                                 : (throw std::runtime_error("ERROR IN DETERMINING BANK FOR EVENT CALLBACK\n"));

    // Check if the specified key (channelstripID) exists, before calling removal for old subscription.
    if (callbackMap.find(channelStripID) != callbackMap.end())
    {
        callbackMap[channelStripID].removeChannelSubscriptionCallback(bank, channelStripID);
    }

    // Old associatied channel should be updated. Now put in the callbacks for the new channel.
    callbackMap[channelStripID].channelObjectCallback = callback;
    callbackMap[channelStripID].removeChannelSubscriptionCallback = removeSubscriptionCallback;
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

        // lineBankCallbacks[eventType][channelStripID].channelStripComponentCallback(eventValue);
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

// SO.... in the channel class we keep a SET of which channelStrips are associeated with that particular channel.
// Then actually there is no need to keep a seperate record of associated channelStripCOMPONENTS.....
// Instead, we make a constant lookup table of channelStripID : componentCallback here.
// BUT... we can't make that a constant..... can we?

// ####################################################################################################################
// When an event has fired, which calls a callback in a channel object (like moving a fader), the channel will the use
// this method to post an event that will update the channelStripComponents in the UI, by calling their callback from
// the lookup tables array "chStripComponentCallbacks".
// ####################################################################################################################
void EventBus::associateChStripEventPost(std::unordered_set<std::string> channelStrips, BankEventType eventType, std::string eventValue)
{
    // Iterate over the set, reference channelstripID to map of channelStripComponent callbacks, run the callback.
    for (auto &stripID : channelStrips)
    {
        // First index is the eventtype in the array, next index is the key of the map of callbacks.
        // The callback takes the event value as parameter - it just mimicks the mixer, so no need for knowing bank here.
        chStripComponentCallbacks[eventType][stripID](eventValue);
    }
}

// ################################################################################################
// This method is used to add the callbacks to the channel strip component callback lookup table.
// #################################################################################################
void EventBus::chStripComponentSubscribe(const std::string stripID,
                                         const BankEventType eventType,
                                         std::function<void(const std::string &)> chStripCompCallback)
{
    // Ensure that a valid strip ID has been provided, and that the callback has not already been set.
    if (chStripComponentCallbacks[eventType].count(stripID) && !chStripComponentCallbacks[eventType][stripID])
    {
        // Set callback
        chStripComponentCallbacks[eventType][stripID] = chStripCompCallback;
    }
    else
    {
        printf("ERROR ON SETTING CHANNELSTRIPCOMPONENT CALLBACK");
        exit(1);
    }
}