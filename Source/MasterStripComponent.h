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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "EventBusClass.h"
#include "FaderValueLookupClass.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MasterStripComponent  : public juce::Component,
                              public juce::Slider::Listener,
                              public juce::ComboBox::Listener,
                              public juce::Button::Listener
{
public:
    //==============================================================================
    MasterStripComponent ();
    ~MasterStripComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void setMasterFaderPosition(double value);

	// void faderMoveEventCallback(std::string faderValue);
    // void vpotTurnEventCallback(std::string vpotValue);
    // void buttonEventCallback(std::string buttonValue);
	void faderUpdateEventCallback(const char (& faderValue)[2]);
    void vpotTurnEventCallback(std::string vpotValue);
    void buttonEventCallback(std::string buttonValue);


    //void setMasterFaderMoveCallbackFunction(std::function<void(float)> callbackFunction);
    //TODO: Set it up through event bus instead.

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    EventBus &eventBus;        // Reference to EventBus singleton.
    FaderValueLookup &faderValueLookup;

	std::function<void(float)> masterFaderMoveCallback;



    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> masterFader;
    std::unique_ptr<juce::Slider> masterVpot;
    std::unique_ptr<juce::Label> masterInsertRLabel;
    std::unique_ptr<juce::ComboBox> masterInsertR;
    std::unique_ptr<juce::TextButton> mastersBankBtn;
    std::unique_ptr<juce::TextButton> effectsBankBtn;
    std::unique_ptr<juce::TextButton> tapeBanksBtn;
    std::unique_ptr<juce::TextButton> lineBanksBtn;
    std::unique_ptr<juce::TextButton> panMasterBtn;
    std::unique_ptr<juce::TextButton> soloMasterBtn;
    std::unique_ptr<juce::TextButton> cuePan1Btn;
    std::unique_ptr<juce::TextButton> cuePan2Btn;
    std::unique_ptr<juce::TextButton> cueLvl1Btn;
    std::unique_ptr<juce::TextButton> cueLvl2Btn;
    std::unique_ptr<juce::TextButton> aux1LevelsBtn;
    std::unique_ptr<juce::TextButton> aux2LevelsBtn;
    std::unique_ptr<juce::TextButton> aux3LevelsBtn;
    std::unique_ptr<juce::TextButton> aux4LevelsBtn;
    std::unique_ptr<juce::TextButton> aux5LevelsBtn;
    std::unique_ptr<juce::TextButton> aux6LevelsBtn;
    std::unique_ptr<juce::TextButton> aux7LevelsBtn;
    std::unique_ptr<juce::TextButton> aux8LevelsBtn;
    std::unique_ptr<juce::TextButton> digitalTrimBtn;
    std::unique_ptr<juce::TextButton> trackingLvlBtn;
    std::unique_ptr<juce::TextButton> metersTopBtn;
    std::unique_ptr<juce::TextButton> metersBotmBtn;
    std::unique_ptr<juce::TextButton> masterSelectBtn;
    std::unique_ptr<juce::TextButton> masterWriteBtn;
    std::unique_ptr<juce::Label> masterInsertRLabel2;
    std::unique_ptr<juce::ComboBox> masterInsertL;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterStripComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

