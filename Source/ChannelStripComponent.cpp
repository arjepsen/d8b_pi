/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...

//[/Headers]

#include "ChannelStripComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

// Set first channel ID. This will increment with every channel object constructed.
int ChannelStripComponent::nextChannelStripComponentID = 0;

//std::array<float, 256> ChannelStripComponent::precomputedLog10Values;

//[/MiscUserDefs]

//==============================================================================
ChannelStripComponent::ChannelStripComponent ()
    : eventBus(EventBus::getInstance()),
      faderValueLookup(FaderValueLookup::getInstance())
{
    //[Constructor_pre] You can add your own custom stuff here..
    // ################################ MY CONSTRUCTOR STUFF##########################################

    // Give each new channelStripComponent a unique hex ID string, from "00" and upwards.
    // This corresponds to "channelStripID" in other places of the program.
    std::stringstream stream;
    stream << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << nextChannelStripComponentID;
    channelStripComponentID = stream.str();

    // // Precompute the array of logarithms the first time this class is instantiated.
    // // This is done to avoid slow log10 computations during runtime, so we can look up the result in an array.
    // if (precomputedLog10Values[0] != -90.0) // Check if array has already been computed. (then first element would be -90)
    // {
    //     const float logFactor = 9.0 / 255; // Factor used in linear byte to fader log scale conversion.
    //     for (int i = 0; i <= 0xFF; ++i)
    //     {
    //         precomputedLog10Values[i] = static_cast<float>(log10((i * logFactor) + 1) * 100 - 90);
    //     }
    // }

    // // Precompute the map of fader/vpot values and their corresponding DSP hex strings:
    // for (int i = -900; i <= 100; i++)
    // {
    //     float faderValue = i / 10.0f;
    //     int dspValue = static_cast<int>((pow(10, (faderValue + 90) / 100.0) - 1) / 9.0 * 255);


    //     // Convert dspValue to 2-digit uppercase hex string, and store in map
    //     std::stringstream hexStream;
    //     hexStream << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << dspValue;
    //     dspHexLookupMap[faderValue] = hexStream.str();
    // }


    // Add the callbacks to the eventBus
    eventBus.chStripComponentSubscribe(channelStripComponentID, FADER_EVENT,
                                       [this](const std::string &valueString)
                                       { this->faderMoveEventCallback(valueString); });
    eventBus.chStripComponentSubscribe(channelStripComponentID, VPOT_EVENT,
                                       [this](const std::string &valueString)
                                       { this->vpotTurnEventCallback(valueString); });
    eventBus.chStripComponentSubscribe(channelStripComponentID, BUTTON_EVENT,
                                       [this](const std::string &valueString)
                                       { this->buttonEventCallback(valueString); });

    // Increment the static counter.
    nextChannelStripComponentID++;

    // ################### END OF MY CONSTRUCTOR STUFF #####################################################
    //[/Constructor_pre]

    setName ("ChannelStripComponent");
    fader.reset (new juce::Slider ("Channel Fader"));
    addAndMakeVisible (fader.get());
    fader->setRange (-90, 10, 0.1);
    fader->setSliderStyle (juce::Slider::LinearVertical);
    fader->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    fader->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x008e989b));
    fader->addListener (this);
    fader->setSkewFactor (3);

    fader->setBounds (2, 788, 71, 230);

    writeButton.reset (new juce::TextButton ("Write Button"));
    addAndMakeVisible (writeButton.get());
    writeButton->setButtonText (TRANS("WRITE"));
    writeButton->addListener (this);
    writeButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff595959));

    writeButton->setBounds (2, 572, 71, 20);

    vPot.reset (new juce::Slider ("Channel Strip V-POT"));
    addAndMakeVisible (vPot.get());
    vPot->setRange (-127, 127, 1);
    vPot->setSliderStyle (juce::Slider::Rotary);
    vPot->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 71, 20);
    vPot->setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff181f22));
    vPot->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff131919));
    vPot->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0xff242627));
    vPot->addListener (this);

    vPot->setBounds (2, 596, 71, 80);

    selectBtn.reset (new juce::TextButton ("Select Button"));
    addAndMakeVisible (selectBtn.get());
    selectBtn->setButtonText (TRANS("SELECT"));
    selectBtn->addListener (this);
    selectBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2e8d9a));
    selectBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff69cee8));

    selectBtn->setBounds (2, 684, 71, 20);

    soloBtn.reset (new juce::TextButton ("Solo Button"));
    addAndMakeVisible (soloBtn.get());
    soloBtn->setButtonText (TRANS("SOLO"));
    soloBtn->addListener (this);
    soloBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xffb08620));
    soloBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffe1ba59));

    soloBtn->setBounds (2, 708, 71, 20);

    muteBtn.reset (new juce::TextButton ("Mute Button"));
    addAndMakeVisible (muteBtn.get());
    muteBtn->setButtonText (TRANS("MUTE"));
    muteBtn->addListener (this);
    muteBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff025c02));
    muteBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colours::red);

    muteBtn->setBounds (2, 732, 71, 20);

    recordBtn.reset (new juce::TextButton ("Record Button"));
    addAndMakeVisible (recordBtn.get());
    recordBtn->setButtonText (TRANS("RECORD"));
    recordBtn->addListener (this);
    recordBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff7a2e2e));
    recordBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colours::red);

    recordBtn->setBounds (2, 548, 71, 20);

    channelSelector.reset (new juce::ComboBox ("Channel Selector"));
    addAndMakeVisible (channelSelector.get());
    channelSelector->setEditableText (false);
    channelSelector->setJustificationType (juce::Justification::centred);
    channelSelector->setTextWhenNothingSelected (TRANS("1"));
    channelSelector->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    channelSelector->addListener (this);

    channelSelector->setBounds (2, 524, 71, 20);

    aux11_12Pan.reset (new juce::Slider ("Channel Aux 11/12 Pan"));
    addAndMakeVisible (aux11_12Pan.get());
    aux11_12Pan->setRange (-127, 127, 1);
    aux11_12Pan->setSliderStyle (juce::Slider::LinearHorizontal);
    aux11_12Pan->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    aux11_12Pan->setColour (juce::Slider::backgroundColourId, juce::Colours::blue);
    aux11_12Pan->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    aux11_12Pan->setColour (juce::Slider::trackColourId, juce::Colours::blue);
    aux11_12Pan->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    aux11_12Pan->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    aux11_12Pan->addListener (this);

    aux11_12Pan->setBounds (16, 500, 54, 8);

    postEqInsert.reset (new juce::ComboBox ("Post EQ Insert Selector"));
    addAndMakeVisible (postEqInsert.get());
    postEqInsert->setEditableText (false);
    postEqInsert->setJustificationType (juce::Justification::centred);
    postEqInsert->setTextWhenNothingSelected (TRANS("POST"));
    postEqInsert->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    postEqInsert->addItem (TRANS("Reset"), 1);
    postEqInsert->addItem (TRANS("P01"), 2);
    postEqInsert->addItem (TRANS("P02"), 3);
    postEqInsert->addItem (TRANS("P03"), 4);
    postEqInsert->addItem (TRANS("P04"), 5);
    postEqInsert->addItem (TRANS("P05"), 6);
    postEqInsert->addItem (TRANS("P06"), 7);
    postEqInsert->addItem (TRANS("P07"), 8);
    postEqInsert->addItem (TRANS("P08"), 9);
    postEqInsert->addItem (TRANS("P09"), 10);
    postEqInsert->addItem (TRANS("P10"), 11);
    postEqInsert->addItem (TRANS("P11"), 12);
    postEqInsert->addItem (TRANS("P12"), 13);
    postEqInsert->addItem (TRANS("P13"), 14);
    postEqInsert->addItem (TRANS("P14"), 15);
    postEqInsert->addItem (TRANS("P15"), 16);
    postEqInsert->addItem (TRANS("P16"), 17);
    postEqInsert->addListener (this);

    postEqInsert->setBounds (2, 316, 71, 20);

    aux7Send.reset (new juce::Slider ("Channel Aux 7 Send"));
    addAndMakeVisible (aux7Send.get());
    aux7Send->setRange (-90, 10, 1);
    aux7Send->setSliderStyle (juce::Slider::LinearBar);
    aux7Send->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    aux7Send->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00263238));
    aux7Send->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    aux7Send->setColour (juce::Slider::trackColourId, juce::Colours::red);
    aux7Send->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    aux7Send->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    aux7Send->addListener (this);

    aux7Send->setBounds (16, 440, 54, 8);

    aux11_12Send.reset (new juce::Slider ("Channel Aux 11/12 Send"));
    addAndMakeVisible (aux11_12Send.get());
    aux11_12Send->setRange (-90, 10, 1);
    aux11_12Send->setSliderStyle (juce::Slider::LinearBar);
    aux11_12Send->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    aux11_12Send->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00263238));
    aux11_12Send->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    aux11_12Send->setColour (juce::Slider::trackColourId, juce::Colour (0xffffc900));
    aux11_12Send->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    aux11_12Send->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    aux11_12Send->addListener (this);

    aux11_12Send->setBounds (16, 488, 54, 8);

    aux9_10Pan.reset (new juce::Slider ("Channel Aux 910 Pan"));
    addAndMakeVisible (aux9_10Pan.get());
    aux9_10Pan->setRange (-127, 127, 1);
    aux9_10Pan->setSliderStyle (juce::Slider::LinearHorizontal);
    aux9_10Pan->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    aux9_10Pan->setColour (juce::Slider::backgroundColourId, juce::Colours::blue);
    aux9_10Pan->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    aux9_10Pan->setColour (juce::Slider::trackColourId, juce::Colours::blue);
    aux9_10Pan->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    aux9_10Pan->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    aux9_10Pan->addListener (this);

    aux9_10Pan->setBounds (16, 476, 54, 8);

    aux9_10Send.reset (new juce::Slider ("Channel Aux 910 Send"));
    addAndMakeVisible (aux9_10Send.get());
    aux9_10Send->setRange (-90, 10, 1);
    aux9_10Send->setSliderStyle (juce::Slider::LinearBar);
    aux9_10Send->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    aux9_10Send->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00263238));
    aux9_10Send->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    aux9_10Send->setColour (juce::Slider::trackColourId, juce::Colour (0xffffc900));
    aux9_10Send->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    aux9_10Send->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    aux9_10Send->addListener (this);

    aux9_10Send->setBounds (16, 464, 54, 8);

    aux8Send.reset (new juce::Slider ("Channel Aux 8 Send"));
    addAndMakeVisible (aux8Send.get());
    aux8Send->setRange (-90, 10, 1);
    aux8Send->setSliderStyle (juce::Slider::LinearBar);
    aux8Send->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    aux8Send->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00263238));
    aux8Send->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    aux8Send->setColour (juce::Slider::trackColourId, juce::Colours::red);
    aux8Send->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    aux8Send->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    aux8Send->addListener (this);

    aux8Send->setBounds (16, 452, 54, 8);

    aux6Send.reset (new juce::Slider ("Channel Aux 6 Send"));
    addAndMakeVisible (aux6Send.get());
    aux6Send->setRange (-90, 10, 1);
    aux6Send->setSliderStyle (juce::Slider::LinearBar);
    aux6Send->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    aux6Send->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00263238));
    aux6Send->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    aux6Send->setColour (juce::Slider::trackColourId, juce::Colours::red);
    aux6Send->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    aux6Send->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    aux6Send->addListener (this);

    aux6Send->setBounds (16, 428, 54, 8);

    aux4Send.reset (new juce::Slider ("Channel Aux 4 Send"));
    addAndMakeVisible (aux4Send.get());
    aux4Send->setRange (-90, 10, 1);
    aux4Send->setSliderStyle (juce::Slider::LinearBar);
    aux4Send->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    aux4Send->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00263238));
    aux4Send->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    aux4Send->setColour (juce::Slider::trackColourId, juce::Colours::red);
    aux4Send->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    aux4Send->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    aux4Send->addListener (this);

    aux4Send->setBounds (16, 404, 54, 8);

    aux5Send.reset (new juce::Slider ("Channel Aux 5 Send"));
    addAndMakeVisible (aux5Send.get());
    aux5Send->setRange (-90, 10, 1);
    aux5Send->setSliderStyle (juce::Slider::LinearBar);
    aux5Send->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    aux5Send->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00263238));
    aux5Send->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    aux5Send->setColour (juce::Slider::trackColourId, juce::Colours::red);
    aux5Send->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    aux5Send->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    aux5Send->addListener (this);

    aux5Send->setBounds (16, 416, 54, 8);

    aux3Send.reset (new juce::Slider ("Channel Aux 3 Send"));
    addAndMakeVisible (aux3Send.get());
    aux3Send->setRange (-90, 10, 1);
    aux3Send->setSliderStyle (juce::Slider::LinearBar);
    aux3Send->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    aux3Send->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00263238));
    aux3Send->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    aux3Send->setColour (juce::Slider::trackColourId, juce::Colours::red);
    aux3Send->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    aux3Send->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    aux3Send->addListener (this);

    aux3Send->setBounds (16, 392, 54, 8);

    aux2Send.reset (new juce::Slider ("Channel Aux 2 Send"));
    addAndMakeVisible (aux2Send.get());
    aux2Send->setRange (-90, 10, 1);
    aux2Send->setSliderStyle (juce::Slider::LinearBar);
    aux2Send->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    aux2Send->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00263238));
    aux2Send->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    aux2Send->setColour (juce::Slider::trackColourId, juce::Colours::red);
    aux2Send->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    aux2Send->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    aux2Send->addListener (this);

    aux2Send->setBounds (16, 380, 54, 8);

    aux1Send.reset (new juce::Slider ("Channel Aux 1 Send"));
    addAndMakeVisible (aux1Send.get());
    aux1Send->setRange (-90, 10, 1);
    aux1Send->setSliderStyle (juce::Slider::LinearBar);
    aux1Send->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    aux1Send->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00263238));
    aux1Send->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    aux1Send->setColour (juce::Slider::trackColourId, juce::Colours::red);
    aux1Send->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    aux1Send->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    aux1Send->addListener (this);

    aux1Send->setBounds (16, 368, 54, 8);

    postEqInsertLabel.reset (new juce::Label ("Post EQ Insert Label",
                                              TRANS("P01")));
    addAndMakeVisible (postEqInsertLabel.get());
    postEqInsertLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    postEqInsertLabel->setJustificationType (juce::Justification::centred);
    postEqInsertLabel->setEditable (false, false, false);
    postEqInsertLabel->setColour (juce::Label::backgroundColourId, juce::Colour (0xff140404));
    postEqInsertLabel->setColour (juce::Label::outlineColourId, juce::Colour (0x044d4c4c));
    postEqInsertLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    postEqInsertLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    postEqInsertLabel->setBounds (2, 292, 71, 20);

    chLabel.reset (new juce::Label ("Channel Label",
                                    juce::String()));
    addAndMakeVisible (chLabel.get());
    chLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    chLabel->setJustificationType (juce::Justification::centred);
    chLabel->setEditable (true, true, false);
    chLabel->setColour (juce::Label::outlineColourId, juce::Colours::black);
    chLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    chLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));
    chLabel->addListener (this);

    chLabel->setBounds (2, 764, 71, 20);

    eqThumbnail.reset (new juce::Component());
    addAndMakeVisible (eqThumbnail.get());
    eqThumbnail->setName ("place_holder_for_eq_show");

    eqThumbnail->setBounds (2, 244, 71, 40);

    phaseBtn.reset (new juce::TextButton ("Channel Phase Button"));
    addAndMakeVisible (phaseBtn.get());
    phaseBtn->setButtonText (juce::CharPointer_UTF8 ("\xc3\x98"));
    phaseBtn->addListener (this);
    phaseBtn->setColour (juce::TextButton::buttonColourId, juce::Colours::grey);

    phaseBtn->setBounds (6, 220, 30, 20);

    eqBtn.reset (new juce::TextButton ("Channel EQ Button"));
    addAndMakeVisible (eqBtn.get());
    eqBtn->setButtonText (TRANS("EQ"));
    eqBtn->addListener (this);
    eqBtn->setColour (juce::TextButton::buttonColourId, juce::Colours::grey);

    eqBtn->setBounds (40, 220, 30, 20);

    compressorBtn.reset (new juce::TextButton ("Channel Compressor Button"));
    addAndMakeVisible (compressorBtn.get());
    compressorBtn->setButtonText (TRANS("C"));
    compressorBtn->addListener (this);
    compressorBtn->setColour (juce::TextButton::buttonColourId, juce::Colours::grey);

    compressorBtn->setBounds (6, 196, 30, 20);

    preEqInsertLabel.reset (new juce::Label ("Pre EQ Insert Label",
                                             TRANS("P01")));
    addAndMakeVisible (preEqInsertLabel.get());
    preEqInsertLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    preEqInsertLabel->setJustificationType (juce::Justification::centred);
    preEqInsertLabel->setEditable (false, false, false);
    preEqInsertLabel->setColour (juce::Label::backgroundColourId, juce::Colour (0xff140404));
    preEqInsertLabel->setColour (juce::Label::outlineColourId, juce::Colour (0x044d4c4c));
    preEqInsertLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    preEqInsertLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    preEqInsertLabel->setBounds (2, 4, 71, 20);

    preEqInsert.reset (new juce::ComboBox ("Pre EQ Insert Selector"));
    addAndMakeVisible (preEqInsert.get());
    preEqInsert->setEditableText (false);
    preEqInsert->setJustificationType (juce::Justification::centred);
    preEqInsert->setTextWhenNothingSelected (TRANS("PRE"));
    preEqInsert->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    preEqInsert->addItem (TRANS("Reset"), 1);
    preEqInsert->addItem (TRANS("P01"), 2);
    preEqInsert->addItem (TRANS("P02"), 3);
    preEqInsert->addItem (TRANS("P03"), 4);
    preEqInsert->addItem (TRANS("P04"), 5);
    preEqInsert->addItem (TRANS("P05"), 6);
    preEqInsert->addItem (TRANS("P06"), 7);
    preEqInsert->addItem (TRANS("P07"), 8);
    preEqInsert->addItem (TRANS("P08"), 9);
    preEqInsert->addItem (TRANS("P09"), 10);
    preEqInsert->addItem (TRANS("P10"), 11);
    preEqInsert->addItem (TRANS("P11"), 12);
    preEqInsert->addItem (TRANS("P12"), 13);
    preEqInsert->addItem (TRANS("P13"), 14);
    preEqInsert->addItem (TRANS("P14"), 15);
    preEqInsert->addItem (TRANS("P15"), 16);
    preEqInsert->addItem (TRANS("P16"), 17);
    preEqInsert->addListener (this);

    preEqInsert->setBounds (2, 28, 71, 20);

    gateBtn.reset (new juce::TextButton ("Channel Gate Button"));
    addAndMakeVisible (gateBtn.get());
    gateBtn->setButtonText (TRANS("G"));
    gateBtn->addListener (this);
    gateBtn->setColour (juce::TextButton::buttonColourId, juce::Colours::grey);

    gateBtn->setBounds (40, 196, 30, 20);

    chAssignLRBtn.reset (new juce::TextButton ("L/R Assignment Button"));
    addAndMakeVisible (chAssignLRBtn.get());
    chAssignLRBtn->setButtonText (TRANS("L R"));
    chAssignLRBtn->addListener (this);
    chAssignLRBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2a8061));
    chAssignLRBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colours::red);

    chAssignLRBtn->setBounds (2, 124, 71, 14);

    bus7AssignBtn.reset (new juce::TextButton ("Bus 7 Assign Button"));
    addAndMakeVisible (bus7AssignBtn.get());
    bus7AssignBtn->setButtonText (TRANS("7"));
    bus7AssignBtn->addListener (this);
    bus7AssignBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2a8061));
    bus7AssignBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colours::red);

    bus7AssignBtn->setBounds (2, 108, 34, 14);

    bus8AssignBtn.reset (new juce::TextButton ("Bus 8 Assign Button"));
    addAndMakeVisible (bus8AssignBtn.get());
    bus8AssignBtn->setButtonText (TRANS("8"));
    bus8AssignBtn->addListener (this);
    bus8AssignBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2a8061));
    bus8AssignBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colours::red);

    bus8AssignBtn->setBounds (40, 108, 34, 14);

    bus5AssignBtn.reset (new juce::TextButton ("Bus 5 Assign Button"));
    addAndMakeVisible (bus5AssignBtn.get());
    bus5AssignBtn->setButtonText (TRANS("5"));
    bus5AssignBtn->addListener (this);
    bus5AssignBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2a8061));
    bus5AssignBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colours::red);

    bus5AssignBtn->setBounds (2, 92, 34, 14);

    bus6AssignBtn.reset (new juce::TextButton ("Bus 6 Assign Button"));
    addAndMakeVisible (bus6AssignBtn.get());
    bus6AssignBtn->setButtonText (TRANS("6"));
    bus6AssignBtn->addListener (this);
    bus6AssignBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2a8061));
    bus6AssignBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colours::red);

    bus6AssignBtn->setBounds (40, 92, 34, 14);

    bus3AssignBtn.reset (new juce::TextButton ("Bus 3 Assign Button"));
    addAndMakeVisible (bus3AssignBtn.get());
    bus3AssignBtn->setButtonText (TRANS("3"));
    bus3AssignBtn->addListener (this);
    bus3AssignBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2a8061));
    bus3AssignBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colours::red);

    bus3AssignBtn->setBounds (2, 76, 34, 14);

    bus4AssignBtn.reset (new juce::TextButton ("Bus 4 Assign Button"));
    addAndMakeVisible (bus4AssignBtn.get());
    bus4AssignBtn->setButtonText (TRANS("4"));
    bus4AssignBtn->addListener (this);
    bus4AssignBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2a8061));
    bus4AssignBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colours::red);

    bus4AssignBtn->setBounds (40, 76, 34, 14);

    bus1AssignBtn.reset (new juce::TextButton ("Bus 1 Assign Button"));
    addAndMakeVisible (bus1AssignBtn.get());
    bus1AssignBtn->setButtonText (TRANS("1"));
    bus1AssignBtn->addListener (this);
    bus1AssignBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2a8061));
    bus1AssignBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colours::red);

    bus1AssignBtn->setBounds (2, 60, 34, 14);

    bus2AssignBtn.reset (new juce::TextButton ("Bus 2 Assign Button"));
    addAndMakeVisible (bus2AssignBtn.get());
    bus2AssignBtn->setButtonText (TRANS("2"));
    bus2AssignBtn->addListener (this);
    bus2AssignBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff2a8061));
    bus2AssignBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colours::red);

    bus2AssignBtn->setBounds (40, 60, 34, 14);

    chDigitalTrim.reset (new juce::Slider ("Channel Digital Trim"));
    addAndMakeVisible (chDigitalTrim.get());
    chDigitalTrim->setRange (-90, 10, 1);
    chDigitalTrim->setSliderStyle (juce::Slider::LinearBar);
    chDigitalTrim->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    chDigitalTrim->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00263238));
    chDigitalTrim->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    chDigitalTrim->setColour (juce::Slider::trackColourId, juce::Colours::blue);
    chDigitalTrim->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    chDigitalTrim->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    chDigitalTrim->addListener (this);

    chDigitalTrim->setBounds (16, 180, 52, 8);

    juce__comboBox4.reset (new juce::ComboBox ("Direct Out Assignment"));
    addAndMakeVisible (juce__comboBox4.get());
    juce__comboBox4->setEditableText (false);
    juce__comboBox4->setJustificationType (juce::Justification::centred);
    juce__comboBox4->setTextWhenNothingSelected (TRANS("-"));
    juce__comboBox4->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    juce__comboBox4->addItem (TRANS(" - "), 1);
    juce__comboBox4->addItem (TRANS("tape out 1 - (ch. 25)"), 2);
    juce__comboBox4->addItem (TRANS("tape out 2 - (ch. 26)"), 3);
    juce__comboBox4->addItem (TRANS("tape out 3 - (ch. 27)"), 4);
    juce__comboBox4->addSeparator();
    juce__comboBox4->addListener (this);

    juce__comboBox4->setBounds (8, 148, 60, 14);

    chLevelToTape.reset (new juce::Slider ("Channel Level To Tape"));
    addAndMakeVisible (chLevelToTape.get());
    chLevelToTape->setRange (-90, 10, 1);
    chLevelToTape->setSliderStyle (juce::Slider::LinearBar);
    chLevelToTape->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    chLevelToTape->setColour (juce::Slider::backgroundColourId, juce::Colour (0x00263238));
    chLevelToTape->setColour (juce::Slider::thumbColourId, juce::Colour (0xff79d5fa));
    chLevelToTape->setColour (juce::Slider::trackColourId, juce::Colours::red);
    chLevelToTape->setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    chLevelToTape->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x4d8e989b));
    chLevelToTape->addListener (this);

    chLevelToTape->setBounds (16, 166, 52, 8);


    //[UserPreSize]

    //[/UserPreSize]

    setSize (75, 1024);


    //[Constructor] You can add your own custom stuff here..

    // Set initial channel strip label (nextID incremented, so fits now.)
    chLabel->setText("Ch. " + (juce::String)nextChannelStripComponentID, juce::dontSendNotification);

    //[/Constructor]
}

ChannelStripComponent::~ChannelStripComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

    //[/Destructor_pre]

    fader = nullptr;
    writeButton = nullptr;
    vPot = nullptr;
    selectBtn = nullptr;
    soloBtn = nullptr;
    muteBtn = nullptr;
    recordBtn = nullptr;
    channelSelector = nullptr;
    aux11_12Pan = nullptr;
    postEqInsert = nullptr;
    aux7Send = nullptr;
    aux11_12Send = nullptr;
    aux9_10Pan = nullptr;
    aux9_10Send = nullptr;
    aux8Send = nullptr;
    aux6Send = nullptr;
    aux4Send = nullptr;
    aux5Send = nullptr;
    aux3Send = nullptr;
    aux2Send = nullptr;
    aux1Send = nullptr;
    postEqInsertLabel = nullptr;
    chLabel = nullptr;
    eqThumbnail = nullptr;
    phaseBtn = nullptr;
    eqBtn = nullptr;
    compressorBtn = nullptr;
    preEqInsertLabel = nullptr;
    preEqInsert = nullptr;
    gateBtn = nullptr;
    chAssignLRBtn = nullptr;
    bus7AssignBtn = nullptr;
    bus8AssignBtn = nullptr;
    bus5AssignBtn = nullptr;
    bus6AssignBtn = nullptr;
    bus3AssignBtn = nullptr;
    bus4AssignBtn = nullptr;
    bus1AssignBtn = nullptr;
    bus2AssignBtn = nullptr;
    chDigitalTrim = nullptr;
    juce__comboBox4 = nullptr;
    chLevelToTape = nullptr;


    //[Destructor]. You can add your own custom destruction code here..

    //[/Destructor]
}

//==============================================================================
void ChannelStripComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..

    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff242d31));

    {
        int x = 0, y = 0, width = 75, height = 1024;
        juce::Colour fillColour = juce::Colour (0xff294e55);
        juce::Colour strokeColour = juce::Colour (0xff1c0b07);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 12, y = 364, width = 60, height = 152;
        juce::Colour fillColour = juce::Colour (0xff162a2e);
        juce::Colour strokeColour = juce::Colours::grey;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 2);

    }

    {
        int x = 0, y = 500, width = 11, height = 8;
        juce::String text (TRANS("P"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 476, width = 12, height = 8;
        juce::String text (TRANS("P"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 488, width = 11, height = 8;
        juce::String text (TRANS("11"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 464, width = 11, height = 8;
        juce::String text (TRANS("9"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 452, width = 11, height = 8;
        juce::String text (TRANS("8"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 440, width = 11, height = 8;
        juce::String text (TRANS("7"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 428, width = 11, height = 8;
        juce::String text (TRANS("6"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 416, width = 11, height = 8;
        juce::String text (TRANS("5"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 404, width = 11, height = 8;
        juce::String text (TRANS("4"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 392, width = 11, height = 8;
        juce::String text (TRANS("3"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 380, width = 11, height = 8;
        juce::String text (TRANS("2"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 368, width = 11, height = 8;
        juce::String text (TRANS("1"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 2, y = 344, width = 71, height = 20;
        juce::String text (TRANS("AUXES"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 4, y = 144, width = 68, height = 48;
        juce::Colour fillColour = juce::Colour (0xff030303);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 4, y = 180, width = 11, height = 8;
        juce::String text (TRANS("D"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 4, y = 166, width = 11, height = 8;
        juce::String text (TRANS("T"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 7, y = 846, width = 14, height = 8;
        juce::String text (TRANS("U -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 893, width = 20, height = 8;
        juce::String text (TRANS("10 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 934, width = 20, height = 8;
        juce::String text (TRANS("30 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 956, width = 20, height = 8;
        juce::String text (TRANS("40 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 870, width = 20, height = 8;
        juce::String text (TRANS("5 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 827, width = 20, height = 8;
        juce::String text (TRANS("5 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 803, width = 20, height = 8;
        juce::String text (TRANS("10 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 914, width = 20, height = 8;
        juce::String text (TRANS("20 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    //[UserPaint] Add your own custom painting code here..

    //[/UserPaint]
}

void ChannelStripComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..

    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

    //[/UserResized]
}

void ChannelStripComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]

    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == fader.get())
    {
        //[UserSliderCode_fader] -- add your slider handling code here..

        // Fader was moved in the UI.
        float newFaderValue = std::round(sliderThatWasMoved->getValue() * 10.0f) / 10.0f;
        std::string dspFaderValue = faderValueLookup.dspHexLookupMap[newFaderValue];

        // Use event post
        eventBus.postEvent(FADER_EVENT, channelStripComponentID, dspFaderValue, UI_EVENT);

        //[/UserSliderCode_fader]
    }
    else if (sliderThatWasMoved == vPot.get())
    {
        //[UserSliderCode_vPot] -- add your slider handling code here..

        // Shift the value from -127 - 127 to 0 - 255
        uint8_t shiftedValue = static_cast<uint8_t>(sliderThatWasMoved->getValue() + 127.0);

        // Use a stringstream to convert the uint8_t to a 2-digit upper case hex string
        std::stringstream ss;
        ss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(shiftedValue);

        // The hex string is now in ss.str()
        std::string dspVpotValue = ss.str();

        // Post the event for the Channel object to handle.
        eventBus.postEvent(VPOT_EVENT, channelStripComponentID, dspVpotValue, UI_EVENT);


        //[/UserSliderCode_vPot]
    }
    else if (sliderThatWasMoved == aux11_12Pan.get())
    {
        //[UserSliderCode_aux11_12Pan] -- add your slider handling code here..

        //[/UserSliderCode_aux11_12Pan]
    }
    else if (sliderThatWasMoved == aux7Send.get())
    {
        //[UserSliderCode_aux7Send] -- add your slider handling code here..

        //[/UserSliderCode_aux7Send]
    }
    else if (sliderThatWasMoved == aux11_12Send.get())
    {
        //[UserSliderCode_aux11_12Send] -- add your slider handling code here..

        //[/UserSliderCode_aux11_12Send]
    }
    else if (sliderThatWasMoved == aux9_10Pan.get())
    {
        //[UserSliderCode_aux9_10Pan] -- add your slider handling code here..

        //[/UserSliderCode_aux9_10Pan]
    }
    else if (sliderThatWasMoved == aux9_10Send.get())
    {
        //[UserSliderCode_aux9_10Send] -- add your slider handling code here..

        //[/UserSliderCode_aux9_10Send]
    }
    else if (sliderThatWasMoved == aux8Send.get())
    {
        //[UserSliderCode_aux8Send] -- add your slider handling code here..

        //[/UserSliderCode_aux8Send]
    }
    else if (sliderThatWasMoved == aux6Send.get())
    {
        //[UserSliderCode_aux6Send] -- add your slider handling code here..

        //[/UserSliderCode_aux6Send]
    }
    else if (sliderThatWasMoved == aux4Send.get())
    {
        //[UserSliderCode_aux4Send] -- add your slider handling code here..

        //[/UserSliderCode_aux4Send]
    }
    else if (sliderThatWasMoved == aux5Send.get())
    {
        //[UserSliderCode_aux5Send] -- add your slider handling code here..

        //[/UserSliderCode_aux5Send]
    }
    else if (sliderThatWasMoved == aux3Send.get())
    {
        //[UserSliderCode_aux3Send] -- add your slider handling code here..

        //[/UserSliderCode_aux3Send]
    }
    else if (sliderThatWasMoved == aux2Send.get())
    {
        //[UserSliderCode_aux2Send] -- add your slider handling code here..

        //[/UserSliderCode_aux2Send]
    }
    else if (sliderThatWasMoved == aux1Send.get())
    {
        //[UserSliderCode_aux1Send] -- add your slider handling code here..

        //[/UserSliderCode_aux1Send]
    }
    else if (sliderThatWasMoved == chDigitalTrim.get())
    {
        //[UserSliderCode_chDigitalTrim] -- add your slider handling code here..

        //[/UserSliderCode_chDigitalTrim]
    }
    else if (sliderThatWasMoved == chLevelToTape.get())
    {
        //[UserSliderCode_chLevelToTape] -- add your slider handling code here..

        //[/UserSliderCode_chLevelToTape]
    }

    //[UsersliderValueChanged_Post]

    //[/UsersliderValueChanged_Post]
}

void ChannelStripComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]

    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == writeButton.get())
    {
        //[UserButtonCode_writeButton] -- add your button handler code here..

        //[/UserButtonCode_writeButton]
    }
    else if (buttonThatWasClicked == selectBtn.get())
    {
        //[UserButtonCode_selectBtn] -- add your button handler code here..

        //[/UserButtonCode_selectBtn]
    }
    else if (buttonThatWasClicked == soloBtn.get())
    {
        //[UserButtonCode_soloBtn] -- add your button handler code here..

        //[/UserButtonCode_soloBtn]
    }
    else if (buttonThatWasClicked == muteBtn.get())
    {
        //[UserButtonCode_muteBtn] -- add your button handler code here..

        //[/UserButtonCode_muteBtn]
    }
    else if (buttonThatWasClicked == recordBtn.get())
    {
        //[UserButtonCode_recordBtn] -- add your button handler code here..

        //[/UserButtonCode_recordBtn]
    }
    else if (buttonThatWasClicked == phaseBtn.get())
    {
        //[UserButtonCode_phaseBtn] -- add your button handler code here..

        //[/UserButtonCode_phaseBtn]
    }
    else if (buttonThatWasClicked == eqBtn.get())
    {
        //[UserButtonCode_eqBtn] -- add your button handler code here..

        //[/UserButtonCode_eqBtn]
    }
    else if (buttonThatWasClicked == compressorBtn.get())
    {
        //[UserButtonCode_compressorBtn] -- add your button handler code here..

        //[/UserButtonCode_compressorBtn]
    }
    else if (buttonThatWasClicked == gateBtn.get())
    {
        //[UserButtonCode_gateBtn] -- add your button handler code here..

        //[/UserButtonCode_gateBtn]
    }
    else if (buttonThatWasClicked == chAssignLRBtn.get())
    {
        //[UserButtonCode_chAssignLRBtn] -- add your button handler code here..

        //[/UserButtonCode_chAssignLRBtn]
    }
    else if (buttonThatWasClicked == bus7AssignBtn.get())
    {
        //[UserButtonCode_bus7AssignBtn] -- add your button handler code here..

        //[/UserButtonCode_bus7AssignBtn]
    }
    else if (buttonThatWasClicked == bus8AssignBtn.get())
    {
        //[UserButtonCode_bus8AssignBtn] -- add your button handler code here..

        //[/UserButtonCode_bus8AssignBtn]
    }
    else if (buttonThatWasClicked == bus5AssignBtn.get())
    {
        //[UserButtonCode_bus5AssignBtn] -- add your button handler code here..

        //[/UserButtonCode_bus5AssignBtn]
    }
    else if (buttonThatWasClicked == bus6AssignBtn.get())
    {
        //[UserButtonCode_bus6AssignBtn] -- add your button handler code here..

        //[/UserButtonCode_bus6AssignBtn]
    }
    else if (buttonThatWasClicked == bus3AssignBtn.get())
    {
        //[UserButtonCode_bus3AssignBtn] -- add your button handler code here..

        //[/UserButtonCode_bus3AssignBtn]
    }
    else if (buttonThatWasClicked == bus4AssignBtn.get())
    {
        //[UserButtonCode_bus4AssignBtn] -- add your button handler code here..

        //[/UserButtonCode_bus4AssignBtn]
    }
    else if (buttonThatWasClicked == bus1AssignBtn.get())
    {
        //[UserButtonCode_bus1AssignBtn] -- add your button handler code here..

        //[/UserButtonCode_bus1AssignBtn]
    }
    else if (buttonThatWasClicked == bus2AssignBtn.get())
    {
        //[UserButtonCode_bus2AssignBtn] -- add your button handler code here..

        //[/UserButtonCode_bus2AssignBtn]
    }

    //[UserbuttonClicked_Post]

    //[/UserbuttonClicked_Post]
}

void ChannelStripComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]

    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == channelSelector.get())
    {
        //[UserComboBoxCode_channelSelector] -- add your combo box handling code here..

        //[/UserComboBoxCode_channelSelector]
    }
    else if (comboBoxThatHasChanged == postEqInsert.get())
    {
        //[UserComboBoxCode_postEqInsert] -- add your combo box handling code here..

        //[/UserComboBoxCode_postEqInsert]
    }
    else if (comboBoxThatHasChanged == preEqInsert.get())
    {
        //[UserComboBoxCode_preEqInsert] -- add your combo box handling code here..

        //[/UserComboBoxCode_preEqInsert]
    }
    else if (comboBoxThatHasChanged == juce__comboBox4.get())
    {
        //[UserComboBoxCode_juce__comboBox4] -- add your combo box handling code here..

        //[/UserComboBoxCode_juce__comboBox4]
    }

    //[UsercomboBoxChanged_Post]

    //[/UsercomboBoxChanged_Post]
}

void ChannelStripComponent::labelTextChanged (juce::Label* labelThatHasChanged)
{
    //[UserlabelTextChanged_Pre]

    //[/UserlabelTextChanged_Pre]

    if (labelThatHasChanged == chLabel.get())
    {
        //[UserLabelCode_chLabel] -- add your label text handling code here..

        //[/UserLabelCode_chLabel]
    }

    //[UserlabelTextChanged_Post]

    //[/UserlabelTextChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

// #################################################################################################
// This method is called, when a fader is moved on the physical mixer.
// "dontSendNotification" prevents a cyclic reaction, where the ui will then try to update the mixer
// #################################################################################################

// TODO: is this method used at all?
void ChannelStripComponent::setFaderPosition(double value)
{
    // TODO: do we still need dontsendnotifacation with the current implementation?
    juce::MessageManager::callAsync([this, value]()
                                    { fader.get()->setValue(value, juce::dontSendNotification); });
}

// void ChannelStripComponent::setFaderMoveCallbackFunction(std::function<void(std::string, float)> callbackFunction)
// {
// 	faderMoveCallback = callbackFunction;
// }

// ################################################################################################
// This is a callback method to be used by the eventBus. It will be called, when the channel object
// has sent it's commands to the DSP, and Brain, and all that is left is to update the UI.
// It looks up the value in the precomputed map, and sets the fader to the given value.
// ################################################################################################
void ChannelStripComponent::faderMoveEventCallback(std::string faderHexValue)
{
    int decimalValue = std::stoi(faderHexValue, nullptr, 16);
    // double faderValue = log10((decimalValue * logFactor) + 1) * 100 - 90;
    // setFaderPosition(precomputedLog10Values[decimalValue]);
    float logValue = faderValueLookup.precomputedLog10Values[decimalValue];
    juce::MessageManager::callAsync([this, logValue]()
                                    { fader.get()->setValue(logValue, juce::dontSendNotification); });
}

// ####################################################
// Same as above, this just updates the VPot in the UI.
// ####################################################
void ChannelStripComponent::vpotTurnEventCallback(std::string vpotHexValue)
{
    // Convert string hex value to a float
    float decimalValue = std::stoi(vpotHexValue, nullptr, 16);

    // Adjust to fit the -127 to 127 range
    decimalValue -= 127.0;

    juce::MessageManager::callAsync([this, decimalValue]()
                                    { vPot.get()->setValue(decimalValue, juce::dontSendNotification); });
}

// #####################################################################################################
// This is the callback for a button event. This might be somewhat different than a button event,
// although this is primarily for updating the UI, after the Channel class has handled the button press.
// #####################################################################################################
void ChannelStripComponent::buttonEventCallback(std::string buttonValue)
{
    // TODO implement handling of channel strip buttons.
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ChannelStripComponent" componentName="ChannelStripComponent"
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers="eventBus(EventBus::getInstance()),&#10;    faderValueLookup(FaderValueLookup::getInstance())"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="75" initialHeight="1024">
  <BACKGROUND backgroundColour="ff242d31">
    <RECT pos="0 0 75 1024" fill="solid: ff294e55" hasStroke="1" stroke="0.9, mitered, butt"
          strokeColour="solid: ff1c0b07"/>
    <RECT pos="12 364 60 152" fill="solid: ff162a2e" hasStroke="1" stroke="1.5, mitered, butt"
          strokeColour="solid: ff808080"/>
    <TEXT pos="0 500 11 8" fill="solid: fff0f8ff" hasStroke="0" text="P"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 476 12 8" fill="solid: fff0f8ff" hasStroke="0" text="P"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 488 11 8" fill="solid: fff0f8ff" hasStroke="0" text="11"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 464 11 8" fill="solid: fff0f8ff" hasStroke="0" text="9"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 452 11 8" fill="solid: fff0f8ff" hasStroke="0" text="8"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 440 11 8" fill="solid: fff0f8ff" hasStroke="0" text="7"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 428 11 8" fill="solid: fff0f8ff" hasStroke="0" text="6"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 416 11 8" fill="solid: fff0f8ff" hasStroke="0" text="5"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 404 11 8" fill="solid: fff0f8ff" hasStroke="0" text="4"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 392 11 8" fill="solid: fff0f8ff" hasStroke="0" text="3"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 380 11 8" fill="solid: fff0f8ff" hasStroke="0" text="2"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 368 11 8" fill="solid: fff0f8ff" hasStroke="0" text="1"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="2 344 71 20" fill="solid: fff0f8ff" hasStroke="0" text="AUXES"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <RECT pos="4 144 68 48" fill="solid: ff030303" hasStroke="0"/>
    <TEXT pos="4 180 11 8" fill="solid: fff0f8ff" hasStroke="0" text="D"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="4 166 11 8" fill="solid: fff0f8ff" hasStroke="0" text="T"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="7 846 14 8" fill="solid: fff0f8ff" hasStroke="0" text="U -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 893 20 8" fill="solid: fff0f8ff" hasStroke="0" text="10 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 934 20 8" fill="solid: fff0f8ff" hasStroke="0" text="30 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 956 20 8" fill="solid: fff0f8ff" hasStroke="0" text="40 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 870 20 8" fill="solid: fff0f8ff" hasStroke="0" text="5 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 827 20 8" fill="solid: fff0f8ff" hasStroke="0" text="5 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 803 20 8" fill="solid: fff0f8ff" hasStroke="0" text="10 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 914 20 8" fill="solid: fff0f8ff" hasStroke="0" text="20 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="Channel Fader" id="628ad89d92251169" memberName="fader"
          virtualName="" explicitFocusOrder="0" pos="2 788 71 230" textboxoutline="8e989b"
          min="-90.0" max="10.0" int="0.1" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="50" textBoxHeight="20" skewFactor="3.0"
          needsCallback="1"/>
  <TEXTBUTTON name="Write Button" id="4bdf7e0855562adc" memberName="writeButton"
              virtualName="" explicitFocusOrder="0" pos="2 572 71 20" bgColOff="ff595959"
              buttonText="WRITE" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="Channel Strip V-POT" id="8303a7c33dfa3daf" memberName="vPot"
          virtualName="" explicitFocusOrder="0" pos="2 596 71 80" rotaryslideroutline="ff181f22"
          textboxbkgd="ff131919" textboxoutline="ff242627" min="-127.0"
          max="127.0" int="1.0" style="Rotary" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="71" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TEXTBUTTON name="Select Button" id="9388e21ee855ae90" memberName="selectBtn"
              virtualName="" explicitFocusOrder="0" pos="2 684 71 20" bgColOff="ff2e8d9a"
              bgColOn="ff69cee8" buttonText="SELECT" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Solo Button" id="857a44f3f2757580" memberName="soloBtn"
              virtualName="" explicitFocusOrder="0" pos="2 708 71 20" bgColOff="ffb08620"
              bgColOn="ffe1ba59" buttonText="SOLO" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Mute Button" id="3d8e1c8017e33a17" memberName="muteBtn"
              virtualName="" explicitFocusOrder="0" pos="2 732 71 20" bgColOff="ff025c02"
              bgColOn="ffff0000" buttonText="MUTE" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Record Button" id="150347c4cab05d88" memberName="recordBtn"
              virtualName="" explicitFocusOrder="0" pos="2 548 71 20" bgColOff="ff7a2e2e"
              bgColOn="ffff0000" buttonText="RECORD" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <COMBOBOX name="Channel Selector" id="e9d3111adc401b26" memberName="channelSelector"
            virtualName="" explicitFocusOrder="0" pos="2 524 71 20" editable="0"
            layout="36" items="" textWhenNonSelected="1" textWhenNoItems="(no choices)"/>
  <SLIDER name="Channel Aux 11/12 Pan" id="d62d026370f1137" memberName="aux11_12Pan"
          virtualName="" explicitFocusOrder="0" pos="16 500 54 8" bkgcol="ff0000ff"
          thumbcol="ff79d5fa" trackcol="ff0000ff" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-127.0" max="127.0" int="1.0"
          style="LinearHorizontal" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <COMBOBOX name="Post EQ Insert Selector" id="3dcefdff46227e00" memberName="postEqInsert"
            virtualName="" explicitFocusOrder="0" pos="2 316 71 20" editable="0"
            layout="36" items="Reset&#10;P01&#10;P02&#10;P03&#10;P04&#10;P05&#10;P06&#10;P07&#10;P08&#10;P09&#10;P10&#10;P11&#10;P12&#10;P13&#10;P14&#10;P15&#10;P16"
            textWhenNonSelected="POST" textWhenNoItems="(no choices)"/>
  <SLIDER name="Channel Aux 7 Send" id="7ddbb9dbfb38a869" memberName="aux7Send"
          virtualName="" explicitFocusOrder="0" pos="16 440 54 8" bkgcol="263238"
          thumbcol="ff79d5fa" trackcol="ffff0000" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-90.0" max="10.0" int="1.0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Channel Aux 11/12 Send" id="924d2821acc7b77" memberName="aux11_12Send"
          virtualName="" explicitFocusOrder="0" pos="16 488 54 8" bkgcol="263238"
          thumbcol="ff79d5fa" trackcol="ffffc900" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-90.0" max="10.0" int="1.0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Channel Aux 910 Pan" id="f9e1c2cb22dc7c85" memberName="aux9_10Pan"
          virtualName="" explicitFocusOrder="0" pos="16 476 54 8" bkgcol="ff0000ff"
          thumbcol="ff79d5fa" trackcol="ff0000ff" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-127.0" max="127.0" int="1.0"
          style="LinearHorizontal" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Channel Aux 910 Send" id="fc021387d2664c96" memberName="aux9_10Send"
          virtualName="" explicitFocusOrder="0" pos="16 464 54 8" bkgcol="263238"
          thumbcol="ff79d5fa" trackcol="ffffc900" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-90.0" max="10.0" int="1.0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Channel Aux 8 Send" id="2e287a269850ecb6" memberName="aux8Send"
          virtualName="" explicitFocusOrder="0" pos="16 452 54 8" bkgcol="263238"
          thumbcol="ff79d5fa" trackcol="ffff0000" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-90.0" max="10.0" int="1.0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Channel Aux 6 Send" id="d7069e36cca46f2a" memberName="aux6Send"
          virtualName="" explicitFocusOrder="0" pos="16 428 54 8" bkgcol="263238"
          thumbcol="ff79d5fa" trackcol="ffff0000" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-90.0" max="10.0" int="1.0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Channel Aux 4 Send" id="cd713329c8ef07e8" memberName="aux4Send"
          virtualName="" explicitFocusOrder="0" pos="16 404 54 8" bkgcol="263238"
          thumbcol="ff79d5fa" trackcol="ffff0000" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-90.0" max="10.0" int="1.0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Channel Aux 5 Send" id="60f56a5ac8063049" memberName="aux5Send"
          virtualName="" explicitFocusOrder="0" pos="16 416 54 8" bkgcol="263238"
          thumbcol="ff79d5fa" trackcol="ffff0000" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-90.0" max="10.0" int="1.0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Channel Aux 3 Send" id="b5c501523d09d1fc" memberName="aux3Send"
          virtualName="" explicitFocusOrder="0" pos="16 392 54 8" bkgcol="263238"
          thumbcol="ff79d5fa" trackcol="ffff0000" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-90.0" max="10.0" int="1.0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Channel Aux 2 Send" id="885e7046957aad3e" memberName="aux2Send"
          virtualName="" explicitFocusOrder="0" pos="16 380 54 8" bkgcol="263238"
          thumbcol="ff79d5fa" trackcol="ffff0000" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-90.0" max="10.0" int="1.0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Channel Aux 1 Send" id="efaafce429946e47" memberName="aux1Send"
          virtualName="" explicitFocusOrder="0" pos="16 368 54 8" bkgcol="263238"
          thumbcol="ff79d5fa" trackcol="ffff0000" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-90.0" max="10.0" int="1.0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="Post EQ Insert Label" id="d20db10e873d43a0" memberName="postEqInsertLabel"
         virtualName="" explicitFocusOrder="0" pos="2 292 71 20" bkgCol="ff140404"
         outlineCol="44d4c4c" edTextCol="ff000000" edBkgCol="0" labelText="P01"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="Channel Label" id="3a62e9f911e17b0e" memberName="chLabel"
         virtualName="" explicitFocusOrder="0" pos="2 764 71 20" outlineCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="" editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="place_holder_for_eq_show" id="79f8c2206c8d1951" memberName="eqThumbnail"
                    virtualName="" explicitFocusOrder="0" pos="2 244 71 40" class="juce::Component"
                    params=""/>
  <TEXTBUTTON name="Channel Phase Button" id="c4d9b782aab723df" memberName="phaseBtn"
              virtualName="" explicitFocusOrder="0" pos="6 220 30 20" bgColOff="ff808080"
              buttonText="&#216;" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Channel EQ Button" id="6f37ac521a7af099" memberName="eqBtn"
              virtualName="" explicitFocusOrder="0" pos="40 220 30 20" bgColOff="ff808080"
              buttonText="EQ" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Channel Compressor Button" id="c007952660d1821b" memberName="compressorBtn"
              virtualName="" explicitFocusOrder="0" pos="6 196 30 20" bgColOff="ff808080"
              buttonText="C" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Pre EQ Insert Label" id="b2e3c1642926f5b" memberName="preEqInsertLabel"
         virtualName="" explicitFocusOrder="0" pos="2 4 71 20" bkgCol="ff140404"
         outlineCol="44d4c4c" edTextCol="ff000000" edBkgCol="0" labelText="P01"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <COMBOBOX name="Pre EQ Insert Selector" id="23e2c5e0906a44ba" memberName="preEqInsert"
            virtualName="" explicitFocusOrder="0" pos="2 28 71 20" editable="0"
            layout="36" items="Reset&#10;P01&#10;P02&#10;P03&#10;P04&#10;P05&#10;P06&#10;P07&#10;P08&#10;P09&#10;P10&#10;P11&#10;P12&#10;P13&#10;P14&#10;P15&#10;P16"
            textWhenNonSelected="PRE" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="Channel Gate Button" id="baaf9005097506ff" memberName="gateBtn"
              virtualName="" explicitFocusOrder="0" pos="40 196 30 20" bgColOff="ff808080"
              buttonText="G" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="L/R Assignment Button" id="ee001f16bc6510a3" memberName="chAssignLRBtn"
              virtualName="" explicitFocusOrder="0" pos="2 124 71 14" bgColOff="ff2a8061"
              bgColOn="ffff0000" buttonText="L R" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Bus 7 Assign Button" id="fefa6cd5ab39b3a1" memberName="bus7AssignBtn"
              virtualName="" explicitFocusOrder="0" pos="2 108 34 14" bgColOff="ff2a8061"
              bgColOn="ffff0000" buttonText="7" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Bus 8 Assign Button" id="142feae195ad1da1" memberName="bus8AssignBtn"
              virtualName="" explicitFocusOrder="0" pos="40 108 34 14" bgColOff="ff2a8061"
              bgColOn="ffff0000" buttonText="8" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Bus 5 Assign Button" id="cabc1f0b689faa6" memberName="bus5AssignBtn"
              virtualName="" explicitFocusOrder="0" pos="2 92 34 14" bgColOff="ff2a8061"
              bgColOn="ffff0000" buttonText="5" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Bus 6 Assign Button" id="df5a2626ce5cb36a" memberName="bus6AssignBtn"
              virtualName="" explicitFocusOrder="0" pos="40 92 34 14" bgColOff="ff2a8061"
              bgColOn="ffff0000" buttonText="6" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Bus 3 Assign Button" id="264a15d69c5f9ecf" memberName="bus3AssignBtn"
              virtualName="" explicitFocusOrder="0" pos="2 76 34 14" bgColOff="ff2a8061"
              bgColOn="ffff0000" buttonText="3" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Bus 4 Assign Button" id="914ae30702302919" memberName="bus4AssignBtn"
              virtualName="" explicitFocusOrder="0" pos="40 76 34 14" bgColOff="ff2a8061"
              bgColOn="ffff0000" buttonText="4" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Bus 1 Assign Button" id="e19a2dd31939e1ad" memberName="bus1AssignBtn"
              virtualName="" explicitFocusOrder="0" pos="2 60 34 14" bgColOff="ff2a8061"
              bgColOn="ffff0000" buttonText="1" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Bus 2 Assign Button" id="42c6aff0d29e211a" memberName="bus2AssignBtn"
              virtualName="" explicitFocusOrder="0" pos="40 60 34 14" bgColOff="ff2a8061"
              bgColOn="ffff0000" buttonText="2" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <SLIDER name="Channel Digital Trim" id="92410009b87841a2" memberName="chDigitalTrim"
          virtualName="" explicitFocusOrder="0" pos="16 180 52 8" bkgcol="263238"
          thumbcol="ff79d5fa" trackcol="ff0000ff" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-90.0" max="10.0" int="1.0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <COMBOBOX name="Direct Out Assignment" id="9997680366c645d8" memberName="juce__comboBox4"
            virtualName="" explicitFocusOrder="0" pos="8 148 60 14" editable="0"
            layout="36" items=" - &#10;tape out 1 - (ch. 25)&#10;tape out 2 - (ch. 26)&#10;tape out 3 - (ch. 27)&#10;"
            textWhenNonSelected="-" textWhenNoItems="(no choices)"/>
  <SLIDER name="Channel Level To Tape" id="632a52cbbec2a341" memberName="chLevelToTape"
          virtualName="" explicitFocusOrder="0" pos="16 166 52 8" bkgcol="263238"
          thumbcol="ff79d5fa" trackcol="ffff0000" rotarysliderfill="ffff0000"
          textboxoutline="4d8e989b" min="-90.0" max="10.0" int="1.0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...

//[/EndFile]

