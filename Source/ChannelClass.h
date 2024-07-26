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

// #include "BankEnum.h" // moved to event bus
//#include "BrainComClass.h"
#include "DspComClass.h"
//#include "EventBusClass.h"
#include <array>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
// #include <functional> // For pointers to vpot function depending on vpot mode.
#include "SharedDataStructures.h"
#include "LEDClass.h"
#include <functional>


class Channel
{
  private:
    // References to singletons
    //EventBus &eventBus;
    //BrainCom &brainCom;
    DspCom &dspCom;
    IntToHexLookup &intToHexLookup;
    HexToIntLookup &hexToIntLookup;
    LEDringLookup &ledRingLookup;

    // Declare bitmaps of LED states that will represent the current state of the channel.
    // These are read by channelstrips for updating their LED's.
    uint32_t desiredLedOnBitmap = 0;
    uint32_t desiredLedBlinkBitmap = 0;

    static const int32_t CLEAR_RING_MASK = ~0x07FF8;   // Mask for clearing the ring LED bits.

    const int CH_NUMBER;  // TODO: maybe enumerate this?
    // const std::string CH_ID_STR; // unique ID for each channel (1 - 48)
    const char *const DSP_CH_ID_STR;

    static constexpr size_t DSP_PAN_CMD_LENGTH = 18;
    static const char DSP_PAN_CMD[DSP_PAN_CMD_LENGTH]; // Declaration

    
    ChStripLED currentRingLED;


    //... more....
    // A - Master Pan (pan button below master vpot) - ch. 1-72 + 81-88 pan control.
    // B - Aux Send Level (Aux1-8 buttons)
    // C - Aux 9-10 / 11-12 send level for the stereo pair
    // D - Aux 9-10 / 11-12 PAN pan control for the stereo pair.
    // E - Level to tape
    // F - Digital trim.


    char volume[3] = "00"; // Initialize to "00".

    // Vpot controls
    int vPotFunctionValues[NUMBER_OF_VPOT_FUNCTIONS] = {0};


    // int pan = 0x7F; // (0 - FE) - weird things happen on "FF".... Initially "c"
    // int auxSend[8] = {0};
    // int auxStereoSend = 0;
    // int lvl2tape = 0;       // OFF - 10.0
    // int digitalTrim = 0;    // OFF - 10.0
    // int auxStereoPan = 127;

    bool mute = false; // Muting sets mute=true and sends vol=0 DSP command, but keeps current volume registered, for unmuting.
    bool solo = false;
    bool select = false;

    // These should probably be made to classes.... Not sure... see below.
    // bool phaseReversed;
    // bool gateOn;
    // bool compressorOn;
    // bool eqOn;

    // uint8_t auxSend[12];    // saved send volume for the auxes (1-8, plus 9-10, 11-12 and their pans.)
    // bool pre_post_aux_send; // saved reference of whether aux send is set to pre- or post-fader.SHOULD THIS BE SOMEHOW COMBINED WITH THE AUX-ARRAY?

    static uint8_t nextChannelNumber; // Static variable to keept track of next object's ID

    int (Channel::*vPotMethods[NUMBER_OF_VPOT_FUNCTIONS])(int vPotValue, EventSource source);
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

    //void removeChStripAssociationCallback(Bank bank, ChStripID chStripID);

    //void setVpotFunction(std::function<void(int)> func); // {vPotFunction = func}
    //void subscribeToChStrip(Bank bank, ChStripID chStripID);

    void initializeChannel(VpotFunction currentVpotFunction);
    //int getCurrentVpotValue();
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
