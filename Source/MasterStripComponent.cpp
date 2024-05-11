/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.11

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MasterStripComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MasterStripComponent::MasterStripComponent ()
    : eventBus(EventBus::getInstance()),
      faderValueLookup(FaderValueLookup::getInstance())
{
    //[Constructor_pre] You can add your own custom stuff here..
    // ################## MY CONSTRUCTOR STUFF ###################

    // // Add the callbacks to the eventBus
    // eventBus.masterStripComponentSubscribe(FADER_EVENT,
    //                                        [this](const std::string &valueString)
    //                                        { this->faderMoveEventCallback(valueString); });
    // eventBus.masterStripComponentSubscribe(VPOT_EVENT,
    //                                        [this](const std::string &valueString)
    //                                        { this->vpotTurnEventCallback(valueString); });
    // eventBus.masterStripComponentSubscribe(BUTTON_EVENT,
    //                                        [this](const std::string &valueString)
    //                                        { this->buttonEventCallback(valueString); });

    MasterUiFaderCallback masterFaderUpdateCallback = [this](int faderValue)
    {
        this->faderUpdateEventCallback(faderValue);
    };

    eventBus.masterUiFaderSubscribe(masterFaderUpdateCallback);



    // ##################### END OF MY CONSTRUCTOR STUFF ##################

    //[/Constructor_pre]

    masterFader.reset (new juce::Slider ("Master Fader"));
    addAndMakeVisible (masterFader.get());
    masterFader->setRange (-90, 10, 0.1);
    masterFader->setSliderStyle (juce::Slider::LinearVertical);
    masterFader->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    masterFader->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x008e989b));
    masterFader->addListener (this);
    masterFader->setSkewFactor (3);

    masterFader->setBounds (2, 788, 116, 230);

    masterVpot.reset (new juce::Slider ("Master Vpot"));
    addAndMakeVisible (masterVpot.get());
    masterVpot->setRange (-127, 127, 1);
    masterVpot->setSliderStyle (juce::Slider::Rotary);
    masterVpot->setTextBoxStyle (juce::Slider::TextBoxBelow, true, 88, 22);
    masterVpot->setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff181f22));
    masterVpot->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff131919));
    masterVpot->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0xff242627));
    masterVpot->addListener (this);

    masterVpot->setBounds (0, 583, 120, 88);

    masterInsertRLabel.reset (new juce::Label ("Master Insert Right Label",
                                               TRANS ("--\n")));
    addAndMakeVisible (masterInsertRLabel.get());
    masterInsertRLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    masterInsertRLabel->setJustificationType (juce::Justification::centred);
    masterInsertRLabel->setEditable (false, false, false);
    masterInsertRLabel->setColour (juce::Label::backgroundColourId, juce::Colour (0xff140404));
    masterInsertRLabel->setColour (juce::Label::outlineColourId, juce::Colour (0x044d4c4c));
    masterInsertRLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    masterInsertRLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    masterInsertRLabel->setBounds (16, 68, 88, 20);

    masterInsertR.reset (new juce::ComboBox ("Master Insert Right"));
    addAndMakeVisible (masterInsertR.get());
    masterInsertR->setTooltip (TRANS ("Allows any plugin to be patched into the main mix outputs.\n"
    "(Post fader insert)"));
    masterInsertR->setEditableText (false);
    masterInsertR->setJustificationType (juce::Justification::centred);
    masterInsertR->setTextWhenNothingSelected (TRANS ("INSERT R"));
    masterInsertR->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    masterInsertR->addItem (TRANS ("Reset"), 1);
    masterInsertR->addItem (TRANS ("P01"), 2);
    masterInsertR->addItem (TRANS ("P02"), 3);
    masterInsertR->addItem (TRANS ("P03"), 4);
    masterInsertR->addItem (TRANS ("P04"), 5);
    masterInsertR->addItem (TRANS ("P05"), 6);
    masterInsertR->addItem (TRANS ("P06"), 7);
    masterInsertR->addItem (TRANS ("P07"), 8);
    masterInsertR->addItem (TRANS ("P08"), 9);
    masterInsertR->addItem (TRANS ("P09"), 10);
    masterInsertR->addItem (TRANS ("P10"), 11);
    masterInsertR->addItem (TRANS ("P11"), 12);
    masterInsertR->addItem (TRANS ("P12"), 13);
    masterInsertR->addItem (TRANS ("P13"), 14);
    masterInsertR->addItem (TRANS ("P14"), 15);
    masterInsertR->addItem (TRANS ("P15"), 16);
    masterInsertR->addItem (TRANS ("P16"), 17);
    masterInsertR->addListener (this);

    masterInsertR->setBounds (16, 94, 88, 18);

    mastersBankBtn.reset (new juce::TextButton ("Masters Bank Button"));
    addAndMakeVisible (mastersBankBtn.get());
    mastersBankBtn->setButtonText (TRANS ("Masters"));
    mastersBankBtn->addListener (this);
    mastersBankBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff3d3f4b));
    mastersBankBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffc8c892));

    mastersBankBtn->setBounds (16, 764, 88, 20);

    effectsBankBtn.reset (new juce::TextButton ("Effects Bank Button"));
    addAndMakeVisible (effectsBankBtn.get());
    effectsBankBtn->setButtonText (TRANS ("Effects"));
    effectsBankBtn->addListener (this);
    effectsBankBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff3d3f4b));
    effectsBankBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffc8c892));

    effectsBankBtn->setBounds (16, 738, 88, 20);

    tapeBanksBtn.reset (new juce::TextButton ("Tape Bank Button"));
    addAndMakeVisible (tapeBanksBtn.get());
    tapeBanksBtn->setButtonText (TRANS ("Tape"));
    tapeBanksBtn->addListener (this);
    tapeBanksBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff3d3f4b));
    tapeBanksBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffc8c892));

    tapeBanksBtn->setBounds (16, 712, 88, 20);

    lineBanksBtn.reset (new juce::TextButton ("Line Bank Button"));
    addAndMakeVisible (lineBanksBtn.get());
    lineBanksBtn->setButtonText (TRANS ("Line"));
    lineBanksBtn->addListener (this);
    lineBanksBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff3d3f4b));
    lineBanksBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffc8c892));

    lineBanksBtn->setBounds (16, 686, 88, 20);

    panMasterBtn.reset (new juce::TextButton ("Pan Master Button"));
    addAndMakeVisible (panMasterBtn.get());
    panMasterBtn->setTooltip (TRANS ("V-Pot set to function as front/rear pan position control for any selected channel in surround modes"));
    panMasterBtn->setButtonText (TRANS ("PAN"));
    panMasterBtn->addListener (this);
    panMasterBtn->setColour (juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);

    panMasterBtn->setBounds (8, 560, 46, 18);

    soloMasterBtn.reset (new juce::TextButton ("Solo Master Btn"));
    addAndMakeVisible (soloMasterBtn.get());
    soloMasterBtn->setTooltip (TRANS ("Solos any selected Aux bus"));
    soloMasterBtn->setButtonText (TRANS ("SOLO"));
    soloMasterBtn->addListener (this);
    soloMasterBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff90883a));

    soloMasterBtn->setBounds (66, 560, 46, 18);

    cuePan1Btn.reset (new juce::TextButton ("Cue Pan 1 Button"));
    addAndMakeVisible (cuePan1Btn.get());
    cuePan1Btn->setTooltip (TRANS ("Set channel Vpots to control aux 9/10 pan\n"));
    cuePan1Btn->setButtonText (TRANS ("1"));
    cuePan1Btn->addListener (this);
    cuePan1Btn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    cuePan1Btn->setBounds (8, 526, 46, 18);

    cuePan2Btn.reset (new juce::TextButton ("Cue Pan 2 Button"));
    addAndMakeVisible (cuePan2Btn.get());
    cuePan2Btn->setTooltip (TRANS ("Set channel Vpots to control aux 11/12 pan"));
    cuePan2Btn->setButtonText (TRANS ("2"));
    cuePan2Btn->addListener (this);
    cuePan2Btn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    cuePan2Btn->setBounds (66, 526, 46, 18);

    cueLvl1Btn.reset (new juce::TextButton ("Cue Level 1 Button"));
    addAndMakeVisible (cueLvl1Btn.get());
    cueLvl1Btn->setTooltip (TRANS ("Set channel Vpots to control aux 9/10 level"));
    cueLvl1Btn->setButtonText (TRANS ("1"));
    cueLvl1Btn->addListener (this);
    cueLvl1Btn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    cueLvl1Btn->setBounds (8, 480, 46, 18);

    cueLvl2Btn.reset (new juce::TextButton ("Cue Level 2 Button"));
    addAndMakeVisible (cueLvl2Btn.get());
    cueLvl2Btn->setTooltip (TRANS ("Vpots to control aux 9/10 level"));
    cueLvl2Btn->setButtonText (TRANS ("2"));
    cueLvl2Btn->addListener (this);
    cueLvl2Btn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    cueLvl2Btn->setBounds (66, 480, 46, 18);

    aux1LevelsBtn.reset (new juce::TextButton ("Aux 1 Levels Button"));
    addAndMakeVisible (aux1LevelsBtn.get());
    aux1LevelsBtn->setTooltip (TRANS ("Sets channel Vpots to control AUX 1 send level.\n"
    "Master Vpot controls AUX out level"));
    aux1LevelsBtn->setButtonText (TRANS ("1"));
    aux1LevelsBtn->addListener (this);
    aux1LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux1LevelsBtn->setBounds (8, 350, 46, 18);

    aux2LevelsBtn.reset (new juce::TextButton ("Aux 2 Levels Button"));
    addAndMakeVisible (aux2LevelsBtn.get());
    aux2LevelsBtn->setTooltip (TRANS ("Sets channel Vpots to control AUX 2 send level.\n"
    "Master Vpot controls AUX out level"));
    aux2LevelsBtn->setButtonText (TRANS ("2"));
    aux2LevelsBtn->addListener (this);
    aux2LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux2LevelsBtn->setBounds (66, 350, 46, 18);

    aux3LevelsBtn.reset (new juce::TextButton ("Aux 3 Levels Button"));
    addAndMakeVisible (aux3LevelsBtn.get());
    aux3LevelsBtn->setTooltip (TRANS ("Sets channel Vpots to control AUX 3 send level.\n"
    "Master Vpot controls AUX out level"));
    aux3LevelsBtn->setButtonText (TRANS ("3"));
    aux3LevelsBtn->addListener (this);
    aux3LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux3LevelsBtn->setBounds (8, 376, 46, 18);

    aux4LevelsBtn.reset (new juce::TextButton ("Aux 4 Levels Button"));
    addAndMakeVisible (aux4LevelsBtn.get());
    aux4LevelsBtn->setTooltip (TRANS ("Sets channel Vpots to control AUX 4 send level.\n"
    "Master Vpot controls AUX out level"));
    aux4LevelsBtn->setButtonText (TRANS ("4"));
    aux4LevelsBtn->addListener (this);
    aux4LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux4LevelsBtn->setBounds (66, 376, 46, 18);

    aux5LevelsBtn.reset (new juce::TextButton ("Aux 5 Levels Button"));
    addAndMakeVisible (aux5LevelsBtn.get());
    aux5LevelsBtn->setTooltip (TRANS ("Sets channel Vpots to control AUX 5 send level.\n"
    "Master Vpot controls AUX out level"));
    aux5LevelsBtn->setButtonText (TRANS ("5"));
    aux5LevelsBtn->addListener (this);
    aux5LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux5LevelsBtn->setBounds (8, 402, 46, 18);

    aux6LevelsBtn.reset (new juce::TextButton ("Aux 6 Levels Button"));
    addAndMakeVisible (aux6LevelsBtn.get());
    aux6LevelsBtn->setTooltip (TRANS ("Sets channel Vpots to control AUX 6 send level.\n"
    "Master Vpot controls AUX out level"));
    aux6LevelsBtn->setButtonText (TRANS ("6"));
    aux6LevelsBtn->addListener (this);
    aux6LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux6LevelsBtn->setBounds (66, 402, 46, 18);

    aux7LevelsBtn.reset (new juce::TextButton ("Aux 7 Levels Button"));
    addAndMakeVisible (aux7LevelsBtn.get());
    aux7LevelsBtn->setTooltip (TRANS ("Sets channel Vpots to control AUX 7 send level.\n"
    "Master Vpot controls AUX out level"));
    aux7LevelsBtn->setButtonText (TRANS ("7"));
    aux7LevelsBtn->addListener (this);
    aux7LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux7LevelsBtn->setBounds (8, 428, 46, 18);

    aux8LevelsBtn.reset (new juce::TextButton ("Aux 8 Levels Button"));
    addAndMakeVisible (aux8LevelsBtn.get());
    aux8LevelsBtn->setTooltip (TRANS ("Sets channel Vpots to control AUX 8 send level.\n"
    "Master Vpot controls AUX out level"));
    aux8LevelsBtn->setButtonText (TRANS ("8"));
    aux8LevelsBtn->addListener (this);
    aux8LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux8LevelsBtn->setBounds (66, 428, 46, 18);

    digitalTrimBtn.reset (new juce::TextButton ("Digital Trim Button"));
    addAndMakeVisible (digitalTrimBtn.get());
    digitalTrimBtn->setTooltip (TRANS ("Sets channel Vpots to control digital input level (pre DSP)"));
    digitalTrimBtn->setButtonText (TRANS ("TRIM"));
    digitalTrimBtn->addListener (this);
    digitalTrimBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    digitalTrimBtn->setBounds (20, 306, 80, 18);

    trackingLvlBtn.reset (new juce::TextButton ("Tracking Level Button"));
    addAndMakeVisible (trackingLvlBtn.get());
    trackingLvlBtn->setTooltip (TRANS ("Sets Channel Vpots to control final post-DSP output level to multitrack outputs."));
    trackingLvlBtn->setButtonText (TRANS ("LEVEL"));
    trackingLvlBtn->addListener (this);
    trackingLvlBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    trackingLvlBtn->setBounds (20, 262, 80, 18);

    metersTopBtn.reset (new juce::TextButton ("Meters On Top Button"));
    addAndMakeVisible (metersTopBtn.get());
    metersTopBtn->setButtonText (TRANS ("TOP"));
    metersTopBtn->addListener (this);
    metersTopBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff172c52));

    metersTopBtn->setBounds (8, 216, 46, 18);

    metersBotmBtn.reset (new juce::TextButton ("Meters On Bottom Button"));
    addAndMakeVisible (metersBotmBtn.get());
    metersBotmBtn->setButtonText (TRANS ("BOTM"));
    metersBotmBtn->addListener (this);
    metersBotmBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff172c52));

    metersBotmBtn->setBounds (66, 216, 46, 18);

    masterSelectBtn.reset (new juce::TextButton ("Master Select Button"));
    addAndMakeVisible (masterSelectBtn.get());
    masterSelectBtn->setTooltip (TRANS ("Activates Master fader in the Fat Channel Display."));
    masterSelectBtn->setButtonText (TRANS ("SELECT"));
    masterSelectBtn->addListener (this);
    masterSelectBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff189994));

    masterSelectBtn->setBounds (20, 142, 80, 18);

    masterWriteBtn.reset (new juce::TextButton ("Master Write Button"));
    addAndMakeVisible (masterWriteBtn.get());
    masterWriteBtn->setTooltip (TRANS ("Activates the Master Strip to record selected automation data by the Main fader og Aux Masters"));
    masterWriteBtn->setButtonText (TRANS ("WRITE"));
    masterWriteBtn->addListener (this);
    masterWriteBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff565555));

    masterWriteBtn->setBounds (20, 168, 80, 18);

    masterInsertRLabel2.reset (new juce::Label ("Master Insert Right Label",
                                                TRANS ("--\n")));
    addAndMakeVisible (masterInsertRLabel2.get());
    masterInsertRLabel2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    masterInsertRLabel2->setJustificationType (juce::Justification::centred);
    masterInsertRLabel2->setEditable (false, false, false);
    masterInsertRLabel2->setColour (juce::Label::backgroundColourId, juce::Colour (0xff140404));
    masterInsertRLabel2->setColour (juce::Label::outlineColourId, juce::Colour (0x044d4c4c));
    masterInsertRLabel2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    masterInsertRLabel2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    masterInsertRLabel2->setBounds (14, 12, 88, 20);

    masterInsertL.reset (new juce::ComboBox ("Master Insert Left"));
    addAndMakeVisible (masterInsertL.get());
    masterInsertL->setTooltip (TRANS ("Allows any plugin to be patched into the main mix outputs.\n"
    "(Post fader insert)"));
    masterInsertL->setEditableText (false);
    masterInsertL->setJustificationType (juce::Justification::centred);
    masterInsertL->setTextWhenNothingSelected (TRANS ("INSERT L"));
    masterInsertL->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    masterInsertL->addItem (TRANS ("Reset"), 1);
    masterInsertL->addItem (TRANS ("P01"), 2);
    masterInsertL->addItem (TRANS ("P02"), 3);
    masterInsertL->addItem (TRANS ("P03"), 4);
    masterInsertL->addItem (TRANS ("P04"), 5);
    masterInsertL->addItem (TRANS ("P05"), 6);
    masterInsertL->addItem (TRANS ("P06"), 7);
    masterInsertL->addItem (TRANS ("P07"), 8);
    masterInsertL->addItem (TRANS ("P08"), 9);
    masterInsertL->addItem (TRANS ("P09"), 10);
    masterInsertL->addItem (TRANS ("P10"), 11);
    masterInsertL->addItem (TRANS ("P11"), 12);
    masterInsertL->addItem (TRANS ("P12"), 13);
    masterInsertL->addItem (TRANS ("P13"), 14);
    masterInsertL->addItem (TRANS ("P14"), 15);
    masterInsertL->addItem (TRANS ("P15"), 16);
    masterInsertL->addItem (TRANS ("P16"), 17);
    masterInsertL->addListener (this);

    masterInsertL->setBounds (14, 38, 88, 18);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MasterStripComponent::~MasterStripComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    masterFader = nullptr;
    masterVpot = nullptr;
    masterInsertRLabel = nullptr;
    masterInsertR = nullptr;
    mastersBankBtn = nullptr;
    effectsBankBtn = nullptr;
    tapeBanksBtn = nullptr;
    lineBanksBtn = nullptr;
    panMasterBtn = nullptr;
    soloMasterBtn = nullptr;
    cuePan1Btn = nullptr;
    cuePan2Btn = nullptr;
    cueLvl1Btn = nullptr;
    cueLvl2Btn = nullptr;
    aux1LevelsBtn = nullptr;
    aux2LevelsBtn = nullptr;
    aux3LevelsBtn = nullptr;
    aux4LevelsBtn = nullptr;
    aux5LevelsBtn = nullptr;
    aux6LevelsBtn = nullptr;
    aux7LevelsBtn = nullptr;
    aux8LevelsBtn = nullptr;
    digitalTrimBtn = nullptr;
    trackingLvlBtn = nullptr;
    metersTopBtn = nullptr;
    metersBotmBtn = nullptr;
    masterSelectBtn = nullptr;
    masterWriteBtn = nullptr;
    masterInsertRLabel2 = nullptr;
    masterInsertL = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MasterStripComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = 0, y = 0, width = 120, height = 1024;
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
        int x = 7, y = 846, width = 59, height = 8;
        juce::String text (TRANS ("U -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 893, width = 65, height = 8;
        juce::String text (TRANS ("10 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 934, width = 65, height = 8;
        juce::String text (TRANS ("30 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 956, width = 65, height = 8;
        juce::String text (TRANS ("40 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 870, width = 65, height = 8;
        juce::String text (TRANS ("5 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 827, width = 65, height = 8;
        juce::String text (TRANS ("5 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 803, width = 65, height = 8;
        juce::String text (TRANS ("10 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 5, y = 914, width = 65, height = 8;
        juce::String text (TRANS ("20 -"));
        juce::Colour fillColour = juce::Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 506, width = 120, height = 20;
        juce::String text (TRANS ("CUE PAN"));
        juce::Colour fillColour = juce::Colour (0xffa4a2a2);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = -4, y = 460, width = 120, height = 20;
        juce::String text (TRANS ("CUE LEVEL"));
        juce::Colour fillColour = juce::Colour (0xffa4a2a2);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 330, width = 120, height = 20;
        juce::String text (TRANS ("AUXES"));
        juce::Colour fillColour = juce::Colour (0xffa4a2a2);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 286, width = 120, height = 20;
        juce::String text (TRANS ("DIGITAL"));
        juce::Colour fillColour = juce::Colour (0xffa4a2a2);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 242, width = 120, height = 20;
        juce::String text (TRANS ("TRACKING"));
        juce::Colour fillColour = juce::Colour (0xffa4a2a2);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 196, width = 120, height = 20;
        juce::String text (TRANS ("METERS"));
        juce::Colour fillColour = juce::Colour (0xffa4a2a2);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 0, y = 240, width = 120, height = 312;
        juce::Colour strokeColour = juce::Colour (0x50060b15);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 0, y = 118, width = 120, height = 78;
        juce::Colour strokeColour = juce::Colour (0x50060b15);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 0, y = 122, width = 120, height = 20;
        juce::String text (TRANS ("MASTER"));
        juce::Colour fillColour = juce::Colour (0xffa4a2a2);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MasterStripComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MasterStripComponent::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == masterFader.get())
    {
        //[UserSliderCode_masterFader] -- add your slider handling code here..

        // Fader was moved in the UI.
        // float newMasterFaderValue = std::round(sliderThatWasMoved->getValue() * 10.0f) / 10.0f;
        // std::string dspMasterFaderValue = faderValueLookup.dspHexLookupMap[newMasterFaderValue];

        const char * faderHexString = faderValueLookup.getDspHexValue(sliderThatWasMoved->getValue());
        char eventValue[2] = {faderHexString[0], faderHexString[1]};


        // Use event post (hardcoded strip ID - master is 18)
        eventBus.postFaderEvent(static_cast<ChStripID>(MASTER_CH_STRIP), eventValue, UI_EVENT);
        //eventBus.postEvent(FADER_EVENT, "18", dspMasterFaderValue, UI_EVENT);

        //[/UserSliderCode_masterFader]
    }
    else if (sliderThatWasMoved == masterVpot.get())
    {
        //[UserSliderCode_masterVpot] -- add your slider handling code here..
        //[/UserSliderCode_masterVpot]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void MasterStripComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == masterInsertR.get())
    {
        //[UserComboBoxCode_masterInsertR] -- add your combo box handling code here..
        //[/UserComboBoxCode_masterInsertR]
    }
    else if (comboBoxThatHasChanged == masterInsertL.get())
    {
        //[UserComboBoxCode_masterInsertL] -- add your combo box handling code here..
        //[/UserComboBoxCode_masterInsertL]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void MasterStripComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == mastersBankBtn.get())
    {
        //[UserButtonCode_mastersBankBtn] -- add your button handler code here..
        //[/UserButtonCode_mastersBankBtn]
    }
    else if (buttonThatWasClicked == effectsBankBtn.get())
    {
        //[UserButtonCode_effectsBankBtn] -- add your button handler code here..
        //[/UserButtonCode_effectsBankBtn]
    }
    else if (buttonThatWasClicked == tapeBanksBtn.get())
    {
        //[UserButtonCode_tapeBanksBtn] -- add your button handler code here..
        //[/UserButtonCode_tapeBanksBtn]
    }
    else if (buttonThatWasClicked == lineBanksBtn.get())
    {
        //[UserButtonCode_lineBanksBtn] -- add your button handler code here..
        //[/UserButtonCode_lineBanksBtn]
    }
    else if (buttonThatWasClicked == panMasterBtn.get())
    {
        //[UserButtonCode_panMasterBtn] -- add your button handler code here..
        //[/UserButtonCode_panMasterBtn]
    }
    else if (buttonThatWasClicked == soloMasterBtn.get())
    {
        //[UserButtonCode_soloMasterBtn] -- add your button handler code here..
        //[/UserButtonCode_soloMasterBtn]
    }
    else if (buttonThatWasClicked == cuePan1Btn.get())
    {
        //[UserButtonCode_cuePan1Btn] -- add your button handler code here..
        //[/UserButtonCode_cuePan1Btn]
    }
    else if (buttonThatWasClicked == cuePan2Btn.get())
    {
        //[UserButtonCode_cuePan2Btn] -- add your button handler code here..
        //[/UserButtonCode_cuePan2Btn]
    }
    else if (buttonThatWasClicked == cueLvl1Btn.get())
    {
        //[UserButtonCode_cueLvl1Btn] -- add your button handler code here..
        //[/UserButtonCode_cueLvl1Btn]
    }
    else if (buttonThatWasClicked == cueLvl2Btn.get())
    {
        //[UserButtonCode_cueLvl2Btn] -- add your button handler code here..
        //[/UserButtonCode_cueLvl2Btn]
    }
    else if (buttonThatWasClicked == aux1LevelsBtn.get())
    {
        //[UserButtonCode_aux1LevelsBtn] -- add your button handler code here..
        //[/UserButtonCode_aux1LevelsBtn]
    }
    else if (buttonThatWasClicked == aux2LevelsBtn.get())
    {
        //[UserButtonCode_aux2LevelsBtn] -- add your button handler code here..
        //[/UserButtonCode_aux2LevelsBtn]
    }
    else if (buttonThatWasClicked == aux3LevelsBtn.get())
    {
        //[UserButtonCode_aux3LevelsBtn] -- add your button handler code here..
        //[/UserButtonCode_aux3LevelsBtn]
    }
    else if (buttonThatWasClicked == aux4LevelsBtn.get())
    {
        //[UserButtonCode_aux4LevelsBtn] -- add your button handler code here..
        //[/UserButtonCode_aux4LevelsBtn]
    }
    else if (buttonThatWasClicked == aux5LevelsBtn.get())
    {
        //[UserButtonCode_aux5LevelsBtn] -- add your button handler code here..
        //[/UserButtonCode_aux5LevelsBtn]
    }
    else if (buttonThatWasClicked == aux6LevelsBtn.get())
    {
        //[UserButtonCode_aux6LevelsBtn] -- add your button handler code here..
        //[/UserButtonCode_aux6LevelsBtn]
    }
    else if (buttonThatWasClicked == aux7LevelsBtn.get())
    {
        //[UserButtonCode_aux7LevelsBtn] -- add your button handler code here..
        //[/UserButtonCode_aux7LevelsBtn]
    }
    else if (buttonThatWasClicked == aux8LevelsBtn.get())
    {
        //[UserButtonCode_aux8LevelsBtn] -- add your button handler code here..
        //[/UserButtonCode_aux8LevelsBtn]
    }
    else if (buttonThatWasClicked == digitalTrimBtn.get())
    {
        //[UserButtonCode_digitalTrimBtn] -- add your button handler code here..
        //[/UserButtonCode_digitalTrimBtn]
    }
    else if (buttonThatWasClicked == trackingLvlBtn.get())
    {
        //[UserButtonCode_trackingLvlBtn] -- add your button handler code here..
        //[/UserButtonCode_trackingLvlBtn]
    }
    else if (buttonThatWasClicked == metersTopBtn.get())
    {
        //[UserButtonCode_metersTopBtn] -- add your button handler code here..
        //[/UserButtonCode_metersTopBtn]
    }
    else if (buttonThatWasClicked == metersBotmBtn.get())
    {
        //[UserButtonCode_metersBotmBtn] -- add your button handler code here..
        //[/UserButtonCode_metersBotmBtn]
    }
    else if (buttonThatWasClicked == masterSelectBtn.get())
    {
        //[UserButtonCode_masterSelectBtn] -- add your button handler code here..
        //[/UserButtonCode_masterSelectBtn]
    }
    else if (buttonThatWasClicked == masterWriteBtn.get())
    {
        //[UserButtonCode_masterWriteBtn] -- add your button handler code here..
        //[/UserButtonCode_masterWriteBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MasterStripComponent::setMasterFaderPosition(double value)
{
    juce::MessageManager::callAsync([this, value]()
                                    { masterFader.get()->setValue(value, juce::dontSendNotification); });
}

// void MasterStripComponent::setMasterFaderMoveCallbackFunction(std::function<void(float)> callbackFunction)
// {
// 	masterFaderMoveCallback = callbackFunction;
// }


/*********************************************************************************
 * @brief This is a callback used by the eventbus, after af physical master
 *        fader move has been handled by the MasterChannel, and all that is left
 *        is to update the ui.
 *        It uses some lookups to convert the provided 2-char hex value to the
 *        log10 decibel value for the UI fader.
 *
 * @param faderValue
 ********************************************************************************/
void MasterStripComponent::faderUpdateEventCallback(int faderValue)
{
    printf("all  the way up in master strip component...");
    //int decimalValue = hexToIntLookup.hexToInt(faderHexValue);
    //int decimalValue = std::stoi(faderHexValue, nullptr, 16);
    float logValue = *faderValueLookup.getLog10Value(faderValue);
    //float logValue = faderValueLookup.precomputedLog10Values[decimalValue];
    juce::MessageManager::callAsync([this, logValue]()
                                    { masterFader.get()->setValue(logValue, juce::dontSendNotification); });
}

// ####################################################
// Same as above, this just updates the VPot in the UI. This value should probably already be calculated
// in the MasterChannel class?
// ####################################################
void MasterStripComponent::vpotTurnEventCallback(std::string vpotHexValue)
{
    // TODO: Check what value we would get here....

    // int decimalValue = std::stoi(vpotHexValue, nullptr, 16);
    // float logValue = faderValueLookup.precomputedLog10Values[decimalValue];
    // juce::MessageManager::callAsync([this, logValue]()
    //                                 { masterVpot.get()->setValue(logValue, juce::dontSendNotification); });
}

// #####################################################################################################
// This is the callback for a button event. This might be somewhat different than a button event,
// although this is primarily for updating the UI, after the Channel class has handled the button press.
// #####################################################################################################
void MasterStripComponent::buttonEventCallback(std::string buttonValue)
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

<JUCER_COMPONENT documentType="Component" className="MasterStripComponent" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers="eventBus(EventBus::getInstance()),&#10;faderValueLookup(FaderValueLookup::getInstance())"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 120 1024" fill="solid: ff294e55" hasStroke="1" stroke="0.9, mitered, butt"
          strokeColour="solid: ff1c0b07"/>
    <TEXT pos="7 846 59 8" fill="solid: fff0f8ff" hasStroke="0" text="U -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 893 65 8" fill="solid: fff0f8ff" hasStroke="0" text="10 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 934 65 8" fill="solid: fff0f8ff" hasStroke="0" text="30 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 956 65 8" fill="solid: fff0f8ff" hasStroke="0" text="40 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 870 65 8" fill="solid: fff0f8ff" hasStroke="0" text="5 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 827 65 8" fill="solid: fff0f8ff" hasStroke="0" text="5 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 803 65 8" fill="solid: fff0f8ff" hasStroke="0" text="10 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="5 914 65 8" fill="solid: fff0f8ff" hasStroke="0" text="20 -"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 506 120 20" fill="solid: ffa4a2a2" hasStroke="0" text="CUE PAN"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="-4 460 120 20" fill="solid: ffa4a2a2" hasStroke="0" text="CUE LEVEL"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 330 120 20" fill="solid: ffa4a2a2" hasStroke="0" text="AUXES"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 286 120 20" fill="solid: ffa4a2a2" hasStroke="0" text="DIGITAL"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 242 120 20" fill="solid: ffa4a2a2" hasStroke="0" text="TRACKING"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="0 196 120 20" fill="solid: ffa4a2a2" hasStroke="0" text="METERS"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <RECT pos="0 240 120 312" fill="solid: 872aa5" hasStroke="1" stroke="1.4, mitered, butt"
          strokeColour="solid: 50060b15"/>
    <RECT pos="0 118 120 78" fill="solid: 872aa5" hasStroke="1" stroke="1.4, mitered, butt"
          strokeColour="solid: 50060b15"/>
    <TEXT pos="0 122 120 20" fill="solid: ffa4a2a2" hasStroke="0" text="MASTER"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="Master Fader" id="628ad89d92251169" memberName="masterFader"
          virtualName="" explicitFocusOrder="0" pos="2 788 116 230" textboxoutline="8e989b"
          min="-90.0" max="10.0" int="0.1" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="50" textBoxHeight="20" skewFactor="3.0"
          needsCallback="1"/>
  <SLIDER name="Master Vpot" id="8303a7c33dfa3daf" memberName="masterVpot"
          virtualName="" explicitFocusOrder="0" pos="0 583 120 88" rotaryslideroutline="ff181f22"
          textboxbkgd="ff131919" textboxoutline="ff242627" min="-127.0"
          max="127.0" int="1.0" style="Rotary" textBoxPos="TextBoxBelow"
          textBoxEditable="0" textBoxWidth="88" textBoxHeight="22" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Master Insert Right Label" id="b2e3c1642926f5b" memberName="masterInsertRLabel"
         virtualName="" explicitFocusOrder="0" pos="16 68 88 20" bkgCol="ff140404"
         outlineCol="44d4c4c" edTextCol="ff000000" edBkgCol="0" labelText="--&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <COMBOBOX name="Master Insert Right" id="23e2c5e0906a44ba" memberName="masterInsertR"
            virtualName="" explicitFocusOrder="0" pos="16 94 88 18" tooltip="Allows any plugin to be patched into the main mix outputs.&#10;(Post fader insert)"
            editable="0" layout="36" items="Reset&#10;P01&#10;P02&#10;P03&#10;P04&#10;P05&#10;P06&#10;P07&#10;P08&#10;P09&#10;P10&#10;P11&#10;P12&#10;P13&#10;P14&#10;P15&#10;P16"
            textWhenNonSelected="INSERT R" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="Masters Bank Button" id="6d334524922ac73a" memberName="mastersBankBtn"
              virtualName="" explicitFocusOrder="0" pos="16 764 88 20" bgColOff="ff3d3f4b"
              bgColOn="ffc8c892" buttonText="Masters" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Effects Bank Button" id="e87dbcbf5a700aa9" memberName="effectsBankBtn"
              virtualName="" explicitFocusOrder="0" pos="16 738 88 20" bgColOff="ff3d3f4b"
              bgColOn="ffc8c892" buttonText="Effects" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Tape Bank Button" id="81d8be7b3bdcb061" memberName="tapeBanksBtn"
              virtualName="" explicitFocusOrder="0" pos="16 712 88 20" bgColOff="ff3d3f4b"
              bgColOn="ffc8c892" buttonText="Tape" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Line Bank Button" id="e5d0d1771b8f7c35" memberName="lineBanksBtn"
              virtualName="" explicitFocusOrder="0" pos="16 686 88 20" bgColOff="ff3d3f4b"
              bgColOn="ffc8c892" buttonText="Line" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Pan Master Button" id="aee37e87e43cb4ee" memberName="panMasterBtn"
              virtualName="" explicitFocusOrder="0" pos="8 560 46 18" tooltip="V-Pot set to function as front/rear pan position control for any selected channel in surround modes"
              bgColOff="ff6495ed" buttonText="PAN" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Solo Master Btn" id="fd11192cc8e53d91" memberName="soloMasterBtn"
              virtualName="" explicitFocusOrder="0" pos="66 560 46 18" tooltip="Solos any selected Aux bus"
              bgColOff="ff90883a" buttonText="SOLO" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Cue Pan 1 Button" id="e0923bc150a3e29d" memberName="cuePan1Btn"
              virtualName="" explicitFocusOrder="0" pos="8 526 46 18" tooltip="Set channel Vpots to control aux 9/10 pan&#10;"
              bgColOff="ff184699" buttonText="1" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Cue Pan 2 Button" id="1bfda75b40556c32" memberName="cuePan2Btn"
              virtualName="" explicitFocusOrder="0" pos="66 526 46 18" tooltip="Set channel Vpots to control aux 11/12 pan"
              bgColOff="ff184699" buttonText="2" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Cue Level 1 Button" id="e501157e291996c0" memberName="cueLvl1Btn"
              virtualName="" explicitFocusOrder="0" pos="8 480 46 18" tooltip="Set channel Vpots to control aux 9/10 level"
              bgColOff="ff184699" buttonText="1" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Cue Level 2 Button" id="e328fe8aae21e6fd" memberName="cueLvl2Btn"
              virtualName="" explicitFocusOrder="0" pos="66 480 46 18" tooltip="Vpots to control aux 9/10 level"
              bgColOff="ff184699" buttonText="2" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Aux 1 Levels Button" id="1bc96ddb12208b35" memberName="aux1LevelsBtn"
              virtualName="" explicitFocusOrder="0" pos="8 350 46 18" tooltip="Sets channel Vpots to control AUX 1 send level.&#10;Master Vpot controls AUX out level"
              bgColOff="ff184699" buttonText="1" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Aux 2 Levels Button" id="e06cec7dc736c67f" memberName="aux2LevelsBtn"
              virtualName="" explicitFocusOrder="0" pos="66 350 46 18" tooltip="Sets channel Vpots to control AUX 2 send level.&#10;Master Vpot controls AUX out level"
              bgColOff="ff184699" buttonText="2" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Aux 3 Levels Button" id="cf733655d958a8ac" memberName="aux3LevelsBtn"
              virtualName="" explicitFocusOrder="0" pos="8 376 46 18" tooltip="Sets channel Vpots to control AUX 3 send level.&#10;Master Vpot controls AUX out level"
              bgColOff="ff184699" buttonText="3" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Aux 4 Levels Button" id="7fadaacbe6afdd58" memberName="aux4LevelsBtn"
              virtualName="" explicitFocusOrder="0" pos="66 376 46 18" tooltip="Sets channel Vpots to control AUX 4 send level.&#10;Master Vpot controls AUX out level"
              bgColOff="ff184699" buttonText="4" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Aux 5 Levels Button" id="7ae5e96a85957516" memberName="aux5LevelsBtn"
              virtualName="" explicitFocusOrder="0" pos="8 402 46 18" tooltip="Sets channel Vpots to control AUX 5 send level.&#10;Master Vpot controls AUX out level"
              bgColOff="ff184699" buttonText="5" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Aux 6 Levels Button" id="72b8d1a5a7b9e551" memberName="aux6LevelsBtn"
              virtualName="" explicitFocusOrder="0" pos="66 402 46 18" tooltip="Sets channel Vpots to control AUX 6 send level.&#10;Master Vpot controls AUX out level"
              bgColOff="ff184699" buttonText="6" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Aux 7 Levels Button" id="e20c98c2a1e1a4b1" memberName="aux7LevelsBtn"
              virtualName="" explicitFocusOrder="0" pos="8 428 46 18" tooltip="Sets channel Vpots to control AUX 7 send level.&#10;Master Vpot controls AUX out level"
              bgColOff="ff184699" buttonText="7" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Aux 8 Levels Button" id="61ad7f84bb5723ed" memberName="aux8LevelsBtn"
              virtualName="" explicitFocusOrder="0" pos="66 428 46 18" tooltip="Sets channel Vpots to control AUX 8 send level.&#10;Master Vpot controls AUX out level"
              bgColOff="ff184699" buttonText="8" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Digital Trim Button" id="ca75ae8b790d0f24" memberName="digitalTrimBtn"
              virtualName="" explicitFocusOrder="0" pos="20 306 80 18" tooltip="Sets channel Vpots to control digital input level (pre DSP)"
              bgColOff="ff184699" buttonText="TRIM" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Tracking Level Button" id="df0c02cc8086d813" memberName="trackingLvlBtn"
              virtualName="" explicitFocusOrder="0" pos="20 262 80 18" tooltip="Sets Channel Vpots to control final post-DSP output level to multitrack outputs."
              bgColOff="ff184699" buttonText="LEVEL" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Meters On Top Button" id="587e741d11e80677" memberName="metersTopBtn"
              virtualName="" explicitFocusOrder="0" pos="8 216 46 18" bgColOff="ff172c52"
              buttonText="TOP" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Meters On Bottom Button" id="f91318300ca1187b" memberName="metersBotmBtn"
              virtualName="" explicitFocusOrder="0" pos="66 216 46 18" bgColOff="ff172c52"
              buttonText="BOTM" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Master Select Button" id="b9c045e6cd012663" memberName="masterSelectBtn"
              virtualName="" explicitFocusOrder="0" pos="20 142 80 18" tooltip="Activates Master fader in the Fat Channel Display."
              bgColOff="ff189994" buttonText="SELECT" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Master Write Button" id="816a1a692b804f0" memberName="masterWriteBtn"
              virtualName="" explicitFocusOrder="0" pos="20 168 80 18" tooltip="Activates the Master Strip to record selected automation data by the Main fader og Aux Masters"
              bgColOff="ff565555" buttonText="WRITE" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="Master Insert Right Label" id="fa6dee02ed011a54" memberName="masterInsertRLabel2"
         virtualName="" explicitFocusOrder="0" pos="14 12 88 20" bkgCol="ff140404"
         outlineCol="44d4c4c" edTextCol="ff000000" edBkgCol="0" labelText="--&#10;"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <COMBOBOX name="Master Insert Left" id="e83632aeadd3806d" memberName="masterInsertL"
            virtualName="" explicitFocusOrder="0" pos="14 38 88 18" tooltip="Allows any plugin to be patched into the main mix outputs.&#10;(Post fader insert)"
            editable="0" layout="36" items="Reset&#10;P01&#10;P02&#10;P03&#10;P04&#10;P05&#10;P06&#10;P07&#10;P08&#10;P09&#10;P10&#10;P11&#10;P12&#10;P13&#10;P14&#10;P15&#10;P16"
            textWhenNonSelected="INSERT L" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

