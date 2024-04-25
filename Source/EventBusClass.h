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
// #include <memory> // For unique_ptr
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
    MASTERS_BANK,
    NUMBER_OF_BANKS
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

    // Structure to hold callbacks, the first element is for the channelobject, the second is for removing a
    // strip from the channel object's association map.
    // The first element is an array which holds the callbacks for the channel:
    // [faderCallback, VpotCallback, ButtonCallback]
    // The second element is the unsubscribe callback for the channel.
    // The unsubscription callback in the channelclass removes the recorded associated strips
    struct BankEventHandlers
    {
        std::array<std::function<void(const std::string &, Bank, const std::string &, EventSource)>, EVENT_TYPE_COUNT> callbackArray;
        std::function<void(Bank, const std::string &)> unsubscribeCallback;
    };

    // We instantiate four maps of BankEventHandlers - meaning we will have a map of the structs for each bank.
    std::unordered_map<std::string, BankEventHandlers> lineBankCallbacks;
    std::unordered_map<std::string, BankEventHandlers> tapeBankCallbacks;
    std::unordered_map<std::string, BankEventHandlers> effectsBankCallbacks;
    std::unordered_map<std::string, BankEventHandlers> mastersBankCallbacks;

    // Pointer to the currently selected bank callbacks
    std::unordered_map<std::string, BankEventHandlers> *currentBankCallbacks;

    // This is used for UI callbacks. It is an array of maps, indexed as [FADER_EVENT, VPOT_EVENT, BUTTON_EVENT].
    // Each array element contains a map of <ChannelStripID, callBackFunction(valueString)>
    // So these callbacks are only for updating the UI after a physical interaction has happened.
    std::array<std::unordered_map<std::string, std::function<void(const std::string &)>>, EVENT_TYPE_COUNT> chStripComponentCallbacks;

    // This .... holds the callbacks for the MasterStripComponent
    std::array<std::function<void(const std::string &)>, EVENT_TYPE_COUNT> masterStripComponentCallback;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////// april '24 --- trying to seperate faders, vpots and buttons, due to master section.... ////////////
    // first, we do need unsubscriptions for channelstrips.... and the unsubscribtions are for all three controls....

    // Set up a definition of CallbackFunction, to shorten the lines.
    using FaderCallbackFunction = std::function<void(const int,  Bank, const std::string &, EventSource)>;
    using VpotCallbackFunction = std::function<void(const std::string &, Bank, const std::string &, EventSource)>;

    // We set up the maps using arrays of maps. We index the arrays uing the bank enumeration.

    // Declare the fader callback maps
    std::unordered_map<std::string, FaderCallbackFunction> faderCallbackMap[NUMBER_OF_BANKS];

	FaderCallbackFunction faderCallbackArray[NUMBER_OF_BANKS][number of channelstrips.]


    // Declare the vpot callback maps
    std::unordered_map<std::string, VpotCallbackFunction> vPotCallbackMap[NUMBER_OF_BANKS];

    // Declare the button callback maps

    // Declare the channelStrip Unsubscribe maps
    std::unordered_map<std::string, std::function<void(Bank, const std::string &)>> unsubscribeCallbackMap[NUMBER_OF_BANKS];

    // The Master section - according to the original d8b manual - is everything to the right of channel 24.
    // Let's create a data structure for the callbacks for these buttons and vpots.
    // No un-subscription - they cant be moved.
    // Maybe seperate maps for vpots and buttons. (Should we handle all buttons here??)
    // std::unordered_map<std::string, std::function<void(const std::string&)>> masterSectionVpotCallbacks;
    // std::unordered_map<std::string, std::function<void(const std::string&)>> masterSectionButtonCallbacks;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Create a Bank variable for holding the currently selected bank.
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

    // This is used by the ChannelStripComponent to subscribe to channel strip events.
    // So these callbacks are only for updating the ui.
    void chStripComponentSubscribe(const std::string stripID,
                                   const BankEventType eventType,
                                   std::function<void(const std::string &)> chStripCompCallback);

    // Similar - this is only used for ui updates.
    void masterStripComponentSubscribe(const BankEventType eventType, std::function<void(const std::string &)> masterStripCompCallback);

    // ############################################# EVENT POST DECLARATIONS #############################################

    void postEvent(BankEventType eventType,
                   const std::string &channelStripID,
                   const std::string &eventValue,
                   EventSource source);

    ////////// April -24 - trying to seperate things a bit, since master section messes things up.
    // POSTING:
    // In mixerManager, we determine if it's a fader, vpot of button.
    // If fader, make a consoleFaderEventPost(channelStripID, value);
    // This should check up agains
    // if vpot, make a consoleVpotEventPost(vpotID, value)

    // First, these are used by message handler, so these are physical events.
    // void lineBankEventPost(BankEventType eventType, const std::string &channelStripID, const std::string &eventValue);
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
    void associateChStripUiEventPost(std::unordered_set<std::string> channelStrips, BankEventType eventType, std::string eventValue);
    void associateMasterEventPost(BankEventType eventType, std::string eventValue);

    void lineBankChannelEventPost(std::unordered_set<std::string> channelStrips, BankEventType eventType, std::string eventValue);

    // This method is used to switch bank - by changing the pointer
    void setBankPoster(Bank bank);

    Bank getCurrentBank(); // Returns the enumeration for the currently selected bank.
                           // void postFaderEvent(const std::string &channelStripID, const std::string &eventValue, EventSource source);
    void postFaderEvent(const int channelStripID, const char &eventValue, EventSource source);

    void channelStripEventSubscribe(Bank bank, const std::string &channelStripID,
                                    FaderCallbackFunction faderCallback,
                                    VpotCallbackFunction vpotCallback,
                                    std::function<void(Bank, const std::string &)> unsubscribeCallback);
};

// Singleton modifications
inline EventBus &EventBus::getInstance()
{
    static EventBus instance;
    return instance;
}

/////////////////////////////////////////////////////////////////////////////////////
// POSTER INTERFACE AND IMPLEMENTATIONS

class VpotCallbackInterface
{
  public:
    virtual void vpotCallback(std::string, std::string, EventSource) = 0;
    virtual ~VpotCallbackInterface() {}
};

// Implementation for channel strip V-Pot callbacks
class ChannelStripVpotCallbacks : public VpotCallbackInterface
{
  private:
    // value, bank, id, source
    std::function<void(std::string, Bank, std::string, EventSource)> callback[NUMBER_OF_BANKS];
    EventBus &eventBus;

  public:
    void vpotCallback(std::string vpotValue, std::string vpotID, EventSource source) override
    {
        Bank bank = eventBus.getCurrentBank();
        callback[bank](vpotValue, bank, vpotID, source);
    }

    void setCallback(std::function<void(std::string, Bank, std::string, EventSource)>);
};

class MasterSectionVpotCallbacks : public VpotCallbackInterface
{
    // Implementation...
};

// value, bank, id, source
// std::string, Bank, std::string, EventSource
// std::array<std::function<void(const std::string &, Bank, const std::string &, EventSource)>, EVENT_TYPE_COUNT> callbackArray;
