/***********************************************************************
 * @file EventBusClass.h
 * @author Anders R. Jepsen
 * @brief The EventBus handles events fired from either UI or console.
 *        It is responsible for calling the methods that will handle
 *        the events.
 *        For example, when a fader is moved, the mixerManager will post 
 *        an event, and EventBus will make the necessary calls to the 
 *        channelstrip object that needs to handle the event.
 * 
 *        EventBus is made a singleton. Not strictly necessary if care
 *        is taken to not instantiate more than one object, but it was
 *        done to learn more about this way of coding.
 * 
 *        One interesting thing is that any channelstrip on any bank
 *        can be made to control any of the 96 DSP channels.
 *        In order to keep track of which strip controls which channel, 
 *        eventBus keeps a 2D array of bitmaps - 96 bitmaps for each bank.
 *        So for each of the 96 channels we use these bitmaps to track 
 *        which channelstrips are currently set up to control it.
 *        This is what is meant by "ChannelAssociation".
 * 
 *        TODO: EXPLAIN BUTTON WORKINGS.
 * 
 * @version 0.1
 * @date 2024-07-26
 ***********************************************************************/

#pragma once

#include <array>
#include <functional>
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

// Create aliases for making shorter lines later.
using ButtonCallback = std::function<void(ButtonAction, Bank)>;
using FaderEventHandler = void (ChannelStripInterface::*)(Bank, const char (&)[2]);


// MAY '25: Trying to move to function pointers:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
struct VpotDispatchEntry 
{
    int (*handler)(void* context, Bank currentBank, int vpotValue, VpotFunction function, EventSource source);
    void* context;
};

int handleChannelStripVpot(void* context, Bank currentBank, int value, VpotFunction func, EventSource src)
{
    ChannelStrip* strip = static_cast<ChannelStrip*>(context);
    return strip->channelPtrs[currentBank]->vPotEvent(func, value, src);
}




class EventBus
{
  private:
    EventBus();  // Constructor
    ~EventBus(); // Destructor

    // Delete copy constructor and assignment operator, to avoid copying the singleton.
    EventBus(const EventBus &) = delete;
    EventBus &operator=(const EventBus &) = delete;

    // TODO:
    // // Buttons are sligthly different - here we need to use a lookup map.
    // // This includes all buttons, both channelstrips and master section.
    std::unordered_map<int, ButtonCallback> buttonCallbackMap[NUMBER_OF_BANKS];

    // Declare a 2D array of channel association bitmaps.
    uint32_t channelAssociationBitmaps[CHANNEL_COUNT][NUMBER_OF_BANKS];

    // Make a pointer to the array of UI ChannelStripComponents
    ChannelStripComponentInterface *channelStripComponentArray[CHANNEL_STRIP_COUNT + 1];    // incl. master

    // Same for ChannelStrips and Channels
    Channel *channelArray;

    // Array of pointers to all channelstrips (+1 = incl. master)
    ChannelStripInterface* channelStripArray[CHANNEL_STRIP_COUNT + 1];


    // MAY '25: exchanging the interface array with function pointers!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    VpotDispatchEntry vpotDispatch[0x20] = {};  // vpot ID's go from 0x00 to 0x20 // TODO: should maybe enum it....


    // Create a Bank variable for holding the currently selected bank.
    Bank currentBank;

    // Keep a variable for current vpot functionality.
    VpotFunction currentVpotFunction = VPOT_PAN;

    // Keep a pointer to the currently selected channelstrip
    ChannelStrip *selectedChStrip;


    void initializeButtonCallbackMaps();
    //void initializeButtonBaseLookup();

    void initializeChannels();
    void initializeChannelStrips();
    void initializeUiStrips();


  public:
    static EventBus &getInstance(); // Returns a reference to the instance.

    const int channelStripButtonBase[CHANNEL_STRIP_COUNT];

    //Bank getCurrentBank(); 

    void postFaderEvent(ChStripID channelStripID, char (&eventValue)[2], EventSource source);
    void postVpotEvent(ChStripID channelStripID, int vPotChangeValue, EventSource source);

    inline void postButtonEvent(int buttonID, ButtonAction buttonAction)
    {
        printf("calling button callback - now in eventbus.h\n");
        //buttonCallbackMap[currentBank][buttonID](buttonAction, currentBank);
    }

    void channelStripEventSubscribe(int chArrayIndex, ChStripID channelStripID, Bank bank);
    void setChannelStripComponentArray(ChannelStripComponent * chStripCompArray, MasterStripComponent * masterComponentPtr);
    void setChannelStripArray(ChannelStrip * chStripArray, MasterChannel * masterChannelPtr);
    void setChannelArray(Channel * chArray);
    void setSelectedChStrip(ChStripID chStrip);


    // Use this for enable UI controls, after the console is booted.
    void enableUiListeners();

    // These methods are for saving and reading all settings to and from file.
    void saveSettings();
    void loadSettings();

    void changeBank(Bank newBank);

};

// Singleton modifications
inline EventBus &EventBus::getInstance()
{
    static EventBus instance;
    return instance;
}


