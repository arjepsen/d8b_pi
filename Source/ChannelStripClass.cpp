/*
  ==============================================================================

    ChannelStripClass.cpp
    Created: 20 May 2024 11:14:09pm
    Author:  anders

  ==============================================================================
*/

#include "ChannelStripClass.h"

//constexpr int BRAIN_FADER_CMD_LENGTH = 5; // Max brain fader command length. i.e. "22ABf" excluding the null terminator


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
    // First, retreive the bitmaps for the LED's for the current channel
    // uint32_t currentOnLedBitmap = channelPtrs[activeBank]->getLedOnBitmap();
    // uint32_t currentBlinkLedBitmap = channelPtrs[activeBank]->getLedBlinkBitmap();

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

        // Add the value to get the ON or OFF command for the LED
        LedStateCommand stateCmd = static_cast<LedStateCommand>(LED_OFF_CMD - bitValue);    // off=106, on=105

        // Construct Brain command, and send it.
        brainLedCommand[0] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][bitIndex][0];
        brainLedCommand[1] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][bitIndex][1];
        brainLedCommand[2] = CH_STRIP_LED_MAP[CH_STRIP_INDEX][bitIndex][2];
        brainLedCommand[3] = stateCmd;
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
    // Create a fader command string from the new channel's volume string,
    // and send it.
    //char brainFaderCommand[BRAIN_FADER_CMD_LENGTH]; 
    //brainFaderCommand[0] = CH_STRIP_ID_STR[0];
    //brainFaderCommand[1] = CH_STRIP_ID_STR[1];
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


// void ChannelStrip::faderMoveCallback(Bank currentBank, const char (&faderValue)[2], EventSource source)
// {
//     // First, call our associated channel, to let it send DSP commands
//     channelPtrs[currentBank]->updateVolume(faderValue);

//     NEXT - how to handle the fader moves?
//     maybe a seperate method? Then eventbus could iterate over them all without
//     having to remove the caller first....
//     NO - 'cause the caller is already moved!!!'

//     OPTIONS:
//     1:
//     Channel object keeps track of associated channelstrips on the different banks.
//     Then, calling upateVolume could return the association, and THIS strip could
//     send brain commands for all associated strips on the current bank.

//     2:
//     Like 1, except now the strip hands the association back to eventbus, 
//     which iterates over it and sends association commands to all associated strips.

//     3:
//     Eventbus keeps track of association. It should then be able to iterate over it
//     and send commands.
//     This avoids the whole back-and-forth, and it also avoids unsubscription handling.

// }


// void ChannelStrip::updateChannelVolume(Bank currentBank, const char (&faderValue)[2])
// {
//     // Call the channel method for sending the DSP command.
//     channelPtrs[currentBank]->updateVolume(faderValue);
// }

void ChannelStrip::updateFaderPosition(Bank currentBank)
{
    // char brainFaderCommand[BRAIN_FADER_CMD_LENGTH]; 
    // brainFaderCommand[0] = CH_STRIP_ID_STR[0];
    // brainFaderCommand[1] = CH_STRIP_ID_STR[1];
    // brainFaderCommand[2] = channelPtrs[currentBank]->getVolume()[0];
    // brainFaderCommand[3] = channelPtrs[currentBank]->getVolume()[1];
    // brainFaderCommand[4] = 'f';
    // brainCom.send(brainFaderCommand, BRAIN_FADER_CMD_LENGTH);
    faderMoveCmd[2] = channelPtrs[currentBank]->getVolume()[0];
    faderMoveCmd[3] = channelPtrs[currentBank]->getVolume()[1];
    brainCom.send(faderMoveCmd, BRAIN_FADER_CMD_LENGTH);
}




void ChannelStrip::refreshStrip()
{


}







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////7
////////////////////////////////////////////////////////////////////////////
//////////////////// UNUSED OLD STUFF //////////////////////////////////////

// void ChannelStrip::updateChStrip(Bank bank)
// {
//     // TODO: Maybe seperate this into fader, vpot, btnleds?

//     // Fetch info from the associated channel object on the given bank.
//     // Then update fader position, Vpot LED's, and button LED's

//     // Fetch pointer to volume string from the channel.
    
//     // ================ Update Fader ==========================
//     const char * volumeString = channelPtrs[bank]->getVolume();
//     char brainFaderCommand[] = "----f";
//     brainFaderCommand[0] = CHSTRIP_ID_STR[0];
//     brainFaderCommand[1] = CHSTRIP_ID_STR[1];
//     brainFaderCommand[2] = volumeString[0];
//     brainFaderCommand[3] = volumeString[1];
//     brainCom.send(brainFaderCommand, 5);

//     // =============== Update Pan LED's ======================
//     // Since we don't know what might have been on, we might need to turn
//     // all LED's off to start with.

//     // Actually, Channelstrip is completely oblivious to current active 
//     // vpot functionality.... It must retreive a simple value from
//     // the Channel object, and use that to deduce led activity.
//     // Should this value be the 2-char value, or an int?


//     int vPotValue = channelPtrs[bank]->getCurrentVpotValue;
//     char brainLedCommand[4];
//     MAYBE SEND ONE LONG STRING?
//     // brainLedCommand[0] = CH_STRIP_LED_MAP[CH_NUMBER][RING_DOT][0];
//     // brainLedCommand[1] = CH_STRIP_LED_MAP[CH_NUMBER][RING_DOT][1];
//     // brainLedCommand[2] = CH_STRIP_LED_MAP[CH_NUMBER][RING_DOT][2];
//     // brainLedCommand[3] = LED_ON_CMD;
//     // brainCom.send(brainLedCommand, BRAIN_LED_CMD_LENGTH);

//     // First, retrieve the pan value.

//     for 



// }

// void ChannelStrip::updateVpotLeds(Bank bank)
// {
//     // First, we need to get the current value of the active vpot function
//     // from the channel. This must be a value between 0 and 255,
//     // regardless of active function.
//     int vPotValue = channelPtrs[bank]->getCurrentVpotValue();

//     // Use the lookup to find which ring LED should be on for the current 
//     // vPot value, and update the array.
//     ChStripLED activeRingLED = ledRingLookup.getRingID(vPotValue);



//     // TODO: Are there any case where we need more than one ring led on??


//     // Compare to the array of currently lit LED's
//     // update as necessary....


// }


// // We already set up an array of LED enumeration ID's.
// // Lets create a temporary comparitive array, that we update with
// // which led's SHOULD be on, and then compare the two, and send commands.
// void ChannelStrip::updateChStripLedsBitmap(Bank bank)
// {
//     // First, set up bitmaps of desired led states for the channelstrip
//     // The first indicates on/off state for all channelstrip leds.
//     // The second only indicates whether a led is blinking.
//     desiredLedOnStates[bank] = 0;
//     desiredLedBlinkStates[bank] = 0;

//     // Retreive the boolean state of various channel attributes, and
//     // use them to set the bits for the button LED's
//     // TODO: WHICH OF THESE MIGHT BE BLINKING? RECRDY ONLY?
//     desiredLedOnStates[bank] |= (channelPtrs[bank]->getMuteState() << MUTE_LED);
//     desiredLedOnStates[bank] |= (channelPtrs[bank]->getSoloState() << SOLO_LED);
//     desiredLedOnStates[bank] |= (channelPtrs[bank]->getSelectState() << SELECT_LED);
//     // TODO: remaining button led's

//     // Now handle the Ring LED's. First, retreive the current vPot value for
//     // the currently selected vPot function. (Must be between 0 and 255).
//     // Convert to ring led enumeration, and set the bit.
//     int vPotValue = channelPtrs[bank]->getCurrentVpotValue();
//     ChStripLED activeRingLED = ledRingLookup.getRingID(vPotValue);
//     desiredLedOnStates[bank] |= (1 << activeRingLED);

//     // Set the bit for the Vpot center dot.
//     desiredLedOnStates[bank] |= (channelPtrs[bank]->getVpotDotState() << RING_DOT);

//     // Get the channel number, for setting the channel LED's (red/green).
//     int chNumber = channelPtrs[bank]->getChannelNumber();

//     // From the channel number, deduce the desired state of the red/green leds.
//     // (channel numbering starts from 0).
//     ChannelLeds chLeds = CH_LEDS_NONE;  
//     if (chNumber < 24)
//         chLeds = CH_LEDS_GREEN;
//     else if (chNumber < 48)
//         chLeds = CH_LEDS_RED;
//     else if (chNumber < 72)
//         chLeds = CH_LEDS_BOTH;

//     // Create a bitmask from the state, by shifting the enum.
//     uint32_t newChLedsBitmap = static_cast<uint32_t>(chLeds) << 18;

//     // Use the channelStrips bitmap for the red/green channel leds to set those
//     desiredLedOnStates[bank] |= newChLedsBitmap;

//     // TODO: NOW HOW TO HANDLE BLINKING?
// }