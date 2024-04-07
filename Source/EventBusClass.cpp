/*
  ==============================================================================

    EventBusClass.cpp
    Created: 11 Jun 2023 8:59:37pm
    Author:  anders

  ==============================================================================
*/

#include "EventBusClass.h"
#include <stdexcept>

// UNCOMMENT TO ENABLE DEBUG MESSAGES.
#define EVENTBUS_DEBUG_MESSAGES

#ifdef EVENTBUS_DEBUG_MESSAGES
#define DEBUG_MSG(format, ...) printf("EVENTBUS_DBG: " format, ##__VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) ((void)0) // do {} while (0)
#endif


/**
 * Constructor for the event bus.
 * TODO: give it a thorough description
*/
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

    // Set the initial bank to Line Bank
    // currentBank = LINE_BANK;
    // currentBankCallbacks = &lineBankCallbacks;
    setBankPoster(LINE_BANK);
}

// Destructor
EventBus::~EventBus() {}


/*****************************************************************************************************************************
 * @brief This method is used to subscribe to events on a given channelstrip on a given bank.
 *      The method must receive a specification of which bank and channelstrip, and 
 *      the callback methods for fader- vpot- and button activity on that strip.
 *      It must also receive a callback for removing the subscription. This is necessary, since a channel can be configured
 *      to any strip on any bank.
 * 
 * @param bank                          Specify which bank.
 * @param channelStripID                Specify which channelstrip on the given bank
 * @param faderCallback                 Hand over the callback to call, when a fader is moved on the specified strip.
 * @param vpotCallback                  Callback for vpot movements on the strip.
 * @param buttonCallback                Callback for button presses on the strip.
 * @param removeSubscriptionCallback    Callback for removing the current subscription.
 *****************************************************************************************************************************/
void EventBus::bankEventSubscribe(Bank bank,
                                  const std::string &channelStripID,
                                  std::function<void(const std::string &, Bank, const std::string &, EventSource)> faderCallback,
                                  std::function<void(const std::string &, Bank, const std::string &, EventSource)> vpotCallback,
                                  std::function<void(const std::string &, Bank, const std::string &, EventSource)> buttonCallback,
                                  std::function<void(Bank, const std::string &)> removeSubscriptionCallback)
{
    if (channelStripID.length() != 2)
    {
        DEBUG_MSG("WRONG LENGTH OF CHANNEL STRIP ID\n");
        exit(1);
    }

    // Set a reference to the specific bank map of callbacks.
    auto &callbackMap =
        (bank == LINE_BANK)      ? lineBankCallbacks
        : (bank == TAPE_BANK)    ? tapeBankCallbacks
        : (bank == EFFECTS_BANK) ? effectsBankCallbacks
        : (bank == MASTERS_BANK) ? mastersBankCallbacks
                                 : (throw std::runtime_error("ERROR IN DETERMINING BANK FOR EVENT CALLBACK\n"));

    // Check if a subscription already exists, if so, run its unsubscribe callback.
    if (callbackMap.find(channelStripID) != callbackMap.end())
    {
        callbackMap[channelStripID].unsubscribeCallback(bank, channelStripID);
    }

    // Write in the new callbacks.
    callbackMap[channelStripID].callbackArray[FADER_EVENT] = faderCallback;
    callbackMap[channelStripID].callbackArray[VPOT_EVENT] = vpotCallback;
    callbackMap[channelStripID].callbackArray[BUTTON_EVENT] = buttonCallback;
    callbackMap[channelStripID].unsubscribeCallback = removeSubscriptionCallback;
}



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

// #######################################################################################################################
// This method is fired when a master strip event has happened, and we need to update the other (console or ui) interface.
// #######################################################################################################################
void EventBus::associateMasterEventPost(BankEventType eventType, std::string eventValue)
{
    masterStripComponentCallback[eventType](eventValue);
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

// #####################################################################
// This method is used to add the masterChannelStripComponent callbacks.
// #####################################################################
void EventBus::masterStripComponentSubscribe(const BankEventType eventType,
                                   std::function<void(const std::string &)> masterStripCompCallback)
{
    // Set the callback
    masterStripComponentCallback[eventType] = masterStripCompCallback;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ##############################################################################################
// This is the event post method that other classes will use.
// It is essentially just a pointer to one of the four internal classes, corresponding to a bank.
// ##############################################################################################
void EventBus::postEvent(BankEventType eventType,
                         const std::string &channelStripID,
                         const std::string &eventValue,
                         EventSource source)
{
    currentBankCallbacks->at(channelStripID).callbackArray[eventType](eventValue, currentBank, channelStripID, source);
}

// ##################################################################################
// This method is used to change poster method, depending on which bank is selected.
// ##################################################################################
void EventBus::setBankPoster(Bank bank)
{
    // Set the currentBank member
    currentBank = bank;

    // Update the callback map pointer.
    switch (currentBank)
    {
        case LINE_BANK:
            currentBankCallbacks = &lineBankCallbacks;
            break;
        case TAPE_BANK:
            currentBankCallbacks = &tapeBankCallbacks;
            break;
        case EFFECTS_BANK:
            currentBankCallbacks = &effectsBankCallbacks;
            break;
        case MASTERS_BANK:
            currentBankCallbacks = &mastersBankCallbacks;
            break;
        default:;
            // Handle invalid bank input.
    }
}
