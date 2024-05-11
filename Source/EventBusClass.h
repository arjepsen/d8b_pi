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
#include "SharedDataStructures.h"
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

using FaderCallback = std::function<void(const char (&)[2], Bank, ChStripID, EventSource)>;
using VpotCallback = std::function<void(const char (&)[2], Bank, ChStripID, EventSource)>;
using ButtonCallback = std::function<void(ButtonAction, Bank)>;
using UnSubscribeCallback = std::function<void(Bank, ChStripID)>;

// using MasterUiFaderCallback = std::function<void(const char (&)[2])>;
using MasterUiFaderCallback = std::function<void(int)>;

using AssociateUiFaderCallback = std::function<void(const char (&)[2])>;
using AssociateUiVpotCallback = std::function<void(const int)>;

class EventBus
{
  private:
    EventBus();  // Constructor
    ~EventBus(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    EventBus(const EventBus &) = delete;
    EventBus &operator=(const EventBus &) = delete;


    // Lets change to use arrays for char arrays instead of the maps of std::strings.
    // TODO: SHOULD THIS INCLUDE THE MASTER STRIP?

    // Declare arrays of fader and vpot callbacks. (+1 for master strip)
    FaderCallback faderCallbackArray[NUMBER_OF_BANKS][CH_STRIP_COUNT + 1] = {};
    VpotCallback vPotCallbackArray[NUMBER_OF_BANKS][CH_STRIP_COUNT + 1] = {};

    // Buttons are sligthly different - here we need to use a lookup map.
    // This includes all buttons, both channelstrips and master section.
    std::unordered_map<int, ButtonCallback> buttonCallbackMap[NUMBER_OF_BANKS];

    // Declare the UnSubscribe callback array
    UnSubscribeCallback unSubScribeCallbackArray[NUMBER_OF_BANKS][CH_STRIP_COUNT] = {};

    // Declare the vpot callback maps
    // std::unordered_map<std::string, ConsoleVpotCallbackFunction> vPotCallbackMap[NUMBER_OF_BANKS];

    // Declare the button callback maps

    // Declare array of associated ui fader callbacks.
    AssociateUiFaderCallback associateUiFaderCallbackArray[NUMBER_OF_BANKS][CH_STRIP_COUNT];
    AssociateUiVpotCallback associateUiVpotCallbackArray[NUMBER_OF_BANKS][CH_STRIP_COUNT];

    MasterUiFaderCallback masterUiFaderCallback;
    // std::function<void(const char (&)[2])> updateUiMasterFaderEventPost()

    // int channelStripButtonBaseLookup[CH_STRIP_COUNT];

    // Create a Bank variable for holding the currently selected bank.
    Bank currentBank;

    void initializeButtonCallbackMaps();
    void initializeButtonBaseLookup();

  public:
    static EventBus &getInstance(); // Returns a reference to the instance.

    const int channelStripButtonBase[CH_STRIP_COUNT];


    struct ChannelStripCallbacks
    {
        FaderCallback faderCallback;
        VpotCallback vPotCallback;
        ButtonCallback muteBtnCallback;
        ButtonCallback soloBtnCallback;
        ButtonCallback selectBtnCallback;
        ButtonCallback writeBtnCallback;
        ButtonCallback assignBtnCallback;
        ButtonCallback recRdyBtnCallback;
        UnSubscribeCallback unSubScribeCallback;
    };

    struct UiStripCallbacks
    {
        AssociateUiFaderCallback uiFaderCallback;
        AssociateUiVpotCallback uiVpotCallback;
        // TODO: buttons??
    };


    // Maybe seperate these into fader, vpot, button, etc...
    void associateUiStripFaderEventPost(int chStripBitMask, const char (&faderValue)[2]);
    void associateUiStripVpotEventPost(int chStripBitMask, int vPotValue);

    // void updateUiMasterFaderEventPost(const char (&faderValue)[2]);
    void updateUiMasterFaderEventPost(int faderValue);

    void masterUiFaderSubscribe(MasterUiFaderCallback masterUiFaderCallback);

    void channelStripComponentsubscribe(Bank bank, ChStripID stripID, UiStripCallbacks uiUpdateCallbacks);

    // void associateMasterEventPost(BankEventType eventType, std::string eventValue);

    // void lineBankChannelEventPost(std::unordered_set<std::string> channelStrips, BankEventType eventType, std::string eventValue);

    // // This method is used to switch bank - by changing the pointer
    void setCurrentBank(Bank bank);

    Bank getCurrentBank(); 

    inline void postFaderEvent(ChStripID channelStripID, char (&eventValue)[2], EventSource source)
    {
        faderCallbackArray[currentBank][channelStripID](eventValue, currentBank, channelStripID, source);
    }

    // TODO: Handle different pot functions - some - maybe most - are within
    // the channel class.
    inline void postVpotEvent(ChStripID channelStripID, char (&eventValue)[2], EventSource source)
    {
        vPotCallbackArray[currentBank][channelStripID](eventValue, currentBank, channelStripID, source);
    }

    inline void postButtonEvent(int buttonID, ButtonAction buttonAction)
    {
        buttonCallbackMap[currentBank][buttonID](buttonAction, currentBank);
    }


    void channelStripEventSubscribe(Bank bank, ChStripID chStripID, ChannelStripCallbacks &callbacks);

    void masterFaderEventSubscribe(FaderCallback masterFadercallback);

    // void channelStripEventSubscribe(Bank bank, const std::string &channelStripID,
    //                                 ConsoleFaderCallback faderCallback,
    //                                 ConsoleVpotCallbackFunction vpotCallback,
    //                                 std::function<void(Bank, const std::string &)> unsubscribeCallback);
};

// Singleton modifications
inline EventBus &EventBus::getInstance()
{
    static EventBus instance;
    return instance;
}

