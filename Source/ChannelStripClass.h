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
//#include "EventBusClass.h"
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
    //void initializeChStrip();


    //void updateChannelVolume(Bank currentBank, const char (&faderValue)[2]) override;
    inline void updateChannelVolume(Bank currentBank, const char (&faderValue)[2])
    {
       channelPtrs[currentBank]->updateVolume(faderValue);  // Call the channel object method for sending the DSP command.
    }


    void updateFaderPosition(Bank currentBank);

    void setChannelPointer(Bank bank, Channel *channelPtr);

    // TODO: These are probably not both necessary.
    inline int getChannelStripIndex() { return CH_STRIP_INDEX; };
    inline ChStripID getChannelStripID() { return CH_STRIP_ID; };

    void test(Bank bank) {channelPtrs[bank]->test(); };

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


    // Make an array for the state of each channelStrip LED state.
    // Initialize all to off ("j") in the constructor.
    //LedStateCommand chStripLeds[CH_STRIP_LED_COUNT];

    // Keep two bitmaps of LED states - one for On/Off, another for blinking.
    // These must always reflect currently state, and will be compared
    // to the desired states, when changing bank.
    uint32_t currentLedOnStates = 0; // Initially all off.
    uint32_t currentLedBlinkStates = 0;

    // Keep arrays of led states as the registry of how the leds SHOULD be
    // lit for this channelstrip, on any given bank.
    // uint32_t desiredLedOnStates[NUMBER_OF_BANKS] {0};
    // uint32_t desiredLedBlinkStates[NUMBER_OF_BANKS] {0};

    // Do we then need this one for just the red/green?
    // uint32_t redGreenChLedState[NUMBER_OF_BANKS] {0};
    


    // Methods

    //void faderMoveCallback(Bank currentBank, const char (&faderValue)[2], EventSource source);


    void updateVpotLeds(Bank bank);
    void updateChStripLedsBitmap(Bank bank);
    void refreshStrip();    // Used for simply updating the LED's according to "currentStates", and updating fader position.
};