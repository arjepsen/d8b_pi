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
#include "ChannelClass.h"
#include "ChannelStripClass.h"
#include "ChannelStripComponent.h"
#include "ChannelStripInterface.h"
#include "ChannelStripComponentInterface.h"
#include "MasterChannelClass.h"
#include "MasterStripComponent.h"

// !!!!!! DO NOT CHANGE ORDER !!!!!!! (used for indexing array)
// TODO: Are we still using these?
// enum BankEventType : int
// {
//     FADER_EVENT,
//     VPOT_EVENT,
//     BUTTON_EVENT,
//     EVENT_TYPE_COUNT
// };



enum EventSource
{
    CONSOLE_EVENT,
    UI_EVENT
};



// using FaderCallback = std::function<void(const char (&)[2], Bank, ChStripID, EventSource)>;
// using VpotCallback = std::function<void(const char (&)[2], Bank, ChStripID, EventSource)>;
using ButtonCallback = std::function<void(ButtonAction, Bank)>;
// using UnSubscribeCallback = std::function<void(Bank, ChStripID)>;

// using MasterUiFaderCallback = std::function<void(const char (&)[2])>;
// using MasterUiFaderCallback = std::function<void(int)>;

// using AssociateUiFaderCallback = std::function<void(const char (&)[2])>;
// using AssociateUiVpotCallback = std::function<void(const int)>;

using FaderEventHandler = void (ChannelStripInterface::*)(Bank, const char (&)[2]);


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

    // // Declare arrays of fader and vpot callbacks. (+1 for master strip)
    // FaderCallback faderCallbackArray[NUMBER_OF_BANKS][CH_STRIP_COUNT + 1] = {};
    // VpotCallback vPotCallbackArray[NUMBER_OF_BANKS][CH_STRIP_COUNT + 1] = {};

    // // Buttons are sligthly different - here we need to use a lookup map.
    // // This includes all buttons, both channelstrips and master section.
    std::unordered_map<int, ButtonCallback> buttonCallbackMap[NUMBER_OF_BANKS];

    // Declare the UnSubscribe callback array
    //UnSubscribeCallback unSubScribeCallbackArray[NUMBER_OF_BANKS][CH_STRIP_COUNT] = {};

    // Declare the vpot callback maps
    // std::unordered_map<std::string, ConsoleVpotCallbackFunction> vPotCallbackMap[NUMBER_OF_BANKS];

    // Declare the button callback maps

    // Declare a 2D array of channel association bitmaps.
    uint32_t channelAssociationBitmaps[CHANNEL_COUNT][NUMBER_OF_BANKS];


    // // Declare array of associated ui fader callbacks.
    // AssociateUiFaderCallback associateUiFaderCallbackArray[NUMBER_OF_BANKS][CH_STRIP_COUNT];
    // AssociateUiVpotCallback associateUiVpotCallbackArray[NUMBER_OF_BANKS][CH_STRIP_COUNT];

    // MasterUiFaderCallback masterUiFaderCallback;
    // std::function<void(const char (&)[2])> updateUiMasterFaderEventPost()

    // int channelStripButtonBaseLookup[CH_STRIP_COUNT];

    // Create a Bank variable for holding the currently selected bank.
    Bank currentBank;

    // Make a pointer to the array of UI ChannelStripComponents
    ChannelStripComponentInterface *channelStripComponentArray[CHANNEL_STRIP_COUNT + 1];

    // Same for ChannelStrips and Channels
    //ChannelStrip *channelStripArray;
    Channel *channelArray;

    // Array of pointers to all channelstrips (incl. master)
    ChannelStripInterface* channelStripArray[CHANNEL_STRIP_COUNT + 1];


    //FaderEventHandler faderEventHandlers[CHANNEL_STRIP_COUNT + 1];  // Let's try with function pointers.

    // Create arrays of Channels and Channelstrips. This instantiates the objects.
    // Channel channelArray[CHANNEL_COUNT]; 
    // ChannelStrip channelStripArray[CHANNEL_STRIP_COUNT];

    void initializeButtonCallbackMaps();
    void initializeButtonBaseLookup();

  public:
    static EventBus &getInstance(); // Returns a reference to the instance.

    const int channelStripButtonBase[CHANNEL_STRIP_COUNT];


    // // TODO: Maybe move this to the channelstrip class?
    // struct ChannelStripCallbacks
    // {
    //     FaderCallback faderCallback;
    //     VpotCallback vPotCallback;
    //     ButtonCallback muteBtnCallback;
    //     ButtonCallback soloBtnCallback;
    //     ButtonCallback selectBtnCallback;
    //     ButtonCallback writeBtnCallback;
    //     ButtonCallback assignBtnCallback;
    //     ButtonCallback recRdyBtnCallback;
    //     UnSubscribeCallback unSubScribeCallback;
    // };

    // struct UiStripCallbacks
    // {
    //     AssociateUiFaderCallback uiFaderCallback;
    //     AssociateUiVpotCallback uiVpotCallback;
    //     // TODO: buttons??
    // };


    // // Maybe seperate these into fader, vpot, button, etc...
    // void associateUiStripFaderEventPost(int chStripBitMask, const char (&faderValue)[2]);
    // void associateUiStripVpotEventPost(int chStripBitMask, int vPotValue);

    // void updateUiMasterFaderEventPost(const char (&faderValue)[2]);
    //void updateUiMasterFaderEventPost(int faderValue);

    //void masterUiFaderSubscribe(MasterUiFaderCallback masterUiFaderCallback);

    //void channelStripComponentsubscribe(Bank bank, ChStripID stripID, UiStripCallbacks uiUpdateCallbacks);

    // void associateMasterEventPost(BankEventType eventType, std::string eventValue);

    // void lineBankChannelEventPost(std::unordered_set<std::string> channelStrips, BankEventType eventType, std::string eventValue);

    // // This method is used to switch bank - by changing the pointer
    void setCurrentBank(Bank bank);

    Bank getCurrentBank(); 

    void postFaderEvent(ChStripID channelStripID, char (&eventValue)[2], EventSource source);

    


    // TODO: Handle different pot functions - some - maybe most - are within
    // the channel class.
    inline void postVpotEvent(ChStripID channelStripID, char (&eventValue)[2], EventSource source)
    {
        printf("vpot event\n");
        //vPotCallbackArray[currentBank][channelStripID](eventValue, currentBank, channelStripID, source);
    }

    inline void postButtonEvent(int buttonID, ButtonAction buttonAction)
    {
        printf("calling button callback - now in eventbus.h\n");
        //buttonCallbackMap[currentBank][buttonID](buttonAction, currentBank);
    }


    // void channelStripEventSubscribe(Bank bank, ChStripID chStripID, ChannelStripCallbacks &callbacks);

    // void masterFaderEventSubscribe(FaderCallback masterFadercallback);

    // void buttonEventSubscribe(int buttonID, Bank bank, ButtonCallback buttonCallback);


    void channelStripEventSubscribe(int chArrayIndex, ChStripID channelStripID, Bank bank);


    void initializeChannels();
    void initializeChannelStrips();
    void initializeUiStrips();

    void setChannelStripComponentArray(ChannelStripComponent * chStripCompArray, MasterStripComponent * masterComponentPtr);
    //void setChannelStripArray(ChannelStrip * chStripArray);
    void setChannelStripArray(ChannelStrip * chStripArray, MasterChannel * masterChannelPtr);
    void setChannelArray(Channel * chArray);

    // set array of fader-event methods.
    //void setFaderEventHandler(FaderEventHandler handler, ChStripID chStripID);

    // Use this for enable UI controls, after the console is booted.
    void enableUiListeners();



};

// Singleton modifications
inline EventBus &EventBus::getInstance()
{
    static EventBus instance;
    return instance;
}


