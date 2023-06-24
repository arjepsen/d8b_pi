/*
  ==============================================================================

    EventBusClass.h
    Created: 11 Jun 2023 8:59:37pm
    Author:  anders

  ==============================================================================
*/

#pragma once

// #include <any>
#include "BankEnum.h"
#include <array>
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>

enum BankEventType
{
    FADER_EVENT,
    VPOT_EVENT,
    BUTTON_EVENT,
    EVENT_TYPE_COUNT // Smart way to set the element count in the array.
};

// So - a channel strip event:
// 1 - associated channel sends dsp command back to DSP.
// 2 - channel sends fader move to all OTHER associated strips in current bank.
// 3 - all associated UI channel strips gets updated.

// There seems to be a tight bond between associated channels, and ui strips...
// But the channel objects shouldn't know about ui strips... the ui strips should subscribe to the events.
// Maybe instead there should be a channelStrip class anyway?,
// So the channel subscribes to strip events.
// but ALSO a channelStrip subscribes to events?

// BUT this kind of means that we now would have THREE classes to make sure gets updated on any subscribe.....

class EventBus
{
private:
    EventBus();  // Constructor
    ~EventBus(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    EventBus(const EventBus &) = delete;
    EventBus &operator=(const EventBus &) = delete;

    // Structure to hold callbacks, the first is for the channelobject, the second is for removing a
    // strip from the channel object's association map.
    struct BankEventCallbacks
    {
        // First string is value, second string is channelstrip ID
        std::function<void(const std::string &, Bank, const std::string &)> channelObjectCallback;
        std::function<void(Bank, const std::string &)> removeChannelSubscriptionCallback;
    };

    // Structure the callback functions in four arrays (for the banks) of three maps, corresponding to the three event types.
    // Each map has the channelstrip ID as key.
    std::array<std::unordered_map<std::string, BankEventCallbacks>, EVENT_TYPE_COUNT> lineBankCallbacks;
    std::array<std::unordered_map<std::string, BankEventCallbacks>, EVENT_TYPE_COUNT> tapeBankCallbacks;
    std::array<std::unordered_map<std::string, BankEventCallbacks>, EVENT_TYPE_COUNT> effectsBankCallbacks;
    std::array<std::unordered_map<std::string, BankEventCallbacks>, EVENT_TYPE_COUNT> mastersBankCallbacks;

    // Structure for "associative events", like when more than one channelstrip is configured to the same channel on the same bank.
    std::array<
        std::unordered_map<std::string, std::unordered_map<std::string, std::function<void(std::string &)>>>,
        EVENT_TYPE_COUNT>
        lineBankAssociativeStripCallbacks;

    // Structure for associating channel strip ID's with a channelStripComponent callback.
    // std::unordered_map<std::string, std::function<void(const BankEventType, const std::string &)>> chStripComponentCallbacks;

    // Trying with the array for event type:
    std::array<std::unordered_map<std::string, std::function<void(const std::string &)>>, EVENT_TYPE_COUNT> chStripComponentCallbacks;

public:
    static EventBus &getInstance(); // Returns a reference to the instance.

    // ########################################## SUBSCRIPTION DECLARATIONS #########################################
    void bankEventSubscribe(Bank bank,
                            BankEventType eventType,
                            const std::string &channelStripID,
                            std::function<void(const std::string &, Bank, const std::string &)> callback,
                            std::function<void(Bank, const std::string &)> removeSubscriptionCallback);

    void chStripComponentSubscribe(const std::string stripID,
                                   const BankEventType eventType,
                                   std::function<void(const std::string &)> chStripCompCallback);

    // ############################################# EVENT POST DECLARATIONS #############################################
    void lineBankEventPost(BankEventType eventType, const std::string &channelStripID, const std::string &eventValue);
    // void tapeBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue);
    // void effectsBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue);
    // void mastersBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue);

    // this should receive a set of associate channels, an event type, and the value. DO WE NEED BANK?
    // this might be purely "cosmetical" - just making the associated channel strips reflect the original.
    void associateChStripEventPost(std::unordered_set<std::string> channelStrips, BankEventType eventType, std::string eventValue);
    void lineBankChannelEventPost(std::unordered_set<std::string> channelStrips, BankEventType eventType, std::string eventValue);
};

// Singleton modifications
inline EventBus &EventBus::getInstance()
{
    static EventBus instance;
    return instance;
}