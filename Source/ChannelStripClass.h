/*
  ==============================================================================

    ChannelStripClass.h
    Created: 20 May 2024 11:14:09pm
    Author:  anders

  ==============================================================================
*/

#pragma once


#include "ChannelStripInterface.h"
#include "ChannelClass.h"
#include "SharedDataStructures.h"
#include "BrainComClass.h"
#include "LEDClass.h"

//static constexpr int CHANNEL_STRIP_COUNT = 24;
static constexpr int CH_STRIP_ID_LENGTH = 3;

// Array of channelstrip ID's, used for assigning the constant
// duing object construction.
const char CHSTRIP_ID_ARRAY[CHANNEL_STRIP_COUNT][CH_STRIP_ID_LENGTH] =
{
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", // strip 1-12
  "0C", "0D", "0E", "0F", "10", "11", "12", "13", "14", "15", "16", "17", // strip 13-24
};



class ChannelStrip : public ChannelStripInterface
{
  public:
    ChannelStrip(); // Constructor
    void setChannelAssociation(Bank activeBank, Bank associationBank, Channel *channelPtr) override;

    void updateChStrip(Bank currentBank) override;
    void updateVpotLeds(Bank bank, VpotFunction vPotFunc) override;

    // Method called by eventbus on fader action.
    inline void updateChannelVolume(Bank currentBank, const char (&faderValue)[2]) override
    {
       channelPtrs[currentBank]->updateVolume(faderValue);  // Call the channel object method for sending the DSP command.
    }

    // Method called by eventbus on vpot action.
    //inline void vPotEventHandler(Bank currentBank, const char (&vPotValue)[2], VpotFunction vPotFunc, EventSource source) override
    inline int vPotEventHandler(Bank currentBank, int vPotValue, VpotFunction vPotFunc, EventSource source) override
    {
        return channelPtrs[currentBank]->vPotEvent(vPotFunc, vPotValue, source);
    }

    inline int forwardVpotEvent(Bank currentBank, int vPotValue, VpotFunction vPotFunc, EventSource source)
    {
        return channelPtrs[currentBank]->vPotEvent(vPotFunc, vPotValue, source);
    }




    // TODO: when vpot functionality changes, channelstrips on current bank must be updated.
    // TODO: THis also goes for the UI.

    void updateFaderPosition(Bank currentBank);

    void setChannelPointer(Bank bank, Channel *channelPtr);

    // TODO: These are probably not both necessary.
    inline int getChannelStripIndex() { return CH_STRIP_INDEX; };
    inline ChStripID getChannelStripID() { return CH_STRIP_ID; };
    
    inline int getChannelStripVpotValue(Bank currentBank, VpotFunction currentVpotFunction) override 
    { 
        return channelPtrs[currentBank]->getVpotValue(currentVpotFunction); 
    }



  private:
    // Objects & Singletons
    Channel *channelPtrs[NUMBER_OF_BANKS] {nullptr};    // Initialize to nullptr.
    BrainCom &brainCom;
    LEDringLookup &ledRingLookup;

    // Attributes
    static int nextChannelStripNumber; // Static variable to keept track of next object's ID
    const int CH_STRIP_INDEX;
    const ChStripID CH_STRIP_ID;
    const char *const CH_STRIP_ID_STR;   // Pointer to this strip's ID.

    char faderMoveCmd[BRAIN_FADER_CMD_LENGTH];  // Care must be taken to avoid changing index 0,1 and 4 in this array!!

    // Keep two bitmaps of LED states - one for On/Off, another for blinking.
    // These must always reflect currently state, and will be compared
    // to the desired states, when changing bank.
    uint32_t currentLedOnStates = 0; // Initially all off.
    uint32_t currentLedBlinkStates = 0;

    static const int32_t RING_LED_MASK = 0x0FFF8;   // mask for selecting only the ring LED's, incl. the dot.


    void updateChStripLedsBitmap(Bank bank);
    void refreshStrip();    // Used for simply updating the LED's according to "currentStates", and updating fader position.
};