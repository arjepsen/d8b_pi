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

//#include "BankEnum.h" // moved to event bus
#include "BrainComClass.h"
#include "DspComClass.h"
#include "EventBusClass.h"
#include <array>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>

class Channel
{
private:
	// References to singletons
    EventBus &eventBus; 
    BrainCom &brainCom;
    DspCom &dspCom;

    // int *dspDescriptorPtr; // Reference to the DSP file descriptor.
    // int *brainDescriptorPtr; // Reference to the Brain file descriptor.

    const uint8_t channelNumber;
    const std::string channelID; // unique ID for each channel (1 - 48)

    std::unordered_map<Bank, std::unordered_set<std::string>> associatedChannelStrips;

    std::string volume;
    // uint8_t volume;          // Fader & DSP volume level. (0 - FF (hex)/ 0 - 255)
    // uint8_t pan;             // (0 - FE) - weird things happen on "FF".

    bool mute; // Muting is done by setting volume to 0, so there should be some mechanism to return to previous volume, when unmuting.
               // So when muted, receive fader volume changes and update the volume here, but dont send anything to dsp.

    // bool solo;               // maybe there should be a general list of soloed channels somewhere?

    // These should probably be made to classes.... Not sure... see below.
    // bool phaseReversed;
    // bool gateOn;
    // bool compressorOn;
    // bool eqOn;

    // bool assignments[9];     // This array should hold the list of which assignments the channel has (bus 1-8 plus L-R) (L-R = index 0)

    // std::array<char, 8> label;          // channel label.

    // uint8_t auxSend[12];    // saved send volume for the auxes (1-8, plus 9-10, 11-12 and their pans.)
    // bool pre_post_aux_send; // saved reference of whether aux send is set to pre- or post-fader.SHOULD THIS BE SOMEHOW COMBINED WITH THE AUX-ARRAY?

    static uint8_t nextChannelNumber; // Static variable to keept track of next object's ID

    // void subscribeToLineBankEvents();

public:
    // Constructor
    Channel();
    // uint8_t getVolume() { return volume; }

    void setVolume(std::string);
    std::string getID();

    // void setChannelStrip(std::string stripID);
    //void linkDspDescriptor(int *dspDescriptor);
    // void removeLineBankStripSubscription(std::string channelStripID);
    // void removeSubscription(BankEventType eventType, std::string channelStripID);

    void channelStripFaderEventCallback(const std::string faderValue, Bank bank, const std::string &channelStripID, EventSource source);
    void channelStripVpotEventCallback(const std::string vpotValue, const Bank bank, const std::string &channelStripID, EventSource source);
    void channelStripButtonEventCallback(const std::string buttonID, const Bank bank, const std::string &channelStripID, EventSource source);

    void removeChStripAssociationCallback(const Bank bank, const std::string channelStripID);
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