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

#include "MasterStripComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MasterStripComponent::MasterStripComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
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
                                               TRANS("--\n")));
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
    masterInsertR->setTooltip (TRANS("Allows any plugin to be patched into the main mix outputs.\n"
    "(Post fader insert)"));
    masterInsertR->setEditableText (false);
    masterInsertR->setJustificationType (juce::Justification::centred);
    masterInsertR->setTextWhenNothingSelected (TRANS("INSERT R"));
    masterInsertR->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    masterInsertR->addItem (TRANS("Reset"), 1);
    masterInsertR->addItem (TRANS("P01"), 2);
    masterInsertR->addItem (TRANS("P02"), 3);
    masterInsertR->addItem (TRANS("P03"), 4);
    masterInsertR->addItem (TRANS("P04"), 5);
    masterInsertR->addItem (TRANS("P05"), 6);
    masterInsertR->addItem (TRANS("P06"), 7);
    masterInsertR->addItem (TRANS("P07"), 8);
    masterInsertR->addItem (TRANS("P08"), 9);
    masterInsertR->addItem (TRANS("P09"), 10);
    masterInsertR->addItem (TRANS("P10"), 11);
    masterInsertR->addItem (TRANS("P11"), 12);
    masterInsertR->addItem (TRANS("P12"), 13);
    masterInsertR->addItem (TRANS("P13"), 14);
    masterInsertR->addItem (TRANS("P14"), 15);
    masterInsertR->addItem (TRANS("P15"), 16);
    masterInsertR->addItem (TRANS("P16"), 17);
    masterInsertR->addListener (this);

    masterInsertR->setBounds (16, 94, 88, 18);

    mastersBankBtn.reset (new juce::TextButton ("Masters Bank Button"));
    addAndMakeVisible (mastersBankBtn.get());
    mastersBankBtn->setButtonText (TRANS("Masters"));
    mastersBankBtn->addListener (this);
    mastersBankBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff3d3f4b));
    mastersBankBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffc8c892));

    mastersBankBtn->setBounds (16, 764, 88, 20);

    effectsBankBtn.reset (new juce::TextButton ("Effects Bank Button"));
    addAndMakeVisible (effectsBankBtn.get());
    effectsBankBtn->setButtonText (TRANS("Effects"));
    effectsBankBtn->addListener (this);
    effectsBankBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff3d3f4b));
    effectsBankBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffc8c892));

    effectsBankBtn->setBounds (16, 738, 88, 20);

    tapeBanksBtn.reset (new juce::TextButton ("Tape Bank Button"));
    addAndMakeVisible (tapeBanksBtn.get());
    tapeBanksBtn->setButtonText (TRANS("Tape"));
    tapeBanksBtn->addListener (this);
    tapeBanksBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff3d3f4b));
    tapeBanksBtn->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffc8c892));

    tapeBanksBtn->setBounds (16, 712, 88, 20);

    tapeBanksBtn2.reset (new juce::TextButton ("Tape Bank Button"));
    addAndMakeVisible (tapeBanksBtn2.get());
    tapeBanksBtn2->setButtonText (TRANS("Tape"));
    tapeBanksBtn2->addListener (this);
    tapeBanksBtn2->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff3d3f4b));
    tapeBanksBtn2->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffc8c892));

    tapeBanksBtn2->setBounds (16, 686, 88, 20);

    panMasterBtn.reset (new juce::TextButton ("Pan Master Button"));
    addAndMakeVisible (panMasterBtn.get());
    panMasterBtn->setTooltip (TRANS("V-Pot set to function as front/rear pan position control for any selected channel in surround modes"));
    panMasterBtn->setButtonText (TRANS("PAN"));
    panMasterBtn->addListener (this);
    panMasterBtn->setColour (juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);

    panMasterBtn->setBounds (8, 560, 46, 18);

    soloMasterBtn.reset (new juce::TextButton ("Solo Master Btn"));
    addAndMakeVisible (soloMasterBtn.get());
    soloMasterBtn->setTooltip (TRANS("Solos any selected Aux bus"));
    soloMasterBtn->setButtonText (TRANS("SOLO"));
    soloMasterBtn->addListener (this);
    soloMasterBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff90883a));

    soloMasterBtn->setBounds (66, 560, 46, 18);

    cuePan1Btn.reset (new juce::TextButton ("Cue Pan 1 Button"));
    addAndMakeVisible (cuePan1Btn.get());
    cuePan1Btn->setTooltip (TRANS("Set channel Vpots to control aux 9/10 pan\n"));
    cuePan1Btn->setButtonText (TRANS("1"));
    cuePan1Btn->addListener (this);
    cuePan1Btn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    cuePan1Btn->setBounds (8, 526, 46, 18);

    cuePan2Btn.reset (new juce::TextButton ("Cue Pan 2 Button"));
    addAndMakeVisible (cuePan2Btn.get());
    cuePan2Btn->setTooltip (TRANS("Set channel Vpots to control aux 11/12 pan"));
    cuePan2Btn->setButtonText (TRANS("2"));
    cuePan2Btn->addListener (this);
    cuePan2Btn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    cuePan2Btn->setBounds (66, 526, 46, 18);

    cueLvl1Btn.reset (new juce::TextButton ("Cue Level 1 Button"));
    addAndMakeVisible (cueLvl1Btn.get());
    cueLvl1Btn->setTooltip (TRANS("Set channel Vpots to control aux 9/10 level"));
    cueLvl1Btn->setButtonText (TRANS("1"));
    cueLvl1Btn->addListener (this);
    cueLvl1Btn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    cueLvl1Btn->setBounds (8, 480, 46, 18);

    cueLvl2Btn.reset (new juce::TextButton ("Cue Level 2 Button"));
    addAndMakeVisible (cueLvl2Btn.get());
    cueLvl2Btn->setTooltip (TRANS("Vpots to control aux 9/10 level"));
    cueLvl2Btn->setButtonText (TRANS("2"));
    cueLvl2Btn->addListener (this);
    cueLvl2Btn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    cueLvl2Btn->setBounds (66, 480, 46, 18);

    aux1LevelsBtn.reset (new juce::TextButton ("Aux 1 Levels Button"));
    addAndMakeVisible (aux1LevelsBtn.get());
    aux1LevelsBtn->setTooltip (TRANS("Sets channel Vpots to control AUX 1 send level.\n"
    "Master Vpot controls AUX out level"));
    aux1LevelsBtn->setButtonText (TRANS("1"));
    aux1LevelsBtn->addListener (this);
    aux1LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux1LevelsBtn->setBounds (8, 350, 46, 18);

    aux2LevelsBtn.reset (new juce::TextButton ("Aux 2 Levels Button"));
    addAndMakeVisible (aux2LevelsBtn.get());
    aux2LevelsBtn->setTooltip (TRANS("Sets channel Vpots to control AUX 2 send level.\n"
    "Master Vpot controls AUX out level"));
    aux2LevelsBtn->setButtonText (TRANS("2"));
    aux2LevelsBtn->addListener (this);
    aux2LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux2LevelsBtn->setBounds (66, 350, 46, 18);

    aux3LevelsBtn.reset (new juce::TextButton ("Aux 3 Levels Button"));
    addAndMakeVisible (aux3LevelsBtn.get());
    aux3LevelsBtn->setTooltip (TRANS("Sets channel Vpots to control AUX 3 send level.\n"
    "Master Vpot controls AUX out level"));
    aux3LevelsBtn->setButtonText (TRANS("3"));
    aux3LevelsBtn->addListener (this);
    aux3LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux3LevelsBtn->setBounds (8, 376, 46, 18);

    aux4LevelsBtn.reset (new juce::TextButton ("Aux 4 Levels Button"));
    addAndMakeVisible (aux4LevelsBtn.get());
    aux4LevelsBtn->setTooltip (TRANS("Sets channel Vpots to control AUX 4 send level.\n"
    "Master Vpot controls AUX out level"));
    aux4LevelsBtn->setButtonText (TRANS("4"));
    aux4LevelsBtn->addListener (this);
    aux4LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux4LevelsBtn->setBounds (66, 376, 46, 18);

    aux5LevelsBtn.reset (new juce::TextButton ("Aux 5 Levels Button"));
    addAndMakeVisible (aux5LevelsBtn.get());
    aux5LevelsBtn->setTooltip (TRANS("Sets channel Vpots to control AUX 5 send level.\n"
    "Master Vpot controls AUX out level"));
    aux5LevelsBtn->setButtonText (TRANS("5"));
    aux5LevelsBtn->addListener (this);
    aux5LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux5LevelsBtn->setBounds (8, 402, 46, 18);

    aux6LevelsBtn.reset (new juce::TextButton ("Aux 6 Levels Button"));
    addAndMakeVisible (aux6LevelsBtn.get());
    aux6LevelsBtn->setTooltip (TRANS("Sets channel Vpots to control AUX 6 send level.\n"
    "Master Vpot controls AUX out level"));
    aux6LevelsBtn->setButtonText (TRANS("6"));
    aux6LevelsBtn->addListener (this);
    aux6LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux6LevelsBtn->setBounds (66, 402, 46, 18);

    aux7LevelsBtn.reset (new juce::TextButton ("Aux 7 Levels Button"));
    addAndMakeVisible (aux7LevelsBtn.get());
    aux7LevelsBtn->setTooltip (TRANS("Sets channel Vpots to control AUX 7 send level.\n"
    "Master Vpot controls AUX out level"));
    aux7LevelsBtn->setButtonText (TRANS("7"));
    aux7LevelsBtn->addListener (this);
    aux7LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux7LevelsBtn->setBounds (8, 428, 46, 18);

    aux8LevelsBtn.reset (new juce::TextButton ("Aux 8 Levels Button"));
    addAndMakeVisible (aux8LevelsBtn.get());
    aux8LevelsBtn->setTooltip (TRANS("Sets channel Vpots to control AUX 8 send level.\n"
    "Master Vpot controls AUX out level"));
    aux8LevelsBtn->setButtonText (TRANS("8"));
    aux8LevelsBtn->addListener (this);
    aux8LevelsBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    aux8LevelsBtn->setBounds (66, 428, 46, 18);

    digitalTrimBtn.reset (new juce::TextButton ("Digital Trim Button"));
    addAndMakeVisible (digitalTrimBtn.get());
    digitalTrimBtn->setTooltip (TRANS("Sets channel Vpots to control digital input level (pre DSP)"));
    digitalTrimBtn->setButtonText (TRANS("TRIM"));
    digitalTrimBtn->addListener (this);
    digitalTrimBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    digitalTrimBtn->setBounds (20, 306, 80, 18);

    trackingLvlBtn.reset (new juce::TextButton ("Tracking Level Button"));
    addAndMakeVisible (trackingLvlBtn.get());
    trackingLvlBtn->setTooltip (TRANS("Sets Channel Vpots to control final post-DSP output level to multitrack outputs."));
    trackingLvlBtn->setButtonText (TRANS("LEVEL"));
    trackingLvlBtn->addListener (this);
    trackingLvlBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff184699));

    trackingLvlBtn->setBounds (20, 262, 80, 18);

    metersTopBtn.reset (new juce::TextButton ("Meters On Top Button"));
    addAndMakeVisible (metersTopBtn.get());
    metersTopBtn->setButtonText (TRANS("TOP"));
    metersTopBtn->addListener (this);
    metersTopBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff172c52));

    metersTopBtn->setBounds (8, 216, 46, 18);

    metersBotmBtn.reset (new juce::TextButton ("Meters On Bottom Button"));
    addAndMakeVisible (metersBotmBtn.get());
    metersBotmBtn->setButtonText (TRANS("BOTM"));
    metersBotmBtn->addListener (this);
    metersBotmBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff172c52));

    metersBotmBtn->setBounds (66, 216, 46, 18);

    masterSelectBtn.reset (new juce::TextButton ("Master Select Button"));
    addAndMakeVisible (masterSelectBtn.get());
    masterSelectBtn->setTooltip (TRANS("Activates Master fader in the Fat Channel Display."));
    masterSelectBtn->setButtonText (TRANS("SELECT"));
    masterSelectBtn->addListener (this);
    masterSelectBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff189994));

    masterSelectBtn->setBounds (20, 142, 80, 18);

    masterWriteBtn.reset (new juce::TextButton ("Master Write Button"));
    addAndMakeVisible (masterWriteBtn.get());
    masterWriteBtn->setTooltip (TRANS("Activates the Master Strip to record selected automation data by the Main fader og Aux Masters"));
    masterWriteBtn->setButtonText (TRANS("WRITE"));
    masterWriteBtn->addListener (this);
    masterWriteBtn->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff565555));

    masterWriteBtn->setBounds (20, 168, 80, 18);

    masterInsertRLabel2.reset (new juce::Label ("Master Insert Right Label",
                                                TRANS("--\n")));
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
    masterInsertL->setTooltip (TRANS("Allows any plugin to be patched into the main mix outputs.\n"
    "(Post fader insert)"));
    masterInsertL->setEditableText (false);
    masterInsertL->setJustificationType (juce::Justification::centred);
    masterInsertL->setTextWhenNothingSelected (TRANS("INSERT L"));
    masterInsertL->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    masterInsertL->addItem (TRANS("Reset"), 1);
    masterInsertL->addItem (TRANS("P01"), 2);
    masterInsertL->addItem (TRANS("P02"), 3);
    masterInsertL->addItem (TRANS("P03"), 4);
    masterInsertL->addItem (TRANS("P04"), 5);
    masterInsertL->addItem (TRANS("P05"), 6);
    masterInsertL->addItem (TRANS("P06"), 7);
    masterInsertL->addItem (TRANS("P07"), 8);
    masterInsertL->addItem (TRANS("P08"), 9);
    masterInsertL->addItem (TRANS("P09"), 10);
    masterInsertL->addItem (TRANS("P10"), 11);
    masterInsertL->addItem (TRANS("P11"), 12);
    masterInsertL->addItem (TRANS("P12"), 13);
    masterInsertL->addItem (TRANS("P13"), 14);
    masterInsertL->addItem (TRANS("P14"), 15);
    masterInsertL->addItem (TRANS("P15"), 16);
    masterInsertL->addItem (TRANS("P16"), 17);
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
    tapeBanksBtn2 = nullptr;
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
        int x = 5, y = 893, width = 65, height = 8;
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
        int x = 5, y = 934, width = 65, height = 8;
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
        int x = 5, y = 956, width = 65, height = 8;
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
        int x = 5, y = 870, width = 65, height = 8;
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
        int x = 5, y = 827, width = 65, height = 8;
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
        int x = 5, y = 803, width = 65, height = 8;
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
        int x = 5, y = 914, width = 65, height = 8;
        juce::String text (TRANS("20 -"));
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
        juce::String text (TRANS("CUE PAN"));
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
        juce::String text (TRANS("CUE LEVEL"));
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
        juce::String text (TRANS("AUXES"));
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
        juce::String text (TRANS("DIGITAL"));
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
        juce::String text (TRANS("TRACKING"));
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
        juce::String text (TRANS("METERS"));
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
        juce::String text (TRANS("MASTER"));
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
    else if (buttonThatWasClicked == tapeBanksBtn2.get())
    {
        //[UserButtonCode_tapeBanksBtn2] -- add your button handler code here..
        //[/UserButtonCode_tapeBanksBtn2]
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
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MasterStripComponent" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
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
  <TEXTBUTTON name="Tape Bank Button" id="e5d0d1771b8f7c35" memberName="tapeBanksBtn2"
              virtualName="" explicitFocusOrder="0" pos="16 686 88 20" bgColOff="ff3d3f4b"
              bgColOn="ffc8c892" buttonText="Tape" connectedEdges="0" needsCallback="1"
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

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: clipboard01_jpg, 25715, "../../../Downloads/Clipboard01.jpg"
static const unsigned char resource_MasterStripComponent_clipboard01_jpg[] = { 255,216,255,224,0,16,74,70,73,70,0,1,1,0,0,1,0,1,0,0,255,219,0,67,0,6,4,5,6,5,4,6,6,5,6,7,7,6,8,10,16,10,10,9,9,10,20,14,
15,12,16,23,20,24,24,23,20,22,22,26,29,37,31,26,27,35,28,22,22,32,44,32,35,38,39,41,42,41,25,31,45,48,45,40,48,37,40,41,40,255,219,0,67,1,7,7,7,10,8,10,19,10,10,19,40,26,22,26,40,40,40,40,40,40,40,40,
40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,255,192,0,17,8,4,118,0,103,3,1,34,0,2,17,1,3,17,1,255,196,0,28,0,0,2,2,3,1,
1,0,0,0,0,0,0,0,0,0,0,0,7,5,6,1,3,4,2,8,255,196,0,88,16,0,1,3,3,0,5,5,11,6,9,10,5,3,3,5,0,1,2,3,4,0,5,17,6,18,33,49,65,7,19,20,50,113,21,34,51,55,81,97,115,145,180,209,240,35,53,83,129,146,148,22,36,82,
84,85,161,177,178,193,23,52,54,66,86,114,116,147,149,210,37,68,87,98,100,67,130,162,99,179,241,8,69,131,194,195,255,196,0,26,1,0,3,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,2,3,4,1,5,6,255,196,0,47,17,0,2,2,1,3,3,
4,2,0,6,2,3,0,0,0,0,0,1,2,3,17,18,49,50,19,33,81,4,65,82,97,145,161,5,20,21,34,177,240,66,113,129,225,241,255,218,0,12,3,1,0,2,17,3,17,0,63,0,245,202,213,222,231,252,169,105,68,120,250,75,164,48,90,140,
168,193,12,68,185,58,203,72,214,142,218,142,18,147,129,146,73,237,38,168,230,255,0,120,7,250,99,165,127,235,47,255,0,186,172,28,175,120,223,211,127,73,19,217,26,165,170,186,199,182,181,87,5,37,177,9,201,
166,90,123,191,120,254,216,233,95,250,203,255,0,238,163,187,247,143,237,142,149,255,0,172,191,254,234,82,94,229,190,213,201,228,182,234,210,156,238,6,184,58,124,175,167,95,218,53,55,40,167,140,14,162,
218,206,71,87,119,239,31,219,29,43,255,0,89,127,253,212,119,126,241,253,177,210,191,245,151,255,0,221,73,94,159,43,233,215,246,141,29,62,87,211,175,237,26,53,199,192,105,126,71,87,119,239,31,219,29,43,
255,0,89,127,253,212,119,126,241,253,177,210,191,245,151,255,0,221,73,94,159,43,233,215,246,141,29,62,87,211,175,237,26,53,199,192,105,126,71,87,119,239,31,219,29,43,255,0,89,127,253,212,119,126,241,253,
177,210,191,245,151,255,0,221,73,94,159,43,233,215,246,141,29,62,87,211,175,237,26,53,199,192,105,126,71,87,119,239,31,219,29,43,255,0,89,127,253,212,119,126,241,253,177,210,191,245,151,255,0,221,73,94,
159,43,233,215,246,141,48,150,194,35,106,182,217,81,72,74,79,124,114,118,140,211,195,76,222,48,44,179,21,156,146,247,141,44,190,67,104,41,189,49,210,172,159,45,225,243,255,0,246,162,169,154,73,224,83,
241,198,138,75,26,139,198,14,197,57,44,228,114,242,189,227,127,77,253,36,79,100,106,150,170,235,30,218,101,114,189,227,127,77,253,36,79,100,106,150,170,235,30,218,181,92,73,217,185,70,210,15,157,95,237,
168,218,146,210,15,157,95,237,168,218,203,46,76,209,29,144,81,69,20,167,66,138,40,160,2,138,40,160,2,154,51,60,48,254,226,127,116,82,186,154,51,60,48,254,226,127,116,85,232,228,74,221,138,246,146,120,
20,252,113,162,141,36,240,41,248,227,69,45,220,206,215,196,114,242,189,227,127,77,253,36,79,100,106,150,170,235,30,218,101,114,189,227,127,77,253,36,79,100,106,150,170,235,30,218,189,92,73,89,185,155,
69,173,185,166,99,200,209,151,239,26,142,234,173,212,21,0,131,129,222,236,63,93,119,247,17,191,250,127,39,237,47,223,82,252,158,60,243,118,43,234,89,112,160,170,114,70,207,238,38,172,178,224,93,152,98,
107,168,147,207,38,27,252,195,220,217,234,157,80,115,217,180,84,180,166,202,106,194,40,125,196,111,254,159,201,251,75,247,209,220,70,255,0,233,252,159,180,191,125,94,90,143,112,91,204,161,115,208,218,
23,29,50,150,226,137,195,72,36,128,85,179,202,43,17,49,41,201,156,206,144,194,83,17,27,74,221,124,21,106,13,99,128,55,103,57,163,74,65,169,148,126,226,55,255,0,79,228,253,165,251,232,238,35,127,244,254,
79,218,95,190,174,241,72,146,169,170,107,72,161,24,241,18,133,56,254,85,168,53,137,0,110,206,114,43,220,118,36,185,47,163,200,187,199,140,165,20,6,75,170,56,123,88,247,165,56,28,104,210,131,83,40,189,
196,111,254,159,201,251,75,247,209,220,70,255,0,233,252,159,180,191,125,91,46,179,58,12,206,136,205,234,60,201,9,42,74,208,193,36,160,164,224,131,145,89,15,77,238,67,215,21,75,213,101,167,144,193,4,237,
42,86,113,143,85,119,66,220,53,50,184,189,20,125,22,199,46,43,228,226,104,130,218,74,148,241,43,213,0,111,57,214,168,119,166,183,112,112,201,97,144,195,106,216,27,7,58,184,217,143,213,86,125,34,191,221,
6,140,75,130,38,186,34,184,117,212,222,118,18,1,247,213,22,197,182,214,201,62,127,218,105,171,88,150,4,155,204,78,93,36,240,41,248,227,69,26,73,224,83,241,198,138,149,220,202,87,196,114,242,189,227,127,
77,253,36,79,100,106,150,170,235,30,218,101,114,189,227,127,77,253,36,79,100,106,150,170,235,30,218,189,92,73,89,185,98,208,37,134,236,119,181,30,19,146,113,255,0,177,53,37,116,191,60,253,197,111,66,101,
230,74,174,202,151,172,72,199,54,89,8,193,250,233,120,161,112,142,36,55,14,74,27,106,67,156,225,4,28,231,24,254,21,203,255,0,24,193,61,49,59,55,236,53,55,23,224,116,254,198,204,173,32,140,189,32,69,192,
70,155,25,44,64,67,13,166,17,71,92,40,231,26,220,48,120,214,45,186,80,25,77,209,195,30,124,113,33,182,219,75,172,134,250,66,212,149,100,173,89,239,114,115,194,149,24,188,107,17,211,19,179,204,107,31,241,
131,171,248,226,118,238,216,107,152,151,128,207,216,205,133,164,175,91,152,184,155,83,87,30,147,33,214,87,206,203,13,235,43,11,202,193,213,217,130,43,9,155,28,105,63,116,3,111,168,115,168,125,101,194,
54,156,228,165,62,97,194,150,103,186,224,103,166,167,25,198,227,89,61,216,25,252,113,57,24,39,97,174,165,37,236,13,231,220,188,48,121,201,19,95,83,124,217,122,83,174,164,43,25,194,150,72,253,181,50,213,
194,222,141,31,145,2,226,196,215,131,146,90,125,61,16,163,91,40,7,126,183,13,180,174,29,216,56,252,113,57,32,145,176,208,59,174,70,122,106,113,156,110,52,127,118,49,131,158,249,201,108,210,73,44,72,129,
48,197,143,37,134,0,194,4,146,157,99,179,254,221,149,90,176,252,214,207,215,251,77,114,186,213,213,228,41,14,73,66,135,16,65,169,27,116,115,22,27,108,168,130,83,188,138,122,212,181,101,163,146,198,48,
136,237,36,240,41,248,227,69,26,73,224,83,241,198,138,141,220,202,87,196,114,242,189,227,127,77,253,36,79,100,106,150,170,235,30,218,101,114,189,227,127,77,253,36,79,100,106,150,170,235,30,218,189,92,
73,89,185,13,54,248,212,89,43,101,109,40,148,156,100,26,211,248,73,31,232,87,235,168,93,32,249,213,254,218,141,168,202,217,38,209,85,92,90,45,159,132,177,254,133,126,186,63,9,99,253,10,253,117,83,162,
185,214,152,116,226,91,63,9,35,253,10,253,116,126,18,199,250,21,250,234,167,69,29,105,135,78,37,179,240,150,63,208,175,215,71,225,36,127,161,95,174,170,116,81,214,152,116,226,91,63,9,88,250,21,250,234,
117,58,250,163,157,108,182,162,51,170,78,113,75,106,104,204,240,195,251,137,253,209,85,170,201,73,225,137,100,20,87,98,189,164,158,5,63,28,104,163,73,60,10,126,56,209,82,187,152,245,241,28,188,175,120,
223,211,127,73,19,217,26,165,170,186,199,182,153,92,175,120,223,211,127,73,19,217,26,165,170,186,199,182,175,87,18,86,110,81,180,131,231,87,251,106,54,174,14,104,173,194,243,50,67,241,87,17,8,11,213,249,
103,195,100,236,225,154,245,252,158,94,63,56,182,125,241,21,154,73,234,101,226,214,17,77,162,174,95,201,229,227,243,139,103,223,17,71,242,121,120,252,226,217,247,196,82,225,157,202,41,180,85,203,249,60,
188,126,113,108,251,226,40,254,79,47,31,156,91,62,248,138,48,195,40,166,209,87,47,228,242,241,249,197,179,239,136,163,249,60,188,126,113,108,251,226,40,195,12,162,155,77,25,158,24,127,113,63,186,42,3,
249,60,188,126,113,108,251,226,42,199,116,104,177,49,77,21,33,101,9,72,37,10,214,7,96,220,106,212,39,168,157,175,177,90,210,79,2,159,142,52,81,164,158,5,63,28,104,165,187,153,218,248,142,94,87,188,111,
233,191,164,137,236,141,82,213,93,99,219,76,174,87,188,111,233,191,164,137,236,141,82,213,93,99,219,87,171,137,43,55,37,244,66,201,103,186,219,110,242,239,45,201,113,214,37,6,219,230,222,82,0,78,160,59,
135,156,215,127,224,246,138,254,107,59,239,75,247,215,158,79,246,217,175,64,254,126,159,220,77,92,47,178,225,180,244,248,211,99,163,154,122,242,168,200,121,3,5,129,204,133,100,1,191,111,3,229,169,246,
247,41,223,216,168,254,15,104,175,230,179,190,244,191,125,31,131,218,43,249,172,239,189,47,223,87,102,216,113,157,37,133,21,136,230,74,90,181,54,231,61,21,160,238,162,138,148,53,194,119,43,59,182,215,
187,39,75,84,235,233,122,91,178,37,34,59,69,180,51,111,109,79,180,10,206,246,183,5,121,107,159,219,224,49,34,141,248,61,162,191,154,206,251,210,253,244,126,15,104,175,230,179,190,244,191,125,92,109,146,
164,197,238,228,171,228,169,16,206,99,180,211,175,91,155,74,208,149,44,128,75,123,182,231,105,174,72,179,166,196,210,209,25,134,90,98,59,207,52,146,227,140,165,69,244,235,109,88,4,97,33,91,246,87,86,31,
177,199,149,238,86,127,7,180,87,243,89,223,122,95,190,143,193,237,21,252,214,119,222,151,239,174,155,165,206,125,210,237,44,203,125,174,98,60,151,153,109,166,216,74,48,2,200,25,32,109,221,86,141,20,126,
60,109,29,152,228,137,130,10,156,152,195,41,147,209,208,246,169,80,86,194,21,176,13,155,235,184,142,51,128,203,206,50,85,110,182,205,5,137,162,51,22,139,109,192,222,19,177,165,153,174,106,227,7,190,35,
56,217,179,101,81,44,235,46,91,218,82,137,36,231,127,109,91,52,223,156,213,184,135,34,24,168,27,16,149,103,43,24,235,99,134,119,253,117,80,176,252,214,207,215,251,77,118,190,210,236,44,184,247,57,116,
147,192,167,227,141,20,105,39,129,79,199,26,42,87,115,41,95,17,203,202,247,141,253,55,244,145,61,145,170,90,171,172,123,105,147,202,249,3,149,253,55,201,3,229,34,123,35,84,181,81,26,199,104,223,87,171,
137,43,55,61,217,180,150,53,150,223,118,137,35,99,143,73,14,39,203,141,64,63,133,115,175,74,173,235,119,156,88,113,74,231,57,220,149,19,223,227,25,223,228,172,173,13,47,58,233,65,207,150,181,244,118,117,
137,213,71,102,5,113,214,253,153,221,75,220,220,189,47,134,165,169,122,239,37,74,64,108,148,172,164,234,131,144,54,26,240,214,149,192,105,183,16,218,159,79,56,114,181,7,20,20,174,211,156,214,177,25,144,
19,177,59,54,29,131,109,100,199,103,7,98,115,156,231,2,185,211,151,144,212,128,233,53,184,165,105,89,121,97,101,37,90,238,41,89,213,57,27,205,116,13,49,135,207,151,178,225,112,128,53,138,142,192,55,99,
110,202,231,49,153,201,239,83,180,1,140,13,148,8,236,236,216,157,153,224,54,208,171,151,144,212,141,172,233,101,189,148,168,33,42,26,202,43,57,219,146,78,73,172,187,165,240,93,108,54,189,114,128,176,189,
92,236,214,27,141,104,232,204,224,12,39,126,115,129,65,140,206,21,222,167,111,152,108,163,167,47,33,170,62,15,23,77,39,137,50,59,233,73,81,113,208,1,82,137,60,48,43,93,139,230,182,126,191,219,93,29,29,
157,96,117,83,179,134,5,109,64,9,72,25,20,208,131,139,203,103,37,36,214,17,17,164,158,5,63,28,104,172,105,25,28,210,118,252,102,138,133,220,138,215,196,113,114,199,30,59,220,175,233,185,125,148,184,174,
114,32,4,147,179,241,70,169,110,89,138,9,252,85,191,89,247,211,51,149,239,27,250,111,233,34,123,35,84,181,87,88,246,213,170,75,73,59,31,115,199,51,23,243,86,253,103,223,94,132,104,229,37,66,26,10,70,243,
183,20,85,166,209,32,254,13,51,110,36,33,155,140,197,50,227,131,172,144,150,245,198,169,242,229,32,83,203,17,89,192,177,78,79,25,42,194,52,114,146,161,13,26,163,142,220,86,57,136,218,185,232,141,227,203,
183,223,86,133,74,45,232,92,203,124,100,40,45,200,172,203,37,88,214,74,214,232,78,175,155,117,117,92,172,50,147,100,16,210,202,138,226,182,36,132,165,196,101,78,43,99,137,35,59,146,6,105,117,199,192,218,
36,83,211,21,149,39,89,48,146,83,229,1,88,172,136,109,17,145,0,16,124,202,169,242,252,169,144,27,102,201,115,49,18,25,33,80,90,29,243,132,14,253,100,227,24,198,221,252,43,83,87,171,171,26,33,61,198,110,
79,133,180,228,118,154,80,35,188,73,81,7,27,56,208,218,94,199,18,251,33,83,13,165,12,166,10,72,243,5,86,58,35,58,165,93,5,58,163,121,194,177,86,37,203,147,15,69,109,206,13,34,92,7,20,185,11,82,74,73,91,
234,214,28,64,35,255,0,205,118,91,86,239,113,45,43,114,238,67,1,15,200,145,17,64,149,203,72,88,36,110,199,28,125,117,205,107,193,221,31,101,59,153,139,249,171,126,179,239,163,153,139,249,171,126,179,239,
161,14,41,210,183,20,223,52,20,181,20,163,200,156,236,253,85,234,170,146,126,196,219,126,72,93,34,67,41,101,60,219,8,71,102,124,180,81,164,158,5,63,28,104,172,150,246,145,162,190,35,151,149,239,27,250,
111,233,34,123,35,84,181,87,88,246,211,43,149,239,27,250,111,233,34,123,35,84,181,87,88,246,213,234,226,74,205,205,106,117,180,156,41,104,7,200,77,123,118,224,87,26,36,116,188,200,106,60,142,147,180,237,
42,213,197,77,104,92,123,107,214,155,219,183,27,108,105,142,183,40,4,45,212,2,82,157,65,176,125,117,216,27,177,19,129,97,134,73,255,0,233,138,51,41,108,140,118,250,202,168,150,153,110,87,94,188,60,236,
139,163,234,122,55,61,61,105,82,207,228,132,168,40,1,183,202,43,203,87,45,75,212,219,169,84,99,50,74,156,57,224,53,193,7,143,158,173,170,182,91,144,164,37,90,44,218,84,179,132,131,31,5,93,155,43,14,91,
109,173,144,28,209,118,210,85,156,5,71,198,113,191,133,115,75,248,139,253,74,191,191,193,83,110,240,251,22,246,226,68,22,216,248,104,52,167,219,78,29,80,198,21,183,60,69,104,50,130,44,238,91,153,121,144,
211,139,109,74,42,57,61,225,200,171,115,49,172,207,184,16,206,142,198,113,103,114,80,208,39,212,43,99,208,45,108,169,9,123,70,89,109,75,56,72,91,24,214,62,109,148,105,150,216,253,156,254,167,83,239,223,
240,87,19,126,83,81,35,199,75,22,183,210,193,81,66,164,163,92,141,99,147,141,181,161,155,195,233,155,108,144,183,98,102,9,90,155,74,54,12,169,65,94,93,219,42,216,109,150,224,226,91,58,44,216,90,129,33,
38,62,211,245,98,188,191,111,182,71,70,187,250,48,211,72,206,53,150,198,168,253,98,140,75,59,126,195,250,149,73,123,254,10,98,100,5,21,173,249,13,173,197,173,75,36,43,202,115,91,65,4,100,16,71,154,165,
239,137,179,139,123,157,30,203,13,183,56,43,155,27,42,183,101,36,219,155,39,202,127,105,170,71,82,236,209,106,111,141,233,202,27,28,122,73,224,83,241,198,138,52,147,192,167,227,141,21,150,238,102,234,
248,142,94,87,188,111,233,191,164,137,236,141,82,213,93,99,219,76,174,87,188,111,233,191,164,137,236,141,82,213,93,99,219,87,171,137,43,55,38,52,55,230,59,255,0,248,161,251,137,169,27,8,112,221,227,6,
3,5,205,109,129,245,4,163,119,18,118,10,167,67,190,185,104,139,113,138,25,113,93,33,240,230,176,70,70,53,64,254,21,160,233,26,206,65,97,223,39,131,52,213,180,147,77,158,55,171,244,151,91,118,184,46,221,
135,19,18,35,55,167,86,213,61,116,144,86,75,154,177,205,200,72,109,35,155,86,23,206,14,245,25,221,131,186,185,160,53,32,105,67,10,147,112,230,216,92,119,144,202,30,189,162,80,14,22,212,50,72,61,232,218,
55,210,133,171,218,26,214,45,67,82,53,134,221,86,113,145,94,81,120,105,10,202,32,148,171,118,67,56,164,209,31,146,54,234,189,38,180,123,167,191,129,187,160,118,231,44,119,104,130,227,62,222,167,92,138,
242,50,204,246,201,214,230,207,245,129,239,123,107,75,101,113,180,141,46,206,148,84,149,198,117,184,109,187,117,68,221,87,138,112,149,18,58,189,246,54,157,212,168,69,225,166,213,148,65,213,56,198,67,56,
216,107,45,94,91,105,122,205,66,40,87,149,44,224,215,90,77,166,228,78,48,182,184,74,16,134,239,59,175,247,5,254,224,213,226,217,42,59,211,175,238,61,53,109,169,35,152,184,7,74,51,214,198,169,56,173,186,
111,62,77,195,73,158,96,220,228,189,17,150,34,169,45,37,242,80,85,205,140,146,55,19,157,244,188,69,237,13,184,92,68,37,37,103,250,193,156,29,181,176,105,9,74,148,161,29,192,165,111,60,217,219,138,100,
161,219,45,17,157,94,169,169,198,41,226,88,221,236,78,94,63,153,42,160,236,159,54,181,218,127,105,173,23,11,243,207,198,83,104,97,221,99,179,193,157,149,209,101,74,209,109,100,56,146,149,109,200,61,180,
206,74,82,73,23,244,62,158,116,84,212,215,185,199,164,158,5,63,28,104,163,73,60,10,126,56,209,89,46,230,122,149,241,28,188,175,120,223,211,127,73,19,217,26,165,170,186,199,182,153,92,175,120,223,211,127,
73,19,217,26,165,170,186,199,182,175,87,18,86,110,115,59,46,59,106,40,117,230,210,124,138,53,175,166,66,214,39,164,53,159,239,85,183,65,159,109,139,21,245,197,197,142,250,196,192,2,157,108,40,142,240,
110,200,174,206,236,35,244,116,31,242,83,238,169,187,218,120,192,234,164,214,114,81,68,184,64,39,241,134,187,221,221,245,6,92,34,8,233,13,109,57,235,83,0,207,88,138,153,38,213,16,48,165,20,7,57,132,224,
168,109,198,234,236,136,221,198,98,194,98,216,25,117,69,176,238,17,25,39,189,59,142,237,198,185,215,126,14,244,151,145,103,211,33,100,158,144,214,209,142,181,2,100,32,71,227,13,108,255,0,186,152,188,252,
146,227,200,238,52,109,118,86,16,224,232,233,202,84,78,0,59,60,187,43,185,219,125,229,150,150,227,186,52,132,54,128,84,165,24,169,192,3,142,234,58,239,192,116,151,145,89,210,225,99,29,33,173,249,235,80,
101,194,58,223,140,53,223,111,239,169,160,168,151,100,196,233,39,71,27,17,245,57,206,115,162,167,26,184,206,119,110,168,158,236,35,244,116,31,242,83,238,163,174,252,7,73,121,40,189,50,22,176,61,33,172,
129,142,181,117,52,82,80,11,106,214,73,220,115,154,159,210,11,152,118,212,248,76,40,109,144,51,148,178,156,254,202,170,88,201,85,177,162,119,156,254,211,84,174,199,55,129,39,13,43,39,38,146,120,20,252,
113,162,141,36,240,41,248,227,69,70,238,101,43,226,57,121,94,241,191,166,254,146,39,178,53,75,85,117,143,109,50,185,94,241,191,166,254,146,39,178,53,75,85,117,143,109,94,174,36,172,220,152,209,31,232,
245,247,252,104,253,193,88,170,227,55,105,214,184,243,163,177,25,110,52,251,225,220,141,221,80,63,133,105,238,237,195,56,232,10,242,86,105,69,229,246,47,22,176,134,42,238,54,120,250,47,108,110,226,94,
126,75,115,220,91,113,90,25,214,5,0,101,91,118,1,180,143,56,174,251,142,144,89,160,76,184,4,184,102,196,238,100,38,18,210,156,45,173,68,58,173,110,169,216,160,14,105,85,221,217,199,7,160,19,141,162,176,
111,115,137,219,111,36,238,221,228,174,97,248,59,148,48,52,146,230,194,109,183,230,154,158,215,202,42,31,68,97,10,239,146,216,116,144,9,222,84,6,250,230,190,202,67,246,43,4,17,49,229,57,206,202,91,205,
135,149,173,131,171,140,237,221,190,169,6,249,52,146,77,188,147,179,135,170,179,221,201,249,7,160,28,140,224,209,135,224,50,134,118,157,201,181,205,142,194,25,184,90,27,45,196,97,42,82,231,186,151,142,
170,0,41,230,199,123,230,170,195,90,188,218,53,14,81,129,131,230,170,178,175,83,84,114,171,118,79,148,138,244,47,211,192,217,1,88,20,97,248,12,162,118,243,243,108,143,238,154,132,176,252,214,207,215,251,
77,115,78,187,92,101,71,83,34,26,211,173,176,145,93,150,102,150,205,185,164,58,146,149,140,228,30,218,181,9,169,119,37,107,77,28,90,73,224,83,241,198,138,52,147,192,167,227,141,20,151,115,26,190,35,131,
150,52,72,87,43,250,110,88,91,9,1,200,153,14,103,39,241,70,183,98,150,229,151,242,114,244,111,82,169,155,202,247,141,253,55,244,145,61,145,170,90,171,172,123,106,213,47,237,39,99,238,120,230,95,250,104,
222,165,87,158,142,246,115,207,70,206,237,202,173,149,59,6,215,17,253,29,122,74,150,225,184,173,210,212,102,18,70,28,32,107,17,234,201,250,170,143,11,187,17,101,236,87,132,103,70,62,86,46,205,219,21,71,
70,119,4,115,177,182,156,238,85,89,163,89,226,126,14,59,54,83,142,38,96,40,90,26,73,24,45,169,65,33,94,178,107,143,72,19,109,183,206,147,13,136,23,69,45,151,185,174,121,106,64,66,176,118,145,199,117,46,
168,231,3,105,120,201,11,209,157,201,60,236,92,145,131,177,84,8,206,140,124,172,109,155,182,42,167,238,34,203,26,205,26,107,113,110,106,92,151,28,105,180,107,163,97,64,7,39,205,182,188,93,155,183,91,91,
105,165,64,186,61,33,113,155,120,184,133,32,54,20,180,228,13,180,106,65,165,144,125,25,220,99,157,141,140,231,114,168,49,157,57,249,88,219,119,236,85,89,103,90,225,54,181,70,105,185,109,62,220,52,203,
92,135,72,230,182,163,88,164,99,110,120,87,21,205,168,54,216,225,165,71,155,46,81,105,46,7,152,41,13,2,161,148,141,189,187,104,213,16,211,34,31,163,187,144,121,232,217,27,55,42,178,150,30,72,192,122,48,
31,251,171,210,78,82,9,24,39,133,102,159,2,100,132,210,54,220,75,35,93,214,85,253,220,249,104,172,233,39,129,79,199,26,43,37,189,164,104,175,136,229,229,123,198,254,155,250,72,158,200,213,45,85,214,61,
180,202,229,123,198,254,155,250,72,158,200,213,42,219,98,91,241,27,150,39,198,105,14,147,132,41,162,74,112,72,223,159,53,90,185,97,34,115,89,103,77,73,55,115,68,72,54,148,54,160,95,110,122,156,82,72,36,
4,22,202,73,253,117,5,204,74,253,39,23,252,147,239,163,152,149,250,78,47,249,39,223,77,39,169,99,2,199,179,201,96,114,229,13,177,117,101,151,15,68,97,166,98,69,37,39,89,105,67,161,89,63,81,53,179,75,174,
49,231,79,149,32,233,27,146,155,15,21,49,15,155,94,16,14,204,100,140,85,111,152,149,250,78,47,249,39,223,71,49,43,244,156,95,242,79,190,145,67,15,40,119,44,172,50,94,231,41,151,44,214,104,168,86,94,105,
247,214,180,227,112,41,78,63,101,73,233,21,194,60,230,90,109,90,72,227,49,155,140,208,232,33,181,144,86,145,183,104,24,218,106,149,14,115,171,114,99,110,45,14,22,19,144,164,167,84,29,252,42,91,70,109,
55,93,34,128,38,69,147,29,150,202,138,53,84,209,81,217,245,208,220,119,5,146,195,119,210,6,110,125,50,4,137,78,170,212,152,76,173,180,128,71,202,165,190,168,243,19,190,184,215,114,139,6,204,99,139,161,
154,20,193,13,64,13,168,37,14,40,100,40,146,49,178,186,219,228,254,252,188,98,227,16,19,195,152,62,250,223,252,155,233,6,62,115,135,254,65,247,215,50,145,222,229,89,25,40,78,182,197,99,111,109,122,171,
42,249,57,210,4,140,247,74,31,249,7,223,92,142,232,53,245,188,102,225,19,105,250,19,239,170,117,80,154,25,75,210,79,2,159,142,52,87,86,152,90,102,90,86,195,23,7,219,121,110,179,206,164,182,141,93,81,173,
140,26,43,61,139,84,178,138,193,225,96,109,242,189,227,127,77,253,36,79,100,106,148,199,230,88,93,171,253,227,77,158,87,188,111,233,191,164,137,236,141,82,145,75,72,179,66,5,64,30,252,224,158,26,198,169,
14,201,9,63,114,201,96,111,71,223,178,220,31,155,108,150,227,240,35,115,238,45,18,117,67,135,88,12,1,141,155,235,146,58,45,12,219,220,187,204,129,33,113,31,146,99,71,136,153,24,82,72,78,182,74,241,183,
102,120,84,43,19,222,143,14,116,102,29,66,81,49,174,97,220,140,157,92,131,179,201,186,186,45,87,185,22,182,212,152,198,43,169,35,33,50,27,14,161,39,242,128,59,1,225,154,227,123,225,130,91,101,22,251,54,
141,91,36,189,41,231,25,82,218,84,38,230,179,29,114,131,92,216,82,245,117,84,225,216,119,111,170,133,245,168,145,238,206,177,5,97,72,78,213,36,43,92,54,120,164,44,108,86,60,162,181,201,188,79,146,139,
130,94,144,201,51,80,150,156,9,64,9,74,18,173,96,148,143,234,140,249,43,133,28,218,6,16,80,6,253,134,185,22,243,221,131,75,217,28,182,210,4,155,160,242,160,255,0,26,109,114,30,208,115,68,210,72,219,207,
44,126,186,82,219,73,233,55,76,13,154,135,63,174,154,188,137,188,164,232,186,16,1,35,158,89,62,109,181,200,237,17,223,185,126,190,223,77,138,229,111,132,196,38,36,61,45,11,88,83,210,209,29,41,212,198,
123,229,108,227,86,86,47,12,78,98,219,30,201,109,114,101,230,107,110,58,136,143,43,152,26,141,144,151,21,206,40,99,0,145,143,202,7,34,169,90,73,97,147,125,184,91,230,52,243,13,46,34,28,70,171,241,131,
232,80,86,63,170,118,103,101,76,166,215,38,27,118,233,22,105,238,70,187,66,109,108,183,37,240,93,72,67,132,21,167,80,157,217,3,30,64,49,93,146,144,169,162,77,249,210,223,135,57,152,182,68,170,247,1,212,
183,46,9,146,144,150,194,129,80,33,204,97,91,49,187,203,85,43,13,218,101,249,145,41,219,64,135,13,101,65,46,25,9,94,84,14,8,213,3,204,106,205,103,68,155,121,157,42,91,198,93,206,114,146,185,47,37,58,137,
89,72,213,78,19,195,101,65,90,33,57,103,179,55,11,156,83,161,181,45,90,218,184,206,178,137,221,245,211,69,51,141,161,95,203,114,2,111,118,252,113,132,127,251,148,86,57,108,214,238,228,29,108,255,0,51,
255,0,253,40,165,123,157,69,199,150,73,12,179,203,6,155,7,93,66,9,114,33,1,71,31,242,141,82,134,76,24,14,184,172,186,113,183,31,42,112,54,249,41,199,202,246,169,229,123,78,2,144,133,101,200,157,100,131,
255,0,40,213,46,10,240,79,120,223,216,20,245,199,49,71,38,240,202,240,182,65,216,75,167,57,201,249,83,178,177,220,200,58,167,229,79,144,124,169,219,86,232,22,233,247,6,212,184,80,75,232,73,193,40,104,
28,26,218,44,183,83,32,176,45,174,115,193,58,229,28,200,206,174,113,154,109,11,232,93,76,166,247,50,6,177,249,83,141,254,20,214,5,178,14,19,242,167,118,15,202,157,149,102,121,183,153,146,168,238,176,148,
60,149,106,148,22,198,65,242,87,151,210,228,119,150,211,236,161,14,160,225,73,83,96,16,107,189,52,115,91,33,98,71,135,23,156,13,56,146,28,216,162,165,231,101,108,105,17,217,26,177,230,62,202,127,37,169,
10,64,245,3,86,21,90,238,9,140,137,10,128,160,194,240,18,190,104,96,231,119,174,180,34,60,133,190,227,9,140,11,205,130,86,128,216,202,64,223,158,202,238,149,244,25,100,80,123,27,174,179,135,100,197,251,
235,60,241,253,43,59,239,139,247,212,210,109,179,212,151,20,32,40,165,180,7,22,121,161,222,164,140,130,124,216,174,101,161,198,218,109,213,176,132,182,230,117,20,91,24,86,55,226,185,161,125,6,166,71,115,
199,244,172,255,0,190,47,223,94,121,192,127,253,210,111,222,215,239,171,3,182,123,163,81,140,135,45,171,75,1,58,229,101,145,128,60,181,29,206,127,216,223,216,20,104,95,65,169,162,191,121,91,33,157,101,
73,121,247,112,18,20,227,165,204,12,238,218,104,173,186,74,188,178,158,245,3,177,32,113,162,161,99,196,176,86,11,43,35,143,149,239,27,250,111,233,34,123,35,84,181,87,88,246,211,43,149,239,27,250,111,233,
34,123,35,84,181,87,88,246,213,170,226,78,205,201,125,18,82,255,0,8,96,164,56,180,164,44,171,84,40,128,78,169,222,43,26,30,25,93,158,252,245,194,84,166,208,184,192,45,228,40,169,105,28,232,221,147,80,
141,93,17,6,72,113,169,28,211,232,220,64,206,14,43,153,155,139,76,219,159,134,212,194,150,223,72,67,157,239,88,107,103,201,229,21,201,164,217,216,182,145,120,98,18,23,125,83,146,92,105,171,61,186,48,8,
144,250,136,231,134,72,75,153,227,146,71,170,176,99,196,93,198,21,201,217,80,164,183,45,11,11,91,78,21,7,37,132,146,127,89,77,83,36,221,250,67,78,52,185,234,230,150,202,88,40,213,217,168,149,107,1,187,
203,88,118,232,203,140,198,103,165,97,136,238,41,196,32,39,102,177,24,39,119,154,185,135,157,198,202,240,74,67,68,185,23,200,202,186,169,241,36,201,65,91,101,69,33,7,88,108,213,27,54,84,158,144,160,234,
92,69,157,197,24,202,150,224,152,233,216,224,86,183,84,142,9,206,64,242,212,12,141,37,148,242,26,111,186,171,75,45,148,148,160,54,55,164,228,109,198,107,76,123,192,142,185,46,55,52,243,178,10,202,215,
171,191,91,126,204,98,187,133,147,153,120,47,87,73,198,218,221,192,235,40,71,90,45,204,186,145,180,169,181,32,133,15,172,86,155,132,37,91,165,89,153,83,109,169,225,14,83,176,218,73,42,5,69,73,45,141,187,
206,14,234,164,203,187,153,104,113,18,46,11,90,92,230,181,129,78,254,108,97,60,40,126,236,135,221,138,167,38,40,136,201,82,91,78,15,123,172,65,59,126,170,69,31,177,181,125,18,209,160,34,109,186,234,253,
210,92,225,50,59,42,124,182,20,66,72,27,210,70,118,87,3,100,20,36,141,216,24,162,225,164,210,230,69,122,51,183,133,166,43,168,230,214,216,105,32,20,249,51,140,215,150,20,133,50,130,218,130,145,140,3,229,
170,195,118,78,91,17,58,73,224,83,241,198,138,52,147,192,167,227,141,21,158,238,101,107,226,57,121,94,241,191,166,254,146,39,178,53,75,85,117,143,109,50,185,94,241,191,166,254,146,39,178,53,75,85,117,
143,109,94,174,36,172,220,157,208,185,171,133,163,247,213,180,132,21,116,209,181,73,7,31,38,43,111,225,28,207,201,107,236,10,143,209,175,232,229,243,252,104,255,0,237,138,229,173,158,150,168,74,45,181,
238,13,150,25,87,155,156,80,201,144,203,104,15,54,29,111,41,29,242,78,227,82,109,183,164,142,67,68,180,192,79,71,90,57,212,172,234,12,167,203,130,115,81,247,116,89,167,53,107,146,246,144,196,138,243,22,
228,48,168,235,105,197,43,89,36,157,224,99,141,110,210,9,118,43,189,130,220,131,113,179,52,251,112,91,66,148,251,15,23,193,78,78,168,41,239,124,212,147,156,34,187,69,103,35,40,154,92,186,221,90,145,29,
133,176,128,235,233,74,218,78,170,114,160,174,169,29,181,35,29,157,38,144,218,156,102,222,149,33,46,41,162,78,160,194,146,112,70,211,194,163,45,183,187,83,54,168,211,101,178,151,38,194,109,77,182,165,
3,174,232,41,195,33,62,77,69,109,219,229,174,152,115,173,146,180,38,36,107,188,235,82,46,43,125,247,157,55,6,157,90,146,167,8,57,78,167,241,162,215,161,118,130,5,20,253,206,121,87,91,164,86,155,113,246,
91,67,110,41,104,74,181,82,65,41,56,80,250,141,115,126,17,204,252,150,190,192,175,109,179,98,151,163,246,102,23,164,16,161,170,19,146,82,160,182,157,60,230,178,134,20,48,55,108,227,229,168,16,182,150,
165,150,28,14,182,20,82,22,1,1,88,59,246,213,169,85,217,148,210,201,201,38,142,141,36,191,203,118,210,242,79,54,54,111,9,21,95,176,146,171,91,36,239,57,63,174,186,47,223,54,59,217,92,214,15,154,88,236,
63,182,165,234,34,161,52,163,224,227,226,115,105,39,129,79,199,26,40,210,79,2,159,142,52,87,157,119,50,181,241,28,188,175,120,223,211,127,73,19,217,26,165,170,186,199,182,153,92,175,120,223,211,127,73,
19,217,26,165,170,186,199,182,175,87,18,86,110,71,42,101,206,19,19,35,68,111,89,169,15,115,185,206,63,170,7,240,168,254,153,120,198,121,175,62,250,159,32,18,50,55,110,175,108,198,113,242,164,178,202,220,
59,200,66,73,253,149,84,231,21,136,203,0,167,244,87,122,93,227,56,230,182,246,209,211,46,255,0,69,250,234,198,236,119,26,115,85,214,86,133,145,185,73,32,145,88,76,103,11,161,164,178,178,234,118,4,4,156,
143,170,187,212,179,230,195,95,209,93,233,151,143,162,253,116,25,151,143,162,242,113,171,42,224,72,70,18,184,174,167,57,56,45,145,156,111,53,233,54,249,75,26,201,136,250,130,134,240,217,57,20,117,44,249,
176,215,244,86,58,101,227,232,188,188,104,19,47,28,26,243,111,171,10,89,81,89,74,91,81,90,1,200,3,104,3,125,96,50,75,101,65,179,205,164,237,86,54,3,71,82,223,155,13,127,69,110,67,215,89,13,41,183,25,202,
72,219,182,165,236,236,173,139,115,77,186,48,176,14,71,215,93,128,0,73,3,105,223,89,165,121,111,84,158,78,57,101,96,133,210,79,2,159,142,52,81,164,158,5,63,28,104,172,151,115,45,95,17,203,202,247,141,
253,55,244,145,61,145,170,90,171,172,123,105,149,202,247,141,253,55,244,145,61,145,170,90,171,172,123,106,245,113,37,102,230,42,102,207,33,232,182,13,34,122,51,171,101,228,197,78,170,208,112,71,202,39,
113,168,7,36,176,218,181,92,121,180,171,200,84,5,117,91,244,131,185,200,144,34,72,136,75,232,8,88,121,41,113,36,103,59,143,101,52,251,166,142,71,179,201,51,109,117,79,90,164,72,185,56,235,235,142,227,
106,97,215,87,149,41,69,105,5,58,199,120,0,231,21,191,72,158,122,27,247,73,22,245,40,77,122,230,243,78,58,54,150,91,78,8,35,242,115,146,51,85,137,119,167,39,200,101,217,179,34,20,178,66,144,211,41,75,
104,214,242,148,141,134,183,70,210,5,49,113,153,55,165,198,91,210,92,83,139,74,128,40,36,249,83,184,210,97,182,62,112,139,77,166,59,136,232,179,157,187,203,152,95,139,45,41,67,170,81,13,169,45,18,119,
246,212,52,43,173,197,26,15,53,212,92,37,165,97,113,144,146,29,35,84,21,96,227,201,92,79,105,76,215,220,100,187,58,2,82,210,29,66,18,211,41,64,28,226,117,85,187,205,81,233,156,218,45,10,183,162,91,33,
165,169,181,47,104,201,212,57,20,186,115,147,186,177,130,233,162,150,190,110,216,228,199,228,180,149,74,115,153,215,145,37,40,82,154,39,14,157,188,124,149,201,120,134,237,175,70,46,113,87,132,148,220,
35,234,234,184,23,148,16,162,146,72,242,140,85,98,100,246,101,183,13,151,164,71,83,17,117,245,16,113,189,68,19,251,42,69,58,85,37,181,73,82,37,91,213,207,243,89,75,173,37,105,28,218,117,83,176,249,169,
154,126,199,19,71,53,21,165,235,160,149,33,199,165,201,136,86,188,108,105,41,109,35,234,21,177,42,74,210,20,146,8,60,69,85,60,146,107,4,62,146,120,20,252,113,162,141,36,240,41,248,227,69,100,187,153,162,
190,35,151,149,239,27,250,111,233,34,123,35,84,181,87,88,246,211,43,149,239,27,250,111,233,34,123,35,84,181,87,88,246,213,234,226,74,205,201,189,13,125,152,246,59,227,206,195,139,33,193,48,4,169,230,146,
178,145,168,54,12,138,223,221,182,255,0,69,219,190,238,143,117,71,232,215,244,114,249,254,52,127,246,197,114,214,191,75,76,39,22,228,189,206,182,201,180,94,80,181,4,166,213,111,42,39,0,8,232,218,125,85,
44,184,183,118,208,165,175,69,155,74,82,50,73,132,156,1,246,106,158,9,4,16,72,35,104,34,165,239,119,37,187,162,22,104,130,227,32,200,92,233,11,121,2,66,181,202,74,19,140,237,221,190,169,109,49,134,28,
98,130,61,247,100,131,206,76,102,10,102,61,163,209,145,17,64,16,234,162,36,36,231,118,221,90,212,236,199,218,105,78,59,98,134,134,210,18,162,163,21,32,0,174,169,221,198,164,39,222,160,173,148,193,128,
182,145,112,141,106,97,45,200,122,74,139,56,194,131,137,82,15,123,172,6,227,140,228,214,187,158,148,177,6,37,193,112,229,199,146,183,34,91,89,40,86,20,28,27,67,128,14,7,28,70,234,205,174,11,120,123,141,
167,236,142,93,208,161,134,222,93,158,10,90,112,144,133,152,200,194,177,191,27,43,95,118,219,253,23,110,251,186,61,213,141,41,188,155,172,13,29,75,156,195,111,53,210,82,88,104,245,17,172,157,76,249,241,
198,161,171,85,85,87,52,243,28,97,136,242,189,205,250,71,121,74,173,79,4,91,160,54,113,214,75,8,7,246,85,126,196,162,171,91,42,59,206,127,109,116,95,190,108,119,178,185,172,31,52,177,216,127,109,70,248,
70,19,74,43,216,31,19,155,73,60,10,126,56,209,70,146,120,20,252,113,162,188,235,185,149,175,136,229,229,123,198,254,155,250,72,158,200,213,45,85,214,61,180,202,229,123,198,254,155,250,72,158,200,213,45,
85,214,61,181,122,184,146,179,115,128,92,231,91,226,207,137,30,50,220,68,135,195,186,192,108,234,129,252,42,63,186,215,47,205,23,229,221,83,165,32,145,158,21,231,155,78,8,198,253,245,104,202,112,88,140,
177,248,13,126,81,9,221,91,158,113,209,23,228,221,88,55,91,137,32,152,138,200,218,54,84,239,54,9,39,87,134,211,71,54,144,19,222,236,27,171,189,91,126,95,164,26,215,130,8,220,238,57,39,161,171,39,97,217,
190,177,221,43,135,230,71,135,245,125,85,60,90,26,167,41,217,156,214,121,173,108,225,36,235,108,221,71,86,223,151,233,6,181,224,129,238,165,196,156,244,53,100,103,110,43,61,213,185,254,104,191,85,78,243,
90,165,57,65,7,112,217,88,45,4,128,10,72,219,157,180,117,109,249,126,144,107,94,10,236,185,247,9,44,41,181,197,94,169,30,74,150,178,180,182,109,172,161,196,148,168,3,144,123,107,176,37,32,146,6,253,245,
234,149,185,74,90,164,242,113,203,43,4,46,146,120,20,252,113,162,141,36,240,41,248,227,69,99,187,153,106,248,142,94,87,188,111,233,191,164,137,236,141,82,213,93,99,219,76,174,87,188,111,233,191,164,137,
236,141,82,201,110,182,20,65,90,119,249,106,245,113,37,102,231,170,43,95,58,223,210,35,214,40,231,91,250,68,122,197,87,34,23,59,37,137,247,44,160,22,220,79,116,114,149,40,20,130,132,36,107,37,67,62,85,
12,84,53,225,15,70,209,155,75,47,182,166,30,233,207,161,214,213,140,130,16,157,134,162,101,204,233,115,97,190,242,154,41,136,202,26,109,33,95,146,115,147,93,157,222,113,79,203,122,75,80,101,151,164,42,
67,109,191,146,134,148,173,228,96,239,192,168,181,44,228,174,99,140,19,18,138,228,221,59,152,66,196,119,173,177,150,165,182,6,91,41,214,57,219,195,203,230,174,219,66,157,102,242,204,22,72,98,52,104,111,
186,153,78,0,66,212,164,117,198,63,170,49,159,45,84,95,184,204,150,212,237,103,25,10,148,27,108,234,103,85,182,208,114,18,159,54,210,43,174,13,249,232,40,138,211,34,42,216,97,14,160,182,230,208,176,177,
130,14,221,212,105,120,13,75,39,68,59,137,85,229,38,109,237,55,80,219,69,45,60,19,134,219,113,67,98,183,3,176,215,61,233,51,151,33,135,166,93,77,197,133,5,6,156,72,1,10,219,223,99,96,59,14,202,212,139,
200,18,91,115,185,214,116,182,218,10,121,164,37,90,170,39,113,59,120,87,44,217,206,79,148,211,178,21,25,150,217,73,75,76,48,112,132,103,126,255,0,41,20,69,99,7,27,220,205,21,175,157,111,233,17,235,20,
115,173,253,34,61,98,173,146,100,78,146,120,20,252,113,162,177,164,74,10,101,37,36,17,230,62,122,43,29,220,141,21,241,28,220,175,120,223,211,127,73,19,217,26,170,141,186,45,149,173,18,182,73,149,101,137,
38,83,218,229,110,184,140,169,93,250,134,218,183,114,189,227,127,77,253,36,79,100,106,169,205,255,0,66,236,221,139,253,245,85,232,130,156,162,164,44,158,50,99,90,197,253,158,129,254,93,122,72,178,45,42,
82,52,110,18,130,70,84,67,89,192,243,212,109,88,173,51,195,26,17,165,113,254,69,181,152,200,82,28,220,226,143,56,129,170,15,17,230,173,246,81,84,34,229,164,84,219,120,201,27,173,98,254,207,64,255,0,46,
188,184,171,23,54,172,104,244,0,112,127,244,235,128,110,21,133,245,21,217,84,94,150,175,7,53,50,183,17,212,46,93,211,153,105,12,182,148,119,168,64,192,27,233,130,244,91,12,75,77,168,174,197,9,215,157,
140,218,220,113,72,218,165,20,140,147,75,123,118,122,85,211,4,99,80,231,245,211,14,239,243,93,155,252,27,127,186,43,5,85,198,118,105,123,119,26,79,27,26,117,172,95,217,232,31,229,214,228,199,180,173,146,
234,116,94,42,154,31,215,12,18,159,93,68,213,255,0,71,24,148,141,23,67,173,206,91,250,250,252,212,51,112,67,13,36,131,185,196,43,174,15,213,90,173,166,170,227,171,73,200,182,253,202,134,181,139,251,61,
3,252,186,211,49,118,49,21,205,75,4,4,171,87,97,13,238,174,76,184,86,225,120,180,92,215,86,121,174,168,219,184,86,185,127,205,156,236,170,199,211,84,240,240,115,44,165,45,192,227,18,74,82,16,142,115,189,
66,119,36,121,168,173,104,254,107,39,210,209,94,20,247,52,35,232,14,87,188,111,233,191,164,137,236,141,82,154,70,146,60,205,162,29,188,70,112,244,109,96,78,161,198,213,19,191,235,166,207,43,222,55,244,
223,210,68,246,70,169,88,236,152,193,197,5,190,216,59,136,42,21,166,166,226,148,147,195,35,39,134,200,175,194,7,255,0,53,115,236,154,194,239,206,184,146,149,196,90,146,120,20,26,146,18,33,140,124,187,
91,14,122,226,142,145,15,4,115,237,109,255,0,188,86,143,230,45,249,126,145,204,175,4,119,225,3,255,0,154,185,246,77,96,233,3,228,17,209,92,218,63,36,212,159,73,135,172,79,62,214,79,253,194,176,36,67,26,
191,46,214,207,251,197,31,204,91,242,253,32,202,240,67,90,16,183,23,112,117,72,90,117,208,112,8,198,78,218,150,184,105,75,206,198,132,202,98,187,134,25,67,89,212,35,104,24,174,150,220,101,206,249,167,
18,160,157,167,84,236,173,106,147,20,149,107,62,222,222,26,227,101,78,13,193,230,47,185,205,89,123,17,159,132,15,254,106,231,217,53,173,235,194,158,32,187,5,75,35,118,178,51,82,221,34,30,71,203,181,176,
99,174,40,18,33,128,7,62,222,195,158,184,170,59,237,125,156,151,225,29,212,188,17,109,223,93,105,1,45,195,90,82,56,4,28,80,229,249,247,16,164,24,174,109,217,213,53,38,100,67,33,67,159,107,190,255,0,188,
87,161,42,38,124,59,95,104,81,215,183,109,95,164,25,94,10,178,80,180,194,117,75,73,78,186,242,50,49,69,74,223,156,109,200,224,178,164,169,35,242,78,120,209,94,125,139,15,5,160,242,178,58,121,94,241,191,
166,254,146,39,178,53,85,187,92,182,33,232,93,161,93,207,132,235,171,11,42,113,198,82,165,30,253,91,201,21,100,229,123,198,254,155,250,72,158,200,213,83,155,254,133,217,187,23,251,234,173,62,158,42,82,
138,100,228,241,147,167,187,109,126,139,183,125,221,30,234,238,130,228,201,236,45,232,90,61,25,246,144,112,165,183,17,36,15,254,53,87,171,21,141,232,236,218,202,164,46,10,48,246,115,34,230,228,101,99,
28,18,157,138,250,235,125,213,87,92,28,148,69,142,91,193,207,221,182,255,0,69,219,254,238,143,117,121,114,246,223,54,175,248,93,188,108,63,242,232,247,84,117,198,91,115,238,179,100,199,121,79,176,227,
164,182,226,154,13,228,121,128,217,92,203,234,43,178,169,10,42,146,79,73,206,229,110,60,131,34,117,213,122,168,71,201,236,74,0,72,27,248,10,102,63,58,60,43,53,157,40,182,64,82,149,17,178,165,41,132,18,
163,170,54,147,138,85,219,241,210,238,121,59,117,14,63,93,49,46,255,0,53,217,191,193,183,251,162,176,213,5,59,113,47,177,165,216,217,221,182,191,69,219,190,238,143,117,116,162,107,206,52,151,17,99,134,
166,212,149,44,40,69,78,10,83,214,59,183,10,174,85,182,197,164,112,163,218,222,143,114,183,178,226,34,91,229,37,14,170,66,144,87,174,65,230,240,60,190,93,251,43,85,213,66,184,234,81,201,200,247,120,201,
25,221,182,191,69,219,190,238,143,117,105,155,122,108,196,116,11,101,188,29,93,226,58,61,213,14,193,203,65,92,21,223,1,249,32,238,31,85,98,95,243,103,59,42,177,244,245,188,61,39,50,202,83,142,23,154,150,
178,18,156,187,185,35,0,81,94,17,142,139,35,31,73,69,120,51,220,208,135,255,0,43,222,55,244,223,210,68,246,70,169,73,38,245,113,106,217,18,2,34,184,83,31,88,3,141,135,42,39,248,211,111,149,239,27,250,
111,233,34,123,35,84,171,114,124,80,181,5,58,50,15,144,214,154,158,148,154,120,100,100,251,190,196,87,117,174,88,207,68,94,55,238,172,42,231,113,94,197,194,82,176,120,167,53,37,211,97,96,142,117,59,119,
236,52,116,232,121,207,58,51,140,110,53,126,189,159,63,240,115,63,68,106,110,183,16,0,76,53,1,142,2,178,110,183,50,8,49,23,196,110,169,17,54,16,213,249,68,236,221,176,208,102,194,193,5,209,180,231,113,
163,175,103,207,252,6,126,136,171,75,47,149,205,91,173,41,42,117,27,1,29,181,37,112,191,92,157,102,35,34,35,152,97,148,182,50,55,234,140,87,75,18,152,120,40,180,226,84,19,191,28,43,82,231,68,59,20,232,
245,26,156,94,142,234,93,195,83,111,98,55,186,183,63,205,23,234,172,27,173,200,131,152,138,35,178,164,140,232,103,63,42,54,239,216,104,233,208,242,15,58,157,131,27,141,83,175,103,207,252,6,126,136,225,
117,185,238,17,23,228,221,94,87,116,184,173,5,38,34,240,71,146,164,196,216,64,1,206,141,135,59,141,96,205,133,133,101,212,224,239,216,104,235,217,243,255,0,1,159,162,186,168,238,177,13,210,242,10,10,214,
20,1,162,164,111,143,180,252,116,169,149,133,164,96,100,118,209,88,44,88,120,69,160,242,178,199,87,43,222,55,244,223,210,68,246,70,170,189,108,186,59,7,66,44,225,148,55,146,23,146,82,9,235,170,172,60,
175,120,223,211,127,73,19,217,26,170,115,127,208,187,55,98,255,0,125,85,171,211,197,74,81,76,156,189,206,175,194,57,159,146,215,216,21,41,108,114,255,0,116,140,169,16,97,33,214,82,173,66,176,18,6,119,
227,109,84,42,211,101,149,8,232,211,177,174,82,44,233,97,15,151,185,185,173,58,165,231,87,25,78,166,204,246,214,251,171,140,33,170,49,18,61,223,115,154,85,242,225,18,75,145,228,54,210,30,109,90,171,78,
170,78,15,213,92,238,233,28,194,218,198,171,91,143,245,5,66,131,12,188,250,173,172,186,212,50,178,91,231,78,84,161,229,52,47,168,174,202,164,41,131,73,180,12,173,71,144,228,153,215,103,29,57,87,55,239,
166,132,155,187,240,236,182,102,217,67,64,116,70,242,117,70,79,122,41,83,110,31,141,93,54,227,188,59,60,187,233,135,119,249,174,205,254,13,191,221,21,130,152,169,91,135,246,52,187,27,127,8,230,126,75,
95,96,87,108,89,247,153,80,223,150,196,100,42,59,3,46,57,132,128,159,93,85,234,214,205,214,208,254,137,49,10,236,152,41,106,41,95,56,31,66,212,243,154,199,63,36,83,176,31,239,86,187,171,133,113,212,163,
145,99,221,145,255,0,132,115,63,37,175,176,43,68,221,34,152,168,142,140,54,50,159,200,21,18,218,210,180,149,54,218,219,111,39,81,43,222,19,195,63,85,120,151,252,217,206,202,180,105,173,225,224,225,73,
91,170,121,169,110,44,229,74,119,109,21,225,31,205,100,250,74,43,231,231,185,165,31,64,114,189,227,127,77,253,36,79,100,106,148,82,174,55,118,224,71,132,211,71,154,143,173,131,172,54,229,68,255,0,26,110,
242,189,227,127,77,253,36,79,100,106,149,165,240,162,86,136,210,150,147,144,22,134,20,160,123,8,21,166,167,165,38,158,25,41,103,47,8,134,19,47,7,255,0,75,142,55,209,211,47,24,240,94,125,245,47,206,236,
31,137,204,217,187,241,101,251,168,231,118,31,196,230,109,223,248,178,253,213,94,180,254,98,229,248,34,58,101,227,56,230,182,246,214,12,203,185,30,11,97,30,90,153,231,142,115,209,38,103,24,254,110,191,
117,99,158,192,31,138,76,1,59,191,23,94,207,213,71,90,127,48,203,240,68,90,161,200,10,154,227,237,234,173,196,20,167,111,29,181,217,58,233,121,117,152,205,243,36,33,134,146,208,202,135,1,138,235,106,83,
47,21,145,172,20,216,201,11,73,73,30,186,57,236,141,97,22,90,146,161,177,66,58,200,80,237,197,44,94,142,234,88,12,188,236,68,116,203,199,209,126,186,58,101,227,232,188,219,234,95,158,218,15,68,153,179,
119,226,203,247,81,206,236,199,67,153,140,231,249,178,253,212,221,105,252,195,47,193,17,211,47,24,39,154,221,231,172,42,85,221,96,164,179,144,118,111,169,130,238,115,152,147,54,239,252,89,123,127,85,6,
70,168,201,139,48,1,199,163,175,221,71,90,127,48,203,240,86,92,142,235,16,150,95,26,165,106,214,3,201,69,119,94,165,55,37,130,90,39,189,56,32,140,16,104,172,86,36,158,17,104,54,215,113,217,202,247,141,
253,55,244,145,61,145,170,174,219,174,178,225,104,61,153,17,221,40,78,23,187,251,234,171,23,43,222,55,244,223,210,68,246,70,170,156,223,244,46,205,216,191,223,85,106,244,201,57,197,50,114,247,55,126,17,
92,255,0,56,85,76,217,141,250,239,29,111,70,157,17,8,74,181,8,126,74,91,57,243,5,26,167,215,125,178,100,40,44,204,118,67,69,217,188,222,33,36,163,89,28,230,118,149,112,198,51,191,142,43,211,178,184,198,
45,165,220,69,185,35,114,187,94,45,215,9,16,164,200,2,68,117,150,220,9,86,176,4,121,197,114,57,164,87,50,218,135,72,86,227,80,237,7,53,117,159,113,79,62,173,174,56,163,146,179,229,38,178,190,162,187,41,
161,84,112,181,37,147,133,102,44,135,36,77,186,173,229,149,44,182,118,147,219,76,249,151,153,209,108,214,86,216,120,165,34,27,123,7,247,69,42,173,216,233,119,76,141,186,135,7,215,76,75,191,205,118,111,
240,109,254,232,175,62,152,169,91,135,246,60,140,254,17,92,255,0,56,85,74,91,222,210,9,240,157,148,203,232,13,163,112,113,192,149,56,124,136,4,229,71,178,170,149,104,180,233,4,11,117,136,34,82,150,151,
25,94,176,71,49,207,41,205,231,228,213,255,0,164,124,227,182,182,91,8,194,57,140,69,93,217,30,173,33,186,37,69,42,125,97,64,224,131,157,149,162,110,144,220,149,17,212,153,10,193,79,150,162,218,81,88,90,
202,86,157,117,169,64,45,122,196,2,115,180,215,153,127,205,156,236,170,194,184,60,60,28,41,42,113,78,179,41,110,18,165,41,220,146,104,175,40,254,107,35,210,81,95,61,61,205,40,127,242,189,227,127,77,253,
36,79,100,106,147,242,157,188,8,172,68,107,83,153,100,168,35,190,242,146,127,141,56,57,94,241,191,166,254,146,39,178,53,75,85,117,143,109,106,171,56,77,60,17,156,176,202,230,111,24,39,89,189,155,251,234,
207,252,103,56,202,51,253,234,178,71,142,228,151,210,204,118,203,142,172,224,33,35,37,70,186,164,218,103,70,46,116,136,111,54,80,141,117,107,39,24,78,113,147,230,205,91,93,159,55,249,23,95,209,80,6,240,
64,57,70,221,221,245,7,187,24,57,83,120,206,58,213,107,141,111,149,36,54,99,198,113,192,181,20,35,85,57,202,128,201,3,234,175,83,237,83,109,232,74,166,196,117,132,172,225,37,105,198,77,115,93,159,55,249,
13,111,193,87,181,193,146,217,148,95,0,41,228,99,97,221,190,183,78,145,122,113,12,54,75,97,12,182,27,79,125,192,12,84,219,204,58,193,72,121,10,65,82,66,211,172,55,131,184,215,111,112,174,106,141,207,244,
9,5,141,93,125,125,77,152,223,154,88,173,28,94,3,83,206,197,52,247,96,103,190,111,102,51,223,86,63,227,27,54,163,36,103,173,86,133,194,125,18,16,194,216,88,121,192,146,148,17,181,64,238,245,215,92,123,
21,206,74,57,198,32,62,226,1,41,214,74,51,180,28,17,77,174,207,155,252,134,167,224,166,142,236,31,235,55,191,29,106,242,174,235,148,168,18,140,1,183,190,171,59,241,93,97,40,83,205,41,9,94,117,74,134,51,
131,131,250,235,86,6,221,131,109,119,85,143,254,111,242,26,254,138,155,240,221,135,13,97,236,107,45,65,88,7,52,84,158,146,120,20,252,113,162,176,216,180,203,8,180,30,86,71,47,43,222,55,244,223,210,68,
246,70,169,106,174,177,237,166,87,43,222,55,244,223,210,68,246,70,169,86,228,248,201,90,129,116,100,31,33,171,212,210,138,201,41,166,217,58,193,121,26,61,33,86,224,122,90,158,213,121,104,25,83,108,227,
173,230,239,176,51,94,45,173,148,104,246,146,172,173,213,56,98,32,101,107,42,62,21,62,90,137,137,122,110,35,201,122,52,158,109,196,238,32,19,250,177,94,198,145,200,51,68,167,174,138,117,228,167,85,36,
182,0,31,80,24,63,93,118,88,97,28,162,219,96,32,68,177,235,41,73,79,76,145,172,83,191,28,193,170,130,186,27,143,54,168,46,220,31,101,40,4,57,47,97,39,142,6,72,160,105,11,230,224,212,215,174,171,118,67,
64,134,148,91,3,83,35,25,0,12,110,53,226,85,245,233,165,177,58,232,167,219,108,146,16,90,74,71,234,2,149,114,212,198,111,251,112,137,109,37,201,189,198,27,113,220,232,255,0,176,215,94,154,42,25,145,29,
28,253,201,83,12,54,1,101,189,140,132,148,239,200,63,194,162,134,145,92,36,176,91,141,117,82,88,67,124,217,72,109,7,9,199,148,140,214,167,116,158,114,152,12,119,101,97,160,216,107,28,210,78,64,24,223,
171,154,26,4,203,43,107,97,199,227,223,93,113,72,17,152,1,196,13,168,66,154,0,54,146,124,171,197,71,92,30,140,189,29,181,189,49,251,130,37,60,100,188,211,49,122,170,214,115,61,241,200,59,51,80,10,185,
180,168,10,135,211,8,101,107,66,214,53,122,197,59,179,178,187,27,210,121,81,227,180,196,59,161,97,150,243,170,144,210,85,191,180,81,36,189,129,63,39,93,225,58,144,180,109,40,214,9,232,207,28,18,78,222,
115,141,113,208,206,148,78,98,59,108,179,121,90,82,140,227,228,82,119,156,157,233,174,67,114,140,73,82,223,5,71,105,56,59,105,224,210,236,36,150,78,13,36,240,41,248,227,69,106,190,72,106,68,112,166,22,
22,6,195,138,43,53,188,139,87,196,117,114,189,227,127,77,253,36,79,100,106,171,214,203,163,176,116,34,206,25,67,121,33,121,37,32,147,223,170,172,60,175,120,223,211,127,73,19,217,26,170,115,127,208,187,
55,98,255,0,125,85,167,211,197,74,81,76,73,123,157,63,132,115,63,37,175,176,43,166,61,214,235,38,60,137,12,71,66,217,142,157,119,86,16,48,129,156,100,253,100,85,114,166,161,93,196,109,12,210,91,123,242,
10,81,34,58,57,150,177,214,115,156,78,113,245,10,244,109,174,16,139,146,136,139,187,15,194,57,127,146,215,216,21,229,205,34,152,91,88,213,107,113,254,160,168,97,184,86,23,212,87,101,81,81,95,131,133,106,
60,133,201,157,117,113,211,149,115,126,250,103,201,187,189,14,203,102,109,148,52,7,68,111,39,80,100,247,162,149,54,253,94,151,115,206,253,67,143,215,76,75,191,205,118,111,240,109,254,232,175,58,152,169,
91,135,246,60,187,27,63,8,230,126,75,95,96,84,156,119,175,210,109,230,115,16,208,184,160,19,174,2,120,111,217,190,170,85,107,180,57,98,137,102,101,231,46,118,232,183,133,107,20,174,91,78,173,76,40,30,
245,72,212,24,243,237,173,119,66,21,199,82,72,88,172,178,60,233,20,192,112,82,214,127,184,43,68,221,33,150,168,142,140,52,50,159,200,21,16,218,249,205,117,244,131,36,149,168,151,72,198,185,206,250,243,
47,249,179,157,149,88,83,91,195,192,20,149,186,167,154,150,226,206,84,167,114,104,175,40,199,69,145,143,164,162,188,9,238,104,67,255,0,149,239,27,250,111,233,34,123,35,84,162,147,113,187,162,12,104,45,
50,121,166,10,130,78,119,229,68,255,0,26,110,242,189,227,127,77,253,36,79,100,106,150,170,235,30,218,213,86,82,77,60,17,155,195,43,221,50,241,140,243,91,59,104,233,151,140,227,154,219,219,83,218,137,219,
176,109,223,231,173,172,199,113,245,16,203,43,113,64,109,212,73,39,31,85,91,169,111,205,139,175,232,174,9,183,130,60,23,159,125,6,93,227,4,22,188,219,234,196,184,235,104,14,113,165,32,36,234,247,201,35,
7,201,94,155,132,243,189,235,113,220,94,70,182,18,130,118,121,104,234,89,243,97,175,232,174,90,98,200,10,150,167,155,213,83,200,192,243,111,174,185,247,75,195,173,198,107,153,33,12,52,150,211,149,13,160,
12,84,187,140,186,219,129,183,26,90,28,59,146,164,144,125,85,153,17,93,100,14,144,203,141,131,187,93,36,103,215,73,20,227,222,47,184,107,250,43,125,50,241,244,95,174,142,153,120,250,47,215,86,5,52,66,
117,212,130,18,189,128,145,176,214,197,69,117,11,108,41,135,18,165,14,240,20,16,85,217,229,167,234,89,243,97,175,232,173,137,151,142,13,113,198,250,242,185,119,117,32,133,53,179,27,118,213,132,182,19,
222,148,227,7,118,55,26,198,170,78,118,13,187,252,244,107,179,228,195,95,209,82,114,43,177,161,185,207,167,85,78,40,43,20,84,174,145,248,20,252,113,162,176,88,180,203,5,160,242,178,57,121,94,241,191,166,
254,146,39,178,53,75,85,117,143,109,50,249,95,24,229,123,77,79,149,200,158,200,213,43,92,151,29,43,80,83,237,130,14,226,161,90,106,127,218,74,205,205,213,45,114,126,68,8,86,230,173,78,185,29,15,179,206,
200,125,163,133,21,100,141,77,97,187,112,56,168,30,155,27,243,134,190,208,174,200,58,66,96,54,226,99,72,136,160,173,201,125,41,113,32,249,64,60,105,229,134,44,123,29,151,37,58,52,98,218,165,190,235,171,
114,226,225,90,156,86,177,87,201,141,230,167,29,145,204,198,152,87,112,122,3,105,181,68,60,243,73,42,82,126,81,91,128,219,183,117,85,227,105,35,236,169,247,12,155,123,142,56,173,112,149,178,146,218,15,
149,41,220,43,17,180,141,246,31,151,32,203,130,227,242,82,148,40,56,218,84,216,74,78,64,8,59,6,13,74,81,109,97,20,77,45,205,214,183,186,77,250,59,221,46,84,177,207,161,9,114,65,57,32,40,99,97,218,42,79,
72,39,166,68,249,214,216,147,95,185,78,145,55,85,28,242,20,148,70,1,123,82,146,118,121,182,121,42,21,55,89,18,167,137,174,61,21,75,111,84,164,48,208,66,65,7,57,192,173,12,92,208,204,245,203,84,166,76,
133,62,95,206,64,26,196,231,117,54,156,224,92,227,37,230,251,100,73,181,173,152,174,199,113,112,16,149,161,9,150,130,70,70,94,42,25,224,70,202,238,74,205,219,74,225,70,25,92,155,122,99,45,132,131,170,
158,104,160,23,9,242,156,227,20,181,139,112,110,59,179,94,68,152,221,34,88,117,43,94,205,206,111,253,181,213,22,254,228,123,178,110,41,156,200,144,148,54,216,41,32,12,33,32,0,71,29,212,186,91,221,141,
169,45,141,97,74,91,242,138,137,39,159,115,105,254,241,175,85,198,212,168,233,11,43,148,210,148,181,169,100,133,1,180,156,214,206,155,27,243,134,190,208,170,197,225,44,146,107,44,141,210,79,2,159,142,
52,87,139,251,173,186,192,45,45,43,30,84,156,241,162,178,221,200,209,95,17,211,202,254,127,149,221,53,206,238,114,38,62,232,213,86,173,114,216,135,161,118,149,119,58,19,174,172,44,169,199,25,74,148,123,
245,111,36,85,155,150,15,27,154,105,233,34,123,35,85,76,111,250,23,102,236,95,239,170,175,233,226,165,40,166,36,158,50,116,247,109,175,209,118,239,187,163,221,91,89,185,151,155,117,198,172,208,86,134,
147,172,226,147,25,4,36,103,25,59,42,2,167,173,247,84,199,208,141,39,130,235,205,54,29,142,133,52,8,1,74,95,56,157,128,239,221,194,189,27,105,174,17,114,81,17,101,188,100,215,221,182,191,69,219,190,238,
143,117,121,114,246,223,54,175,248,93,184,108,63,242,232,247,84,48,220,43,11,234,43,178,168,189,61,94,14,101,149,184,242,12,137,215,85,234,161,31,39,177,40,72,72,27,248,10,102,63,58,60,43,53,157,40,182,
64,82,149,17,178,165,41,132,18,163,170,54,147,138,85,219,241,210,238,121,59,117,14,63,93,49,46,255,0,53,217,191,193,183,251,162,188,250,160,167,110,37,246,60,187,27,59,182,215,232,187,119,221,209,238,
174,230,92,152,244,19,49,173,30,140,184,128,18,93,76,68,148,128,55,237,213,170,189,91,173,61,14,45,146,60,132,221,32,137,234,215,82,25,153,61,76,165,146,14,206,240,100,44,29,231,34,181,93,85,117,199,86,
147,145,203,100,95,118,218,253,23,110,251,186,61,213,162,109,233,179,21,208,45,150,240,117,119,136,232,247,84,82,28,46,149,173,79,54,242,148,181,18,182,241,170,78,120,99,133,120,151,252,217,206,202,172,
40,169,225,224,230,89,74,113,194,243,82,214,66,83,151,119,36,96,10,43,194,49,209,100,99,233,40,175,6,123,154,17,244,15,44,30,55,52,211,210,68,246,70,169,69,38,245,113,106,217,18,222,136,174,20,199,214,
0,227,97,202,137,254,52,215,229,151,164,43,149,237,54,230,11,24,74,226,103,156,39,39,241,70,183,98,150,202,106,65,39,43,139,235,53,166,166,210,77,110,70,79,12,131,238,181,203,25,232,139,198,253,212,27,
165,201,91,12,53,30,24,34,166,122,59,216,35,90,46,221,251,77,28,195,249,206,188,92,227,27,205,95,175,111,203,244,142,106,94,8,110,235,92,191,52,95,151,117,6,235,114,32,131,17,124,70,234,153,17,222,26,
189,244,93,155,182,154,12,119,136,35,90,46,211,158,177,163,175,111,201,254,16,106,94,8,43,75,47,21,205,91,173,41,42,117,27,1,29,181,37,112,191,92,157,102,35,34,35,152,97,148,182,50,55,234,140,87,111,53,
32,127,94,47,172,214,20,195,234,29,242,227,31,172,212,226,220,59,197,247,57,168,133,238,173,207,243,69,250,171,202,238,87,5,237,92,34,172,121,83,154,155,49,223,57,239,162,237,223,180,209,209,223,200,58,
209,118,12,111,53,78,181,191,47,210,59,169,120,33,83,116,185,36,97,48,212,7,144,10,194,238,151,21,160,164,196,94,8,242,84,216,142,240,0,107,70,216,115,214,52,24,239,16,174,250,46,221,251,77,29,107,126,
79,240,131,82,42,138,142,235,16,221,47,32,160,173,97,64,26,42,87,72,155,117,44,39,93,108,159,238,147,229,162,176,78,56,120,43,7,149,145,205,202,247,141,253,55,244,145,61,145,170,81,119,84,173,107,230,
98,62,226,65,35,89,56,197,55,121,94,241,191,166,254,146,39,178,53,80,28,137,67,98,82,37,23,153,14,97,103,127,109,105,166,46,109,69,60,19,158,59,182,80,196,247,201,192,183,74,207,96,172,244,217,31,163,
101,121,119,10,250,46,67,218,47,103,146,196,75,171,172,177,33,228,107,182,158,105,74,58,185,198,118,3,198,172,247,88,26,39,102,137,21,251,139,177,155,68,162,18,209,74,10,242,127,246,131,129,231,59,42,
150,37,7,135,47,208,169,103,100,124,152,103,72,27,237,210,183,227,112,163,167,191,156,119,58,86,123,5,125,83,62,217,163,137,185,155,115,42,138,169,124,208,144,27,78,245,32,156,5,14,7,104,170,247,67,180,
201,144,247,67,75,110,22,31,83,46,97,56,33,99,122,126,170,165,116,245,22,99,47,209,198,210,246,62,127,231,220,109,110,34,84,119,99,184,134,195,165,14,99,58,167,113,253,85,34,221,158,254,236,118,159,106,
195,49,108,186,144,182,214,10,48,164,157,160,141,181,41,202,234,16,222,156,206,67,105,8,72,183,53,176,118,42,174,86,230,36,75,177,218,115,47,162,197,106,19,69,110,168,21,4,247,163,27,6,211,94,111,173,
245,51,163,26,59,149,133,113,150,226,235,184,186,67,250,2,111,218,71,190,142,226,233,15,232,9,191,105,30,250,189,199,13,200,122,82,155,189,180,96,70,0,59,48,161,97,9,81,220,156,17,156,236,60,43,217,97,
213,33,199,227,220,208,244,36,199,114,66,36,0,160,151,2,49,172,0,35,59,9,242,87,158,255,0,137,250,133,255,0,15,247,242,83,161,2,131,220,93,33,253,1,55,237,35,223,94,92,180,95,219,66,150,187,12,192,148,
140,147,172,143,125,93,173,74,149,113,119,8,148,91,101,40,231,28,117,71,189,109,28,84,124,213,230,237,145,110,114,68,27,162,39,197,10,230,150,235,97,73,9,94,240,158,248,2,118,109,169,199,248,197,178,139,
154,143,101,254,249,59,208,136,161,189,72,231,218,90,84,218,155,113,179,170,164,43,120,52,87,11,234,42,76,194,163,146,94,162,189,55,39,46,236,88,172,118,67,239,149,239,27,250,111,233,34,123,35,85,7,200,
146,212,220,105,106,72,86,66,206,7,13,245,57,202,247,141,253,55,244,145,61,145,170,160,104,245,254,249,163,140,169,139,91,144,3,90,196,229,214,10,149,191,137,205,108,244,239,75,82,100,167,223,40,108,220,
45,178,174,122,104,204,180,202,159,10,26,32,150,11,177,156,9,43,86,190,117,14,70,236,86,203,141,150,67,250,64,23,108,126,117,178,218,205,171,162,181,208,157,13,235,171,92,157,67,156,236,219,154,94,14,
81,116,184,127,234,218,126,234,127,221,71,242,141,165,223,75,105,251,169,255,0,117,104,148,170,147,239,22,34,109,123,150,61,30,209,185,105,118,3,151,22,156,141,38,29,165,168,204,73,73,5,109,60,149,168,
156,99,204,112,124,198,165,244,66,53,202,35,55,36,221,117,76,153,19,214,249,90,18,66,22,14,54,129,188,110,227,84,95,229,27,75,190,150,211,247,83,254,234,242,174,80,244,184,159,13,107,29,145,143,251,171,
181,217,8,62,201,131,239,238,105,229,140,164,233,244,220,141,189,206,107,30,165,85,170,223,118,143,106,178,218,159,82,103,46,71,66,105,45,152,111,37,181,39,41,0,237,52,185,189,207,159,125,185,191,62,236,
227,10,148,243,33,143,145,108,165,33,41,206,54,100,249,106,37,225,118,45,182,216,158,53,89,72,66,64,73,24,3,235,175,43,214,211,101,216,112,243,158,229,107,154,91,140,196,94,91,158,212,198,238,176,150,
152,14,173,14,134,89,90,82,242,150,222,112,84,173,196,156,228,214,81,123,140,252,207,149,183,73,110,214,204,39,226,37,142,117,60,242,185,220,18,117,183,112,165,111,55,119,200,29,63,120,39,113,247,208,
27,187,16,15,79,222,113,184,251,235,4,125,23,170,139,202,107,241,250,43,213,128,206,143,116,183,235,174,44,123,108,216,246,247,227,170,60,142,121,244,41,101,39,29,82,55,29,149,199,123,184,179,42,218,213,
186,217,5,248,144,154,120,72,90,164,184,149,169,74,72,41,0,99,118,195,75,210,221,220,5,30,159,213,223,176,251,235,37,139,178,198,170,167,100,31,40,59,189,116,171,248,125,235,178,194,95,227,59,227,254,
206,245,98,67,57,181,185,103,129,118,138,232,157,13,81,34,171,156,80,82,220,86,177,197,21,233,73,56,246,98,69,231,186,30,156,175,120,223,211,127,73,19,217,26,165,52,171,172,72,242,22,211,171,33,105,59,
70,41,179,202,247,141,253,55,244,145,61,145,170,249,251,73,62,118,127,182,172,166,225,4,208,142,42,82,121,44,125,220,131,244,135,236,209,221,200,63,72,126,205,82,40,165,235,200,239,74,37,223,187,144,126,
144,253,154,59,185,7,233,15,217,171,38,142,114,123,10,55,39,205,233,69,229,184,183,49,57,193,30,52,86,174,9,101,109,40,227,85,71,25,42,86,78,212,99,96,219,85,155,255,0,38,183,251,4,121,139,186,8,76,189,
21,254,96,198,50,83,207,184,114,0,40,70,245,36,147,176,141,148,117,228,29,40,158,187,185,7,233,15,217,163,187,144,126,144,253,154,230,159,201,245,246,19,48,28,113,17,92,50,220,67,60,219,79,133,174,58,
214,64,72,121,35,107,100,147,142,251,207,91,180,147,147,123,230,143,72,101,169,174,91,156,215,120,71,113,113,229,165,212,176,178,112,3,164,108,70,223,47,144,209,215,144,116,162,123,238,228,31,164,63,102,
142,238,65,250,67,246,107,187,150,157,7,99,66,111,54,230,97,56,194,163,201,130,203,133,40,146,29,80,115,81,37,100,227,114,73,57,30,81,186,151,84,117,228,29,40,151,126,238,65,250,67,246,104,238,228,31,
164,63,102,169,20,81,215,144,116,162,90,175,82,17,38,42,92,104,43,80,238,42,73,25,219,194,138,247,116,254,141,89,253,1,253,243,69,78,111,83,203,26,43,11,3,163,149,239,27,250,111,233,34,123,35,85,243,246,
146,124,236,255,0,109,125,3,202,247,141,253,55,244,145,61,145,170,89,53,10,212,94,121,203,133,188,75,113,106,200,37,213,35,84,121,54,85,116,185,65,36,38,165,25,60,139,122,41,157,208,244,119,244,2,126,
244,186,58,30,142,254,128,79,222,151,75,209,152,221,72,145,80,180,154,222,206,129,216,173,43,46,244,184,119,181,79,116,4,119,188,209,74,70,195,196,236,59,43,86,158,233,99,119,158,84,167,233,28,71,31,147,
19,167,244,136,201,124,144,121,176,173,100,167,7,58,163,205,83,93,15,71,127,64,39,239,75,173,137,182,216,20,1,252,31,0,30,38,82,232,232,207,192,117,34,119,77,211,173,31,98,100,203,132,23,165,63,38,253,
112,141,54,226,202,217,212,76,62,109,224,225,74,21,159,148,226,56,110,168,91,206,151,90,229,219,52,253,134,84,247,57,122,184,181,38,38,81,128,80,151,20,163,173,228,56,34,187,141,170,192,14,13,133,176,
127,197,174,188,170,219,96,4,142,224,35,102,255,0,198,151,71,66,126,3,169,18,185,202,22,144,195,210,133,218,238,77,235,181,61,17,27,134,252,125,92,161,9,105,1,8,80,87,29,96,50,70,54,85,66,153,198,22,142,
131,131,96,79,222,151,71,67,209,223,208,9,251,210,232,232,207,192,117,34,44,104,166,119,67,209,223,208,9,251,210,232,232,122,59,250,1,63,122,93,29,25,135,82,37,122,233,253,26,179,250,3,251,230,138,236,
210,247,163,46,44,86,161,196,17,89,101,26,137,64,89,95,245,179,188,246,209,73,36,211,195,59,23,158,232,110,114,189,227,127,77,253,36,79,100,106,150,170,235,30,218,101,114,189,227,127,77,253,36,79,100,
106,150,170,235,30,218,213,87,18,22,110,98,178,132,149,168,36,111,53,138,222,20,150,89,215,27,73,29,111,33,171,34,103,176,27,101,176,85,141,125,251,107,137,219,143,124,123,226,71,155,117,17,35,61,116,
145,222,156,52,147,223,19,192,84,245,146,60,103,110,98,221,106,100,72,150,119,169,99,189,21,57,218,151,97,227,6,202,209,126,65,36,128,176,217,219,186,183,198,152,20,160,92,81,206,14,118,83,34,94,137,222,
155,104,147,34,26,112,7,123,252,55,85,2,252,190,230,92,58,37,217,148,161,100,18,151,91,220,65,169,171,215,184,238,166,99,92,56,222,204,100,158,63,194,181,29,134,181,176,83,189,11,14,53,253,82,43,106,182,
237,242,240,171,166,154,201,38,98,138,40,160,225,11,164,158,5,63,28,104,163,73,60,10,126,56,209,88,238,230,105,175,136,229,229,123,198,254,155,250,72,158,200,213,45,85,214,61,180,202,229,123,198,254,155,
250,72,158,200,213,45,85,214,61,181,122,184,146,179,115,9,25,32,110,175,51,130,222,144,219,8,36,231,246,87,180,156,40,17,89,138,164,247,88,45,71,58,131,121,242,213,36,241,22,42,44,90,63,111,19,110,108,
90,219,58,145,154,249,73,78,121,7,147,245,84,142,147,221,160,243,203,110,198,219,112,226,181,242,106,144,142,187,157,158,106,167,219,116,145,108,218,175,168,66,20,29,89,24,116,121,10,133,112,72,124,150,
25,105,39,189,74,70,209,198,176,55,151,150,106,75,8,232,126,75,42,10,64,117,245,100,231,92,171,109,104,238,153,138,180,137,160,75,138,118,5,175,123,121,174,151,236,55,6,108,109,93,156,97,73,132,226,202,
2,207,151,103,190,160,231,96,198,88,62,78,53,195,167,169,132,91,46,141,56,194,181,162,73,220,158,3,60,127,93,78,55,223,39,134,14,220,213,38,92,213,57,108,105,181,12,156,224,31,38,42,229,13,37,112,153,
192,81,58,169,253,149,167,211,201,236,70,213,142,230,53,143,58,164,17,176,113,28,43,221,107,112,145,52,129,176,20,238,173,149,160,137,11,164,158,5,63,28,104,163,73,60,10,126,56,209,88,238,230,104,175,
136,229,229,123,198,254,155,250,72,158,200,213,45,85,214,61,180,202,229,123,198,254,155,250,72,158,200,213,45,85,214,61,181,122,184,146,179,115,0,224,228,87,130,117,95,214,198,53,147,128,69,123,163,133,
85,164,196,76,175,196,144,67,23,11,115,167,85,74,57,7,203,83,28,158,219,93,210,27,204,107,112,4,169,7,229,73,224,145,188,212,69,254,18,150,226,100,198,216,234,118,29,155,234,227,161,247,235,94,138,104,
12,235,139,15,5,233,4,210,88,9,198,20,216,217,159,218,107,12,226,226,240,106,140,181,34,99,149,125,32,97,114,25,176,219,92,74,109,182,241,169,132,157,138,94,226,127,80,165,77,214,106,117,11,109,156,147,
178,162,158,146,251,235,82,221,117,75,82,142,73,81,218,77,105,10,58,221,238,210,118,82,12,118,148,151,3,17,147,133,41,71,213,154,191,70,71,55,13,180,149,96,234,128,79,148,10,174,104,205,177,92,240,126,
70,197,17,222,236,169,219,155,197,180,134,208,53,79,1,190,182,81,29,43,83,51,217,44,188,26,91,58,242,86,161,146,6,192,107,162,181,71,70,163,99,59,206,210,107,109,85,19,33,116,147,192,167,227,141,20,105,
39,129,79,199,26,43,29,220,205,21,241,28,188,175,120,223,211,127,73,19,217,26,165,170,186,199,182,153,92,175,120,223,211,127,73,19,217,26,165,170,186,199,182,175,87,18,86,110,98,176,162,64,216,50,124,
148,40,225,36,249,5,87,215,164,137,74,138,75,27,65,198,250,121,77,71,113,84,92,182,39,92,45,184,149,5,235,100,12,96,84,28,251,99,110,18,162,156,31,40,242,214,165,233,19,106,32,150,8,62,101,86,183,47,233,
86,117,90,35,39,39,39,53,41,78,18,220,164,99,36,115,27,65,214,227,131,199,59,170,74,221,106,105,42,42,86,50,55,102,185,59,189,223,19,205,39,111,12,108,172,170,255,0,173,176,180,49,230,217,74,186,105,228,
103,173,150,99,33,12,52,16,223,168,28,214,182,80,86,190,113,221,167,128,242,84,3,119,246,80,118,71,63,89,173,159,132,201,250,3,235,170,117,99,156,228,158,137,120,44,116,87,60,7,156,147,13,185,43,108,33,
183,51,169,223,103,56,56,53,209,84,77,53,148,35,88,236,200,93,36,240,41,248,227,69,26,73,224,83,241,198,138,201,119,51,69,124,71,47,43,222,55,244,223,210,68,246,70,169,106,174,177,237,166,87,43,222,55,
244,223,210,68,246,70,169,106,174,177,237,171,213,196,149,155,158,23,212,87,101,46,223,240,203,237,166,34,250,138,236,165,219,254,25,125,180,158,163,216,122,125,198,199,35,54,253,28,110,193,164,122,69,
113,150,226,110,246,134,67,172,182,184,97,246,155,10,80,72,81,74,142,23,146,113,130,54,111,174,125,38,209,59,75,108,69,210,29,40,191,162,51,151,168,73,156,204,120,49,82,84,30,80,36,160,182,8,212,70,236,
43,183,102,202,165,104,254,146,57,102,177,233,5,181,184,232,117,23,118,16,194,214,165,16,91,9,88,94,64,227,187,21,227,74,52,129,203,248,180,135,24,67,61,207,128,212,20,234,171,58,225,25,239,143,144,156,
238,172,197,139,84,78,78,27,145,109,106,48,184,44,105,67,240,141,205,152,92,216,230,76,93,82,189,98,238,118,43,84,43,189,197,119,220,249,49,182,162,35,17,45,119,169,18,116,145,118,196,93,85,17,216,225,
12,165,158,111,156,87,202,100,228,132,231,3,27,106,30,47,40,242,99,219,91,41,183,180,111,172,197,232,13,93,57,195,148,70,213,41,230,249,188,106,158,244,145,173,191,109,122,111,148,153,40,210,36,93,187,
156,201,90,44,198,205,205,243,135,26,188,209,111,95,56,223,183,56,160,9,141,55,182,104,204,126,71,52,70,85,178,115,238,220,22,227,248,214,136,17,206,157,97,206,5,43,57,194,14,193,229,243,82,162,167,157,
210,37,200,209,38,108,82,227,33,228,197,116,185,17,237,98,146,192,81,203,131,3,173,172,113,191,118,42,6,128,24,86,127,232,237,183,251,171,253,234,232,174,123,63,244,118,219,253,213,254,245,116,86,218,
184,35,45,156,153,11,164,158,5,63,28,104,163,73,60,10,126,56,209,89,238,230,90,190,35,151,149,239,27,250,111,233,34,123,35,84,181,87,88,246,211,43,149,239,27,250,111,233,34,123,35,84,181,87,88,246,213,
234,226,74,205,207,39,24,193,174,248,119,89,17,35,161,150,68,112,218,6,6,179,40,39,214,69,66,88,44,159,132,26,65,46,59,139,80,8,86,194,14,225,129,87,150,57,40,142,188,19,37,126,179,83,157,241,79,18,67,
198,167,140,166,67,247,122,103,254,55,221,209,238,163,187,211,63,241,190,238,143,117,90,163,242,53,25,220,102,74,246,249,9,169,70,57,7,140,224,200,148,225,29,166,167,252,197,126,6,233,75,201,65,238,244,
207,252,111,187,163,221,71,119,166,127,227,125,221,30,234,102,39,255,0,211,211,10,72,34,75,152,62,115,92,207,242,4,195,36,235,73,112,96,99,4,154,103,116,23,252,67,167,47,34,243,187,211,63,241,190,238,
143,117,29,222,153,255,0,141,247,116,123,170,223,39,145,72,204,255,0,205,44,253,102,163,36,114,73,25,178,113,41,207,93,47,243,21,248,14,148,188,149,153,247,7,231,150,250,66,144,121,176,66,66,80,18,7,170,
185,106,221,109,228,182,33,184,54,135,222,90,145,197,37,71,109,82,32,167,81,133,35,36,234,45,105,25,242,5,17,87,170,213,55,132,137,206,183,30,236,142,210,79,2,159,142,52,81,164,158,5,63,28,104,168,221,
204,165,124,71,47,43,222,55,244,223,210,68,246,70,169,106,174,177,237,166,87,43,222,55,244,223,210,68,246,70,169,106,174,177,237,171,213,196,149,155,157,188,154,16,52,178,113,60,21,252,5,60,33,40,2,141,
190,122,68,232,52,171,117,187,72,39,63,118,154,34,37,75,239,114,149,29,97,129,228,6,154,44,105,158,137,183,143,248,226,49,232,215,254,218,195,118,92,154,52,195,24,67,30,3,169,26,185,59,184,87,190,77,244,
131,159,114,252,155,140,192,162,205,209,230,89,11,80,26,168,26,186,169,30,106,163,179,167,218,34,140,127,199,155,255,0,45,127,237,168,213,221,121,45,118,226,185,207,201,134,185,107,115,157,83,188,211,
185,43,206,115,187,125,77,69,173,199,202,45,81,244,207,73,123,173,111,106,223,37,18,137,188,220,219,91,15,245,93,101,164,164,165,3,27,114,54,234,249,206,218,174,114,129,202,29,206,115,218,66,187,52,249,
177,155,109,118,246,211,29,160,144,227,11,90,136,117,3,88,99,91,135,146,164,89,211,62,79,35,202,110,67,23,56,232,121,14,56,242,86,26,115,41,90,198,22,173,219,206,54,215,29,219,73,185,55,186,116,147,62,
124,73,6,78,167,60,84,211,157,254,167,87,59,56,102,153,127,209,206,222,73,125,10,233,106,182,189,34,227,62,231,33,199,21,142,102,224,90,43,107,28,126,79,102,218,234,154,180,149,29,187,42,171,108,210,46,
79,44,225,209,105,185,70,138,29,35,92,33,183,54,227,118,241,94,164,105,174,137,43,171,125,65,255,0,248,215,254,218,71,25,111,131,185,68,213,185,67,186,72,32,138,64,69,234,187,233,156,253,243,77,184,218,
101,162,140,74,14,11,210,14,56,243,107,255,0,109,40,225,29,102,150,161,185,78,45,64,249,65,81,34,181,122,84,245,50,55,236,136,221,36,240,41,248,227,69,26,73,224,83,241,198,138,107,185,156,175,136,229,
229,123,198,254,155,250,72,158,200,213,45,85,214,61,180,202,229,123,198,254,155,250,72,158,200,213,45,85,214,61,181,122,184,146,179,115,153,82,91,10,80,8,117,122,167,10,40,108,168,3,245,10,243,210,219,
250,39,255,0,201,87,186,172,26,41,113,147,2,193,125,49,151,170,76,192,127,248,38,181,254,19,221,62,158,166,238,146,120,40,170,77,16,125,45,191,162,127,252,149,123,168,233,109,253,19,255,0,228,171,221,
86,107,109,230,243,113,154,220,88,210,17,206,185,156,115,139,8,79,214,73,192,169,107,187,26,79,105,137,210,38,63,20,54,66,74,66,36,161,74,80,39,0,128,14,79,213,92,235,200,58,72,161,244,182,254,137,255,
0,242,85,238,163,165,183,244,79,255,0,146,175,117,93,36,175,73,163,183,17,78,41,7,164,169,41,74,82,176,165,36,146,0,11,0,229,59,248,226,182,220,91,210,139,124,150,88,125,109,41,199,92,13,0,211,169,94,
170,137,192,10,193,239,118,249,104,235,200,58,72,163,116,182,254,137,255,0,242,85,238,163,165,183,244,79,255,0,146,175,117,93,239,95,132,118,104,202,122,116,168,129,41,88,65,74,36,161,106,7,201,170,21,
154,132,252,39,186,125,61,29,121,7,73,16,75,154,202,18,84,180,60,148,142,37,165,15,225,93,12,184,135,155,11,108,229,39,113,173,183,235,253,198,77,177,214,221,124,148,145,186,163,108,39,54,166,73,243,254,
218,165,86,57,188,49,39,5,21,216,229,210,79,2,159,142,52,81,164,158,5,63,28,104,168,221,204,165,124,71,47,43,222,55,244,223,210,68,246,70,169,106,174,177,237,166,87,43,222,55,244,223,210,68,246,70,169,
106,174,177,237,171,213,196,149,155,157,54,63,232,253,243,252,96,253,193,81,181,196,249,185,48,153,76,196,212,230,95,119,156,218,172,109,198,63,133,112,226,237,130,112,222,205,253,245,103,148,37,151,216,
180,100,176,187,147,68,100,96,238,169,123,148,232,50,116,134,196,234,18,75,48,97,70,105,194,81,213,82,20,74,177,229,170,118,46,217,198,171,121,254,245,99,254,45,179,189,111,110,237,180,186,37,224,238,
165,228,105,220,116,225,137,175,94,212,227,81,217,105,219,140,117,199,91,17,117,28,113,180,60,20,162,179,199,103,150,162,229,105,75,183,29,43,233,4,71,139,106,77,192,60,174,141,31,154,91,201,74,242,20,
188,117,143,26,160,145,118,28,27,223,142,176,160,139,176,254,171,124,63,173,93,211,47,1,169,121,47,58,113,112,179,93,102,174,84,73,177,93,119,158,81,67,44,218,148,194,136,82,179,149,47,113,35,203,80,53,
8,5,216,255,0,85,190,60,104,2,236,120,55,191,29,97,92,209,47,1,169,121,59,238,191,204,93,236,175,118,15,154,88,236,63,182,162,157,106,232,243,106,66,208,130,49,180,102,166,173,44,46,52,6,154,115,26,233,
27,113,219,86,166,45,62,232,157,141,53,216,225,210,79,2,159,142,52,81,164,158,5,63,28,104,164,187,152,213,241,28,188,175,120,223,211,127,73,19,217,26,165,170,186,199,182,153,92,175,120,223,211,127,73,
19,217,26,165,170,186,199,182,175,87,18,86,110,123,183,91,110,119,102,165,63,111,102,42,153,140,231,52,181,60,246,161,214,192,59,6,60,134,182,247,6,247,183,228,45,219,127,242,71,186,189,216,86,164,232,
253,243,85,68,126,56,55,31,251,5,89,215,163,48,98,104,204,59,180,233,55,194,151,227,243,234,84,104,129,198,155,218,64,5,90,195,224,212,101,100,178,251,149,80,142,10,175,112,175,121,240,54,255,0,189,15,
117,29,194,189,236,249,27,118,205,223,141,15,117,89,220,209,216,17,52,102,29,218,108,139,238,172,136,193,242,166,33,133,180,130,114,2,74,181,182,109,253,181,137,58,61,2,14,141,196,186,77,145,126,249,120,
169,127,89,152,97,109,37,74,220,146,173,111,45,115,171,63,39,122,113,43,61,193,189,227,192,91,190,242,61,212,119,10,247,183,228,109,251,127,242,135,186,172,247,13,29,129,109,209,248,215,25,178,47,223,
45,25,15,133,181,12,41,144,165,13,137,43,214,242,236,168,185,80,237,118,184,40,77,226,225,113,69,209,214,4,134,216,142,200,113,26,170,25,70,178,181,129,25,227,179,101,29,89,249,14,156,72,206,225,94,246,
124,141,191,103,254,80,247,81,220,27,223,208,91,190,242,61,213,97,137,110,209,167,52,121,235,171,247,155,194,17,29,77,54,242,83,20,18,22,177,176,39,190,218,50,13,105,177,89,225,77,180,202,185,75,149,118,
49,27,89,199,68,143,206,169,8,4,237,112,107,13,92,239,27,232,234,207,200,116,226,86,231,219,174,240,35,46,67,236,193,212,27,245,100,228,159,213,92,240,100,116,168,200,119,24,214,225,92,215,119,249,200,
79,243,107,116,181,147,171,206,108,36,112,36,81,96,249,165,143,175,246,213,41,156,164,240,201,217,20,151,99,155,73,60,10,126,56,209,70,146,120,20,252,113,162,167,119,49,235,226,57,121,94,241,191,166,254,
146,39,178,53,75,85,117,143,109,50,185,94,241,191,166,254,146,39,178,53,75,85,117,143,109,94,174,36,172,220,219,103,121,182,236,87,164,45,105,10,84,193,128,120,247,130,164,165,105,107,82,45,48,160,73,
178,199,148,152,172,115,9,113,82,221,111,35,36,237,74,78,14,250,169,204,181,115,235,113,72,144,227,124,225,214,41,27,137,174,78,224,157,98,58,75,216,242,236,168,202,169,101,178,177,178,56,39,174,215,121,
23,100,64,106,66,185,166,34,69,76,112,219,110,171,85,68,18,114,71,215,89,189,93,94,187,57,17,47,45,77,198,141,21,184,233,109,14,171,85,90,164,247,196,110,206,218,175,139,18,176,156,201,123,111,102,202,
13,136,237,252,101,237,248,225,92,233,76,239,82,37,218,233,165,140,207,132,196,121,54,72,210,20,204,116,176,151,85,49,212,245,70,195,168,14,174,70,250,208,238,146,54,252,36,137,214,136,147,103,243,33,
145,41,199,150,130,132,164,97,32,1,176,226,170,6,196,172,145,210,94,216,1,225,182,178,44,71,35,241,151,182,231,201,178,142,148,195,169,18,124,93,92,78,142,74,181,37,13,148,201,125,167,150,178,163,148,
234,103,96,245,215,124,13,37,69,178,222,150,98,91,35,185,40,18,174,146,185,11,73,214,224,74,7,122,113,228,53,80,238,17,198,122,75,219,241,194,131,98,86,21,248,203,219,59,54,209,210,152,117,34,117,93,29,
83,144,223,83,174,5,186,188,169,68,12,109,59,107,109,131,230,150,59,15,237,174,14,224,146,64,50,94,193,236,169,136,12,8,209,80,210,117,136,79,229,111,170,213,9,69,229,147,178,106,75,177,27,164,158,5,63,
28,104,163,73,60,10,126,56,209,82,187,152,245,241,28,188,175,120,223,211,127,73,19,217,26,165,170,186,199,182,153,92,175,120,223,211,127,73,19,217,26,165,170,186,199,182,175,87,18,86,110,116,89,45,81,
110,80,110,82,231,92,165,197,49,159,13,33,12,132,224,141,80,114,114,60,245,158,229,90,127,77,220,125,72,247,81,99,254,143,223,63,198,15,220,21,104,147,7,71,90,137,163,166,111,57,29,245,67,68,135,194,63,
230,65,112,167,9,255,0,187,56,59,118,96,86,121,73,229,247,47,24,172,46,197,95,185,86,159,211,119,31,82,61,212,119,42,211,250,110,227,234,71,186,175,238,217,236,112,238,23,119,95,93,182,42,87,116,92,6,
26,152,218,212,128,144,18,123,205,93,161,91,120,236,168,216,118,251,93,190,235,124,130,219,17,221,126,36,149,48,218,174,77,45,198,220,0,236,9,212,220,175,41,59,55,87,53,75,201,221,43,193,82,238,85,167,
244,221,199,212,143,117,29,202,180,254,155,184,250,145,238,171,62,135,60,203,151,121,214,251,173,134,210,181,180,220,151,213,132,168,148,20,32,168,32,28,227,0,138,247,161,174,67,187,219,111,55,73,182,
235,36,78,108,48,150,146,227,46,173,164,100,156,236,73,42,201,174,106,151,144,210,188,21,94,229,90,127,77,220,125,72,247,81,220,171,79,233,187,143,169,30,234,186,232,171,48,174,108,222,164,202,135,98,
140,226,100,177,26,49,125,151,67,27,74,134,193,157,96,85,129,190,162,236,237,90,5,250,230,133,69,8,230,73,74,23,53,165,46,48,35,173,177,3,91,127,87,205,70,169,121,13,43,193,82,187,194,183,197,130,183,
99,93,231,184,232,220,20,17,143,217,92,182,151,148,252,6,156,112,229,71,59,126,186,232,211,68,190,137,83,147,34,52,72,167,0,165,168,185,212,3,27,8,201,39,110,250,226,176,124,210,199,97,253,181,106,91,
114,238,74,212,146,57,180,147,192,167,227,141,20,105,39,129,79,199,26,41,46,230,53,124,71,47,43,222,55,244,223,210,68,246,70,169,106,174,177,237,166,87,43,222,55,244,223,210,68,246,70,169,106,174,177,
237,171,213,196,149,155,156,240,238,241,160,219,46,209,94,56,113,217,65,67,179,80,10,227,122,254,151,157,142,227,178,156,89,142,216,105,144,78,196,36,28,128,62,186,233,145,6,59,235,11,113,150,212,174,
37,67,38,180,247,38,46,15,200,183,230,239,106,114,165,182,216,234,212,150,12,74,210,85,202,116,57,38,224,251,170,18,12,174,249,89,249,82,0,42,237,216,43,177,26,113,115,105,169,45,179,124,154,210,36,184,
167,94,8,94,53,212,173,228,236,227,92,157,202,137,172,79,48,222,60,154,180,11,76,76,39,44,183,231,239,107,157,9,121,59,213,70,171,125,245,187,121,90,161,201,91,46,45,10,66,150,147,130,66,134,21,235,21,
211,104,210,201,22,102,22,221,162,231,38,16,88,1,69,149,96,171,27,179,90,205,170,46,60,3,89,207,228,240,160,218,98,109,249,6,241,179,27,40,232,72,58,168,204,253,41,147,113,67,201,184,93,165,73,15,41,11,
115,156,94,117,138,58,185,236,205,118,49,167,119,56,203,121,113,111,147,88,83,161,1,69,11,198,66,70,18,62,161,92,66,211,19,103,200,55,140,28,247,180,11,76,92,12,176,214,115,249,60,40,232,75,200,117,81,
27,54,224,195,172,63,242,170,113,231,73,82,150,163,181,71,202,106,70,193,243,75,29,135,246,214,69,166,30,78,88,111,28,54,87,107,77,161,166,194,27,72,74,70,224,42,149,214,224,242,196,156,212,150,8,141,
36,240,41,248,227,69,26,73,224,83,241,198,138,141,220,202,87,196,114,242,189,227,127,77,253,36,79,100,106,150,170,235,30,218,101,114,189,227,127,77,253,36,79,100,106,150,170,235,30,218,189,92,73,89,185,
138,40,162,170,76,40,162,138,0,229,118,116,118,158,13,45,125,249,174,132,40,45,33,73,57,6,169,55,149,169,187,147,193,39,142,106,238,219,73,106,44,93,79,235,50,133,30,210,42,80,177,201,180,82,112,81,73,
153,162,138,42,164,194,138,40,160,8,93,36,240,41,248,227,69,26,73,224,83,241,198,138,199,119,51,77,124,71,47,43,222,55,244,223,210,68,246,70,169,106,174,177,237,166,87,43,222,55,244,223,210,68,246,70,
169,106,174,177,237,171,213,196,149,155,152,171,126,133,55,29,235,46,144,161,246,91,82,144,195,106,14,40,100,167,229,82,14,60,149,80,175,104,117,198,210,180,182,181,165,43,24,80,73,198,176,243,213,26,
202,17,12,29,60,133,111,110,59,58,233,76,118,209,45,109,52,182,155,25,83,1,32,165,95,247,109,206,218,168,46,61,160,33,69,19,228,21,99,96,44,111,62,186,227,101,209,33,230,211,62,67,220,194,118,100,119,
229,35,204,9,173,151,6,224,33,40,232,18,31,117,68,247,193,198,194,112,61,116,169,99,182,78,228,92,95,62,115,119,182,152,240,27,134,228,104,189,54,67,140,226,59,122,186,141,235,103,103,109,46,47,159,57,
187,219,87,230,194,75,48,3,132,132,115,13,235,16,50,64,197,70,174,76,173,155,35,174,107,54,228,48,76,73,111,58,238,122,171,107,84,99,183,53,193,82,19,154,182,33,130,97,202,144,227,185,29,235,141,4,140,
118,230,163,235,66,34,194,138,40,174,156,33,116,147,192,167,227,141,20,105,39,129,79,199,26,43,29,220,205,53,241,28,188,175,120,223,211,127,73,19,217,26,165,170,186,199,182,153,92,175,120,223,211,127,
73,19,217,26,165,170,186,199,182,175,87,18,86,110,98,138,42,87,69,173,141,222,47,241,32,62,226,219,109,229,16,84,140,100,96,19,179,61,149,86,241,220,66,62,43,136,105,244,173,214,131,200,27,208,73,0,250,
171,124,249,49,228,37,2,60,36,70,32,237,41,89,86,125,117,217,164,109,177,163,183,79,197,76,182,57,161,223,25,104,72,80,59,183,12,140,84,68,219,255,0,78,74,4,153,33,97,39,35,8,199,236,20,153,89,201,212,
158,10,37,243,231,55,123,106,252,218,130,89,128,165,36,41,33,134,201,73,227,179,117,80,174,233,83,211,220,91,104,90,146,78,194,18,106,230,139,148,118,218,134,80,240,11,109,150,199,84,236,80,29,149,26,
154,82,101,108,79,8,149,153,50,43,204,234,51,111,109,133,231,58,233,90,137,253,117,193,91,158,210,23,46,3,163,57,37,46,103,190,213,212,0,236,250,171,77,104,142,196,152,81,69,21,209,72,93,36,240,41,248,
227,69,26,73,224,83,241,198,138,199,119,51,77,124,71,47,43,222,55,244,223,210,68,246,70,169,106,174,177,237,166,87,43,222,55,244,223,210,68,246,70,169,106,174,177,237,171,213,196,149,155,152,174,155,100,
164,194,158,204,133,176,220,132,182,114,90,112,144,149,108,227,141,181,205,69,84,67,183,72,238,138,188,205,138,227,172,182,211,73,117,166,208,210,50,82,148,235,141,155,118,241,175,170,100,53,13,149,115,
77,69,142,148,36,96,14,108,87,200,82,215,205,165,165,141,165,47,54,127,249,138,119,92,185,69,125,19,28,73,132,147,183,25,31,254,107,37,253,154,52,85,176,203,196,110,49,227,253,109,138,2,99,236,252,94,
62,51,191,155,20,171,254,81,222,7,38,10,126,62,186,7,40,207,147,158,132,159,143,174,161,146,165,55,150,119,130,244,246,35,73,66,16,148,52,188,4,164,13,248,170,157,109,211,27,250,239,218,110,151,92,103,
153,40,104,140,125,66,181,86,202,56,153,173,228,20,81,69,88,145,11,164,158,5,63,28,104,163,73,60,10,126,56,209,88,238,230,105,175,136,229,229,123,198,254,155,250,72,158,200,213,45,85,214,61,180,202,229,
123,198,254,155,250,72,158,200,213,45,85,214,61,181,122,184,146,179,115,21,43,162,214,246,174,183,232,176,228,21,6,157,214,201,73,193,216,146,127,133,69,87,125,134,226,109,55,104,243,82,157,114,209,61,
239,148,16,65,253,181,71,183,97,15,55,251,115,81,244,29,155,162,53,213,33,83,139,58,185,216,66,117,72,250,235,76,189,33,184,189,33,78,42,212,82,79,13,67,82,58,83,120,131,42,213,14,221,106,140,252,104,
109,203,67,231,159,112,45,90,197,64,29,160,13,148,210,186,63,110,106,106,208,167,208,72,222,121,179,88,238,206,77,158,158,80,138,122,150,127,242,37,187,185,112,25,197,176,142,196,26,59,185,113,253,26,
126,193,166,240,155,109,198,67,205,237,255,0,233,154,58,101,183,35,229,155,243,252,153,168,247,255,0,127,250,104,234,83,240,253,255,0,232,67,7,221,145,164,193,199,152,230,84,91,59,49,142,21,53,93,90,110,
235,46,233,171,70,58,146,164,243,74,220,156,112,21,203,91,104,226,96,189,167,60,160,162,138,42,196,72,93,36,240,41,248,227,69,26,73,224,83,241,198,138,199,119,51,77,124,71,47,43,222,55,244,223,210,68,
246,70,169,106,174,177,237,166,87,43,222,55,244,223,210,68,246,70,169,106,174,177,237,171,213,196,149,155,152,162,138,147,209,171,95,118,111,113,109,229,210,208,120,145,174,19,173,140,2,119,113,221,85,
111,29,196,33,39,120,38,253,43,127,188,42,243,164,211,177,121,123,7,24,56,170,214,150,89,249,137,6,21,169,114,36,188,53,85,135,88,45,40,40,28,227,7,178,163,39,71,210,121,15,173,247,32,56,56,146,115,178,
178,220,156,158,81,122,222,17,100,233,199,129,52,116,229,13,185,165,188,187,220,248,239,169,181,148,133,13,227,125,76,199,99,72,36,54,149,49,31,156,214,64,94,80,115,176,238,168,168,183,178,43,147,101,
197,222,123,74,27,81,57,249,53,127,10,238,168,200,214,123,188,123,130,102,92,98,173,180,4,148,149,17,179,39,117,73,214,186,86,35,220,205,99,203,10,40,162,170,76,133,210,79,2,159,142,52,81,164,158,5,63,
28,104,172,119,115,52,215,196,114,242,189,227,127,77,253,36,79,100,106,150,170,235,30,218,101,114,189,227,127,77,253,36,79,100,106,150,170,235,30,218,189,92,73,89,185,138,234,181,74,68,59,131,50,29,67,
142,33,179,146,150,220,45,168,236,224,161,180,87,45,21,81,9,187,165,205,203,221,213,133,177,136,154,136,230,219,43,116,228,1,158,178,206,210,124,230,180,79,19,162,33,60,229,192,58,23,145,134,228,21,250,
235,130,34,89,84,132,9,75,90,25,61,101,33,57,35,234,174,139,131,112,16,148,116,9,15,186,162,123,224,227,97,56,30,186,92,97,224,239,176,184,190,124,230,239,109,48,162,186,242,25,130,150,30,83,69,108,54,
9,11,41,27,184,210,246,249,243,155,189,181,126,108,36,179,0,56,72,71,48,222,177,3,36,12,84,106,228,202,217,178,36,110,49,166,53,24,170,68,246,159,70,71,120,153,26,231,213,81,85,33,57,171,98,24,38,28,169,
14,59,145,222,184,208,72,199,110,106,62,175,29,136,176,162,138,43,167,8,93,36,240,41,248,227,69,26,73,224,83,241,198,138,199,119,51,77,124,71,47,43,222,55,244,223,210,68,246,70,169,106,174,177,237,166,
87,43,222,55,244,223,210,68,246,70,169,106,174,177,237,171,213,196,149,155,152,169,75,93,146,85,202,12,217,113,245,57,184,137,10,94,78,211,146,6,7,172,84,93,78,232,221,229,171,84,43,179,78,165,106,84,
166,80,134,245,119,2,22,149,109,250,133,81,231,29,132,65,113,177,201,177,22,95,158,211,79,180,86,91,90,2,143,122,176,50,80,175,33,21,29,58,76,121,9,64,143,9,17,136,59,74,86,85,159,93,89,52,146,252,157,
32,75,113,237,204,59,207,187,41,82,214,20,6,197,148,128,66,124,219,56,212,100,249,119,184,141,142,153,150,208,230,82,53,155,78,223,213,74,155,247,220,97,81,124,249,205,222,218,191,54,66,89,128,165,39,
93,33,134,201,73,227,179,117,80,111,159,57,187,219,76,123,124,201,81,153,134,152,106,194,220,97,161,141,80,114,113,179,125,70,174,76,165,155,35,212,201,145,94,103,81,155,123,108,47,57,215,74,212,79,235,
174,10,154,187,119,103,162,30,232,54,164,177,172,54,148,36,109,225,186,161,107,68,118,36,194,138,40,174,138,66,233,39,129,79,199,26,40,210,79,2,159,142,52,86,59,185,154,107,226,57,121,94,241,191,166,254,
146,39,178,53,75,85,117,143,109,50,185,94,241,191,166,254,146,39,178,53,75,85,117,143,109,94,174,36,172,220,197,20,85,143,69,180,85,219,250,10,145,49,136,217,89,109,1,192,78,186,177,156,108,221,179,203,
84,109,46,236,68,178,65,67,108,59,33,8,83,201,96,31,253,69,103,3,213,93,23,40,200,97,8,40,158,204,172,157,200,42,239,125,98,165,110,218,52,187,42,153,92,213,243,237,23,11,46,165,129,223,33,192,50,83,183,
142,234,139,156,204,109,68,244,38,38,36,142,183,58,1,24,250,171,153,203,236,119,29,133,181,243,231,55,123,106,252,218,66,153,128,146,160,128,88,108,107,29,195,102,250,160,223,62,115,119,182,152,12,160,
173,152,57,66,212,128,195,122,218,131,110,49,81,171,147,43,102,200,236,159,17,182,99,149,166,230,196,147,144,57,180,21,103,183,104,168,218,145,156,136,41,96,152,204,77,67,153,27,93,198,173,71,85,227,177,
22,20,81,69,116,225,11,164,158,5,63,28,104,163,73,60,10,126,56,209,88,238,230,105,175,136,229,229,123,198,254,155,250,72,158,200,213,45,85,214,61,180,202,229,123,198,254,155,250,72,158,200,213,45,85,214,
61,181,122,184,146,179,115,21,106,209,123,253,190,213,108,121,137,17,230,9,43,115,88,189,25,196,163,89,59,48,133,100,109,25,25,170,173,21,70,179,216,68,240,92,47,58,74,237,237,198,90,182,180,227,18,23,
33,82,150,165,44,15,148,82,112,112,120,12,10,140,184,203,190,68,108,116,169,110,106,57,148,224,56,21,159,85,68,193,13,170,82,3,205,56,235,124,80,217,194,143,101,117,93,81,21,40,111,163,67,149,28,228,228,
188,172,131,217,176,82,233,73,224,108,246,22,151,207,156,221,237,166,61,190,100,184,172,195,76,39,214,210,156,97,160,117,78,51,179,101,46,47,159,57,187,219,87,246,181,75,86,253,116,169,73,230,27,202,83,
188,140,110,21,26,185,50,150,108,137,107,191,118,122,25,238,131,234,91,26,195,97,117,42,219,195,96,168,74,149,184,183,13,49,137,143,2,107,14,100,119,238,171,41,253,149,21,87,142,196,165,184,81,69,20,194,
144,186,73,224,83,241,198,138,52,147,192,167,227,141,21,142,238,102,154,248,142,94,87,188,111,233,191,164,137,236,141,82,213,93,99,219,76,174,87,188,111,233,191,164,137,236,141,82,213,93,99,219,87,171,
137,43,55,49,86,141,17,181,219,174,172,58,204,182,31,231,194,129,231,144,231,85,39,3,1,24,202,142,120,14,27,106,175,83,22,141,34,157,105,136,184,240,195,9,10,94,184,113,77,130,180,156,99,189,86,241,84,
121,246,17,125,150,13,36,209,198,173,74,109,86,151,22,212,134,165,46,26,150,183,48,20,164,167,37,64,240,7,59,170,22,109,182,232,251,121,149,45,135,82,140,168,5,74,74,177,217,182,181,79,189,201,189,41,
134,174,143,132,50,130,84,84,134,246,147,142,177,28,79,158,184,103,51,13,164,160,195,148,183,201,61,240,83,122,184,164,73,251,141,148,46,175,159,57,187,219,76,155,108,23,230,49,17,81,150,210,20,220,118,
142,86,224,71,14,25,165,181,243,231,55,123,106,252,216,73,102,0,90,181,80,88,111,39,25,192,197,74,174,76,165,155,34,90,234,205,209,17,9,155,49,15,51,172,59,209,32,47,111,96,53,11,93,211,24,128,219,58,
209,102,45,231,51,213,45,20,236,237,174,26,209,29,137,48,162,138,43,162,144,186,73,224,83,241,198,138,52,147,192,167,227,141,21,142,238,102,154,248,142,94,87,188,111,233,191,164,137,236,141,82,213,93,
99,219,76,174,87,188,111,233,191,164,137,236,141,82,213,93,99,219,87,171,137,43,55,49,69,21,105,208,125,22,78,145,201,82,36,73,232,141,19,168,210,240,15,56,230,51,170,60,248,219,84,111,29,216,137,100,
173,70,91,72,121,42,125,174,117,177,189,26,218,185,250,235,124,215,162,58,148,136,177,11,4,29,164,184,85,159,93,89,47,90,52,237,137,214,93,128,226,164,190,135,213,25,196,41,176,112,226,83,146,0,226,48,
119,212,101,198,61,230,99,99,164,193,90,82,222,85,148,176,19,235,192,165,212,155,201,220,60,10,155,231,206,110,246,213,249,5,33,136,5,105,214,72,97,188,140,227,35,21,65,190,124,230,239,109,49,160,69,149,
33,152,75,134,194,222,83,108,52,78,170,117,177,179,141,70,174,76,173,155,35,50,228,66,113,157,88,208,139,46,103,174,93,42,253,85,197,83,119,101,221,149,12,137,209,3,76,235,14,251,152,8,219,218,5,66,86,
136,236,73,133,20,81,93,20,133,210,79,2,159,142,52,81,164,158,5,63,28,104,172,119,115,52,215,196,114,242,189,227,127,77,253,36,79,100,106,150,170,235,30,218,101,114,189,227,127,77,253,36,79,100,106,150,
170,235,30,218,189,92,73,89,185,138,177,232,166,151,79,209,195,171,25,45,186,198,185,115,155,90,71,91,24,200,56,200,170,229,21,86,179,184,137,227,98,213,55,73,157,189,73,97,18,25,97,148,243,170,125,194,
167,10,66,220,35,5,68,240,225,186,184,175,175,164,54,216,142,227,35,36,235,115,18,22,188,143,62,107,130,200,22,110,108,134,249,205,108,156,115,105,5,91,184,3,178,166,175,178,166,67,101,24,84,164,135,50,
147,207,180,216,4,99,134,42,109,97,164,135,91,9,219,231,206,110,246,213,253,183,92,110,44,62,109,107,70,99,183,157,82,71,10,160,95,62,115,119,182,152,208,167,200,133,22,47,70,88,70,180,118,243,222,131,
195,207,82,171,147,30,205,145,35,115,113,165,90,81,133,70,46,119,185,212,144,165,47,235,73,217,80,85,101,189,25,11,179,33,199,186,89,10,213,36,173,180,4,122,198,218,173,85,225,177,41,110,20,81,69,48,164,
46,146,120,20,252,113,162,141,36,240,41,248,227,69,99,187,153,166,190,35,151,149,239,27,250,111,233,34,123,35,84,181,87,88,246,211,43,149,239,27,250,111,233,34,123,35,84,181,87,88,246,213,234,226,74,205,
204,86,66,84,65,33,36,129,188,129,186,177,87,13,19,186,219,33,217,37,51,60,36,229,205,103,155,199,124,251,120,24,66,120,3,157,187,124,149,70,240,34,89,43,150,148,132,220,25,47,6,194,14,223,149,214,213,
59,60,219,106,102,239,38,36,118,83,171,22,222,249,94,71,120,92,202,124,251,106,70,253,124,181,185,26,219,208,181,144,90,89,82,121,173,139,105,162,48,27,57,217,144,127,109,66,207,185,194,154,208,76,133,
220,28,41,201,70,178,145,128,125,84,143,45,228,117,217,96,84,95,62,115,119,182,153,16,37,181,22,52,94,122,27,50,117,163,183,142,115,61,238,207,49,165,189,243,231,55,123,105,141,8,194,17,162,244,228,190,
175,197,219,213,230,136,28,56,230,165,87,38,60,246,68,181,225,81,215,106,74,154,76,36,40,148,157,86,139,154,195,205,183,101,87,106,102,231,115,102,68,1,29,151,38,144,8,194,93,41,213,0,118,84,53,94,43,
8,148,183,10,40,162,152,82,23,73,60,10,126,56,209,70,146,120,20,252,113,162,177,221,204,211,95,17,203,202,247,141,253,55,244,145,61,145,170,90,171,172,123,105,149,202,247,141,253,55,244,145,61,145,170,
90,171,172,123,106,245,113,37,102,230,40,162,138,169,48,162,138,40,2,63,240,69,235,196,224,227,11,74,80,163,223,235,40,12,14,53,59,119,97,152,178,27,143,29,122,232,101,164,183,173,229,32,96,215,21,20,
145,130,139,202,29,201,181,134,20,81,69,56,129,69,20,80,4,46,146,120,20,252,113,162,141,36,240,41,248,227,69,99,187,153,166,190,35,151,149,239,27,250,111,233,34,123,35,84,181,87,88,246,211,43,149,239,
27,250,111,233,34,123,35,84,181,87,88,246,213,234,226,74,205,204,81,69,21,82,97,83,48,52,110,124,232,93,41,174,142,132,109,33,46,188,148,45,67,202,18,78,79,154,161,170,227,54,44,137,114,244,122,68,86,
92,121,134,99,48,151,28,109,37,73,65,7,36,18,55,98,150,79,3,37,146,162,166,156,75,188,210,144,160,238,117,117,8,219,159,38,42,94,86,141,92,99,64,233,107,12,41,56,4,182,219,169,83,137,30,116,13,163,28,
124,149,213,113,33,221,57,147,41,163,175,25,19,194,148,234,118,164,2,189,132,157,213,51,5,135,163,105,101,234,116,134,150,212,55,26,149,168,250,210,66,21,172,147,171,133,110,57,225,92,114,58,162,80,168,
162,138,113,2,138,40,160,8,93,36,240,41,248,227,69,26,73,224,83,241,198,138,199,119,51,77,124,70,119,47,23,97,110,229,147,76,155,213,82,138,215,16,140,1,249,163,84,171,93,249,69,68,132,16,62,170,40,163,
91,81,88,13,41,183,147,207,119,87,249,39,212,40,238,234,255,0,36,250,133,20,87,58,178,242,119,68,124,7,119,87,249,39,212,43,165,157,43,156,195,69,166,36,72,109,163,189,8,89,0,253,64,209,69,29,73,121,13,
17,52,167,72,94,74,22,132,149,132,47,26,201,27,149,141,217,173,206,105,92,231,88,12,59,34,66,217,24,195,106,89,41,24,221,179,56,162,138,58,146,242,26,34,115,119,117,127,146,125,66,142,238,175,242,79,168,
81,69,29,89,121,13,17,240,29,221,95,228,159,80,163,187,171,252,147,234,20,81,71,86,94,67,68,124,28,55,75,154,164,160,36,167,30,122,40,162,145,182,222,88,201,37,177,255,217,0,0};

const char* MasterStripComponent::clipboard01_jpg = (const char*) resource_MasterStripComponent_clipboard01_jpg;
const int MasterStripComponent::clipboard01_jpgSize = 25715;


//[EndFile] You can add extra defines here...
//[/EndFile]

