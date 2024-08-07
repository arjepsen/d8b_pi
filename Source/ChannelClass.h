/*
  ==============================================================================

    ChannelClass.h
    Created: 14 Apr 2023 9:48:22pm
    Author:  anders
    Description: This is a class for saving/referencing all the various
                 setting values of each channel.
                 This is separated from the "physical" channelstrip, because
                 a channelstrip can host any channel.
  ==============================================================================
*/

#pragma once

#include "DspComClass.h"
#include <cstdint>
#include "SharedDataStructures.h"
#include "LEDClass.h"



class Channel
{
  private:
    // References to singletons
    DspCom &dspCom;
    IntToHexLookup &intToHexLookup;
    HexToIntLookup &hexToIntLookup;
    LEDringLookup &ledRingLookup;

    // Declare bitmaps of LED states that will represent the current state of the channel.
    // These are read by channelstrips for updating their LED's.
    uint32_t desiredLedOnBitmap = 0;
    uint32_t desiredLedBlinkBitmap = 0;

    // Mask for clearing the ring LED bits.
    static const int32_t CLEAR_RING_MASK = ~0x07FF8;   // 0000 0111 1111 1111 1000 0000

    const int CH_NUMBER;
    const char *const DSP_CH_ID_STR;

    static constexpr size_t DSP_PAN_CMD_LENGTH = 18;
    static const char DSP_PAN_CMD[DSP_PAN_CMD_LENGTH]; // Declaration

    char volume[3] = "00"; // Initialize to "00".

    // Vpot controls
    // A - Master Pan (pan button below master vpot) - ch. 1-72 + 81-88 pan control.
    // B - Aux Send Level (Aux1-8 buttons)
    // C - Aux 9-10 / 11-12 send level for the stereo pair
    // D - Aux 9-10 / 11-12 PAN pan control for the stereo pair.
    // E - Level to tape
    // F - Digital trim.
    int vPotFunctionValues[NUMBER_OF_VPOT_FUNCTIONS] = {0};


    bool mute = false; // Muting sets mute=true and sends vol=0 DSP command, but keeps current volume registered, for unmuting.
    bool solo = false;
    bool select = false;

    // These should probably be made to classes.... Not sure... see below.
    // bool phaseReversed;
    // bool gateOn;
    // bool compressorOn;
    // bool eqOn;


    static uint8_t nextChannelNumber; // Static variable to keept track of next object's ID

    // Pointer to the vPot turn method - set by current vpot function.
    int (Channel::*vPotMethods[NUMBER_OF_VPOT_FUNCTIONS])(int vPotValue, EventSource source);
    
    // These are the specific vPot methods
    // TODO: These might need to be adjusted, to account for channels above 48.... (FX, RET, etc.)
    int updatePan(int vPotValue, EventSource source);
    int updateAuxSend1(int vPotValue, EventSource source);
    int updateAuxSend2(int vPotValue, EventSource source);
    int updateAuxSend3(int vPotValue, EventSource source);
    int updateAuxSend4(int vPotValue, EventSource source);
    int updateAuxSend5(int vPotValue, EventSource source);
    int updateAuxSend6(int vPotValue, EventSource source);
    int updateAuxSend7(int vPotValue, EventSource source);
    int updateAuxSend8(int vPotValue, EventSource source);
    int updateAuxSend9_10(int vPotValue, EventSource source);
    int updateAuxPan9_10(int vPotValue, EventSource source);
    int updateAuxSend11_12(int vPotValue, EventSource source);
    int updateAuxPan11_12(int vPotValue, EventSource source);
    int updateLvl2Tape(int vPotValue, EventSource source);
    int updateDigitalTrim(int vPotValue, EventSource source);



  public:
    // Constructor
    Channel();

    void updateVolume(const char (&faderValue)[2]);

    inline int vPotEvent(VpotFunction vPotFunction, int vPotValue, EventSource source)
    {
        return (this->*vPotMethods[vPotFunction])(vPotValue, source);
    }


    // Button callbacks. 
    // 1: Send dsp command
    // 2: Send Brain command for all associated strip LED's
    // 3: Update all associated UI strips.
    
    void muteBtnCallback(ButtonAction btnAction, Bank currentBank);
    void soloBtnCallback(ButtonAction btnAction, Bank currentBank);
    void selectBtnCallback(ButtonAction btnAction, Bank currentBank);
    void writeBtnCallback(ButtonAction btnAction, Bank currentBank);
    void assignBtnCallback(ButtonAction btnAction, Bank currentBank);
    void recRdyBtnCallback(ButtonAction btnAction, Bank currentBank);

    void initializeChannel(VpotFunction currentVpotFunction);

    inline const char * getVolume() const {return volume;}
    inline int getChannelNumber() {return CH_NUMBER; };
    inline bool getMuteState() { return mute; };
    inline bool getSoloState() { return solo; };
    inline bool getSelectState() { return select; };
    //inline bool getVpotDotState() { return vPotDotOn; };
    inline uint32_t getLedOnBitmap() { return desiredLedOnBitmap; };
    inline uint32_t getLedBlinkBitmap() { return desiredLedBlinkBitmap; };

    inline int getVpotValue(VpotFunction currentVpotFunction) {return vPotFunctionValues[currentVpotFunction]; };

};

// // Should these be classes also??
// int pre_dsp_insert;  // somehow must ID channel-, bus-, aux- taps or 2-track inputs or plugins.
// int post_dsp_insert; // only plugins possible as post insert. this variable must ID.
// int key_input;       // ID for what is set as key for key-eq/dynamics.. Default is channel iteself. Others: ch.tap, alt-return, bus-tap, aux-tap, 2-track input.

// // Channel DSP:
// int delay;
// int digital_trim;  // Need to sniff this one. This one is shown as the blue bar
// int level_to_tape; // the red bar.
// int phase;
// // Gate-in meter???
// int Gate;       // this might have to be a separate class?
// int Compressor; // Might also have to be a class.
// // Compressor-out meter??
// int hi_pass; // Unsure how much is in this - is it just a boolean or??
// // int EQ; // This should probably also be a class.
// float eqLow;
// float eqMid;
// float eqHigh;
// float eqLowQ;
// float eqMidQ;
// float eqHighQ;

// ================= CHANNEL VPOT FUNCTIONS =====================
// Pan: 
// Ch. 1 - 48: 127L-c-127R, Standard: c.
// FX + RET: Stereo pairs: 127L+127R
// Grp: None, MIDI: c, Bus: None
// master: front(127)-back(127). Center dot on front. 
//
// Level to tape:
// ALL channels: OFF(-90) - 0.0 - 10.0, standard: 0.0 (following same logarithmic steps as fader)
// master: none/disabled.
//
// Digital Trim:
// Ch. 1-48: Same as level to tape.
// FX+Masters banks: None.
//
// Aux1-8: 
// Line, Tape + FX banks: OFF - 10.0, standard: OFF
// Masters bank: none.
// master: OFF(-100.0) - 0.0, standard: 0.0 (dotted)
// the values for master seems to follow different scale... center seems to be around -10.0
//
// Stereo Aux (9-10, 11-12):
// Same as Aux1-8. This is called "Cue Level" in the UI.
//
// Cue Pans (two buttons below stereo auxes):
// Controls the PAN of the stereo auxes.
// Same as for the normal pan, except all None on Masters bank.
