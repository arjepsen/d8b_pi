/*
  ==============================================================================

    ChannelStripClass.cpp
    Created: 20 May 2024 11:14:09pm
    Author:  anders

  ==============================================================================
*/

#include "ChannelStripClass.h"

constexpr int BRAIN_LED_CMD_LENGTH = 4;

// Set first strip ID. (Increments with every object constructed)
int ChannelStrip::nextChannelStripNumber = 0;

// Constructor
ChannelStrip::ChannelStrip() :
    CH_STRIP_ID_STR{CHSTRIP_ID_ARRAY[nextChannelStripNumber]},    // Set strip ID.
    CH_STRIP_INDEX{nextChannelStripNumber},
    CH_STRIP_ID{static_cast<ChStripID>(nextChannelStripNumber)},
    brainCom(BrainCom::getInstance()),
    ledRingLookup(LEDringLookup::getInstance())
{
    // Instatiation of channelstrips happens in MainComponent.
    // EventBus receives a pointer to the array.

    // Initialize the fader move BrainCommand template.
    faderMoveCmd[0] = CH_STRIP_ID_STR[0];
    faderMoveCmd[1] = CH_STRIP_ID_STR[1];
    faderMoveCmd[2] = '0';
    faderMoveCmd[3] = '0';
    faderMoveCmd[4] = 'f';

    // Increment for next object instantiation.
    nextChannelStripNumber++;
}


// This is the Callback for the eventbus, to be fired when 
// changing which channel the channelstrip should control.
// EVENTBUS MUST TRACK ASSOCIATION BETWEEN STRIPS. (ARRAY OF CHANNEL NUMBER PER STRIP PER BANK)
void ChannelStrip::setChannelAssociation(Bank activeBank, Bank associationBank, Channel *channelPtr)
{
    // If the new association is on the current bank, we need to update LED's
    // and fader position. (Also check if pointer exists..)
    if (associationBank == activeBank)
    {
        updateChStrip(associationBank);

    }

    // Update the channel pointer for the given bank.
    channelPtrs[associationBank] = channelPtr;
}


// This is for updating a strip according to it's channel. It is up to the caller to ensure that this is
// called on an active bank.
void ChannelStrip::updateChStrip(Bank bank)
{
    // THE CURRENT STATE IS SAVED IN THE OBJECT. COMPARE FOR CHECKING WHAT TO CHANGE
    uint32_t newLedOnStates = channelPtrs[bank]->getLedOnBitmap();
    uint32_t newLedBlinkStates = channelPtrs[bank]->getLedBlinkBitmap();

    // XOR the bitmaps with the new channel's bitmaps, to get a bitmap of changes.
    uint32_t changeOnBitmap = currentLedOnStates ^ newLedOnStates;
    uint32_t changeBlinkBitmap = currentLedBlinkStates ^ newLedBlinkStates;

    // Set up a char string for the brain LED commands
    char brainLedCommand[LED_COMMAND_LENGTH];

    // Iterate through the bitmaps and send brain messages to update LED's
    while(changeOnBitmap)
    {
        // Get the index of the lowest set bit. (corresponds to ChStripLed)
        int bitIndex = __builtin_ctz(changeOnBitmap);

        // Get bit value (0 or 1)
        uint32_t bitValue = (changeOnBitmap >> bitIndex) & 1;

        // Construct Brain command, and send it.
        brainLedCommand[0] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][bitIndex][0];
        brainLedCommand[1] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][bitIndex][1];
        brainLedCommand[2] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][bitIndex][2];
        brainLedCommand[3] = LED_OFF_CMD - ((newLedOnStates >> bitIndex) & 1);  // Set on/off command. off=106, on=105
        brainCom.send(brainLedCommand, LED_COMMAND_LENGTH);

        // Clear lowest set bit
        changeOnBitmap &= changeOnBitmap - 1;
    }

    // For the blink bitmap, all 1's means activate blink.
    while(changeBlinkBitmap)
    {
        int blinkBitIndex = __builtin_ctz(changeBlinkBitmap);
        brainLedCommand[0] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][blinkBitIndex][0];
        brainLedCommand[1] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][blinkBitIndex][1];
        brainLedCommand[2] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][blinkBitIndex][2];
        brainLedCommand[3] = LED_BLINK_CMD;
        brainCom.send(brainLedCommand, LED_COMMAND_LENGTH);
        changeBlinkBitmap &= changeBlinkBitmap - 1;
    }

    // Update the ChannelStrip's record of LED's
    currentLedOnStates = newLedOnStates;
    currentLedBlinkStates = newLedBlinkStates;

    // Now update the fader position.
    faderMoveCmd[2] = channelPtrs[bank]->getVolume()[0];
    faderMoveCmd[3] = channelPtrs[bank]->getVolume()[1];
    brainCom.send(faderMoveCmd, BRAIN_FADER_CMD_LENGTH);
}



void ChannelStrip::setChannelPointer(Bank bank, Channel *channelPtr)
{
    // TODO: It might be good to do some safety-error-checking here?
    // It probably shouldnt be TOO easy to change the pointer?

    channelPtrs[bank] = channelPtr;
}



void ChannelStrip::updateVpotLeds(Bank bank, VpotFunction vPotFunc)
{
    // TODO: not using vPotFunc?

    // ChannelStrip objects keeps bitmaps of its current LED On and Blink states.
    // Channel objects keeps bitmaps of "desired" states.

    // First, get the bitmap of desired LED states from the channel
    uint32_t desiredRingBits = channelPtrs[bank]->getLedOnBitmap();

    // Do an XOR with the bitmap of currently lit channelStrip LED's, to find differences.
    uint32_t diffMask = currentLedOnStates ^ desiredRingBits;

    // Limit the mask to only consider the ring LED's
    diffMask = diffMask & RING_LED_MASK;

    // Iterate over the mask and send commands.
    char brainLedCommand[BRAIN_LED_CMD_LENGTH];
    while (diffMask)
    {
        // Use lowest set bit to index the chars in the command array
        int charIndex = __builtin_ctz(diffMask);

        // Copy the ID parts of the LED command
        brainLedCommand[0] = CH_STRIP_LED_MAP[CH_STRIP_ID][charIndex][0];
        brainLedCommand[1] = CH_STRIP_LED_MAP[CH_STRIP_ID][charIndex][1];
        brainLedCommand[2] = CH_STRIP_LED_MAP[CH_STRIP_ID][charIndex][2];

        // The on/off command needs to be deduced from the desired bitmap
        brainLedCommand[3] = LED_OFF_CMD - ((desiredRingBits >> charIndex) & 1);  // Computes either 'j' or 'i'
    
        // Send the command
        brainCom.send(brainLedCommand, BRAIN_LED_CMD_LENGTH);

        // Update currentLedOnStates by toggling the bit at charIndex
        currentLedOnStates ^= (1 << charIndex); // Toggle the bit to reflect new state

        // Clear the processed bit from the difference mask
        diffMask &= ~(1 << charIndex);
    }
}


void ChannelStrip::updateFaderPosition(Bank currentBank)
{
    faderMoveCmd[2] = channelPtrs[currentBank]->getVolume()[0];
    faderMoveCmd[3] = channelPtrs[currentBank]->getVolume()[1];
    brainCom.send(faderMoveCmd, BRAIN_FADER_CMD_LENGTH);
}




void ChannelStrip::refreshStrip()
{


}


