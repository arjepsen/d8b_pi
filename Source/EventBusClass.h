/*
  ==============================================================================

    EventBusClass.h
    Created: 11 Jun 2023 8:59:37pm
    Author:  anders

  ==============================================================================
*/

#pragma once

// #include <any>
// #include "BankEnum.h"
#include <array>
#include <functional>
//#include <memory> // For unique_ptr
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

// !!!!!! DO NOT CHANGE ORDER !!!!!!! (used for indexing array)
enum BankEventType : int
{
    FADER_EVENT,
    VPOT_EVENT,
    BUTTON_EVENT,
    EVENT_TYPE_COUNT
};

enum Bank
{
    LINE_BANK,
    TAPE_BANK,
    EFFECTS_BANK,
    MASTERS_BANK
};

enum EventSource
{
    CONSOLE_EVENT,
    UI_EVENT
};

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

    struct BankEventHandlers
    {
        std::array<std::function<void(const std::string &, Bank, const std::string &, EventSource)>, EVENT_TYPE_COUNT> callbackArray;
        std::function<void(Bank, const std::string &)> unsubscribeCallback;
    };

    std::unordered_map<std::string, BankEventHandlers> lineBankCallbacks;
    std::unordered_map<std::string, BankEventHandlers> tapeBankCallbacks;
    std::unordered_map<std::string, BankEventHandlers> effectsBankCallbacks;
    std::unordered_map<std::string, BankEventHandlers> mastersBankCallbacks;

    // Pointer to the currently selected bank callbacks
    std::unordered_map<std::string, BankEventHandlers> *currentBankCallbacks;

    // Trying with the array for event type, these are only for updating UI, they only provide an event type, and the value:
    std::array<std::unordered_map<std::string, std::function<void(const std::string &)>>, EVENT_TYPE_COUNT> chStripComponentCallbacks;

    // This .... holds the callbacks for the MasterStripComponent
    std::array<std::function<void(const std::string&)>, EVENT_TYPE_COUNT> masterStripComponentCallback;

    Bank currentBank;

public:
    static EventBus &getInstance(); // Returns a reference to the instance.

    // ########################################## SUBSCRIPTION DECLARATIONS #########################################
    // Register/update event subscription for a particular strip, on a particular bank.
    // This subscribed by the channel class.
    void bankEventSubscribe(Bank bank,
                            // BankEventType eventType,
                            const std::string &channelStripID,
                            std::function<void(const std::string &, Bank, const std::string &, EventSource)> faderCallback,
                            std::function<void(const std::string &, Bank, const std::string &, EventSource)> vpotCallback,
                            std::function<void(const std::string &, Bank, const std::string &, EventSource)> buttonCallback,
                            std::function<void(Bank, const std::string &)> removeSubscriptionCallback);

    void chStripComponentSubscribe(const std::string stripID,
                                   const BankEventType eventType,
                                   std::function<void(const std::string &)> chStripCompCallback);

    void masterStripComponentSubscribe(const BankEventType eventType, std::function<void(const std::string &)> masterStripCompCallback);

    // ############################################# EVENT POST DECLARATIONS #############################################

    void postEvent(BankEventType eventType,
                   const std::string &channelStripID,
                   const std::string &eventValue,
                   EventSource source);

    // First, these are used by message handler, so these are physical events.
    //void lineBankEventPost(BankEventType eventType, const std::string &channelStripID, const std::string &eventValue);
    // void tapeBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue);
    // void effectsBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue);
    // void mastersBankChStripPost(EventType eventType, std::string ChannelStripID, std::string eventValue);

    // Second, these are used by the UI "Components". Technically, the UI don't "know" about which bank is selected...
    // so this probably the channel class should subscribe to these?
    // When a fader is moved in UI:
    // Send a dsp command.
    // Move all associated physcal faders (don't move the first one seperately).
    // Move all associated UI faders.

    // this should receive a set of associate channels, an event type, and the value. DO WE NEED BANK?
    // this might be purely "cosmetical" - just making the associated channel strips reflect the original.
    void associateChStripEventPost(std::unordered_set<std::string> channelStrips, BankEventType eventType, std::string eventValue);
    void associateMasterEventPost(BankEventType eventType, std::string eventValue);
    
    
    void lineBankChannelEventPost(std::unordered_set<std::string> channelStrips, BankEventType eventType, std::string eventValue);

    // This method is used to switch bank - by changing the pointer
    void setBankPoster(Bank bank);
};

// Singleton modifications
inline EventBus &EventBus::getInstance()
{
    static EventBus instance;
    return instance;
}

/////////////////////////////////////////////////////////////////////////////////////
// POSTER INTERFACE AND IMPLEMENTATIONS
