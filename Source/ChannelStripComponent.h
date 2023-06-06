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

#pragma once

//[Headers]     -- You can add your own extra header files here --

#include <JuceHeader.h>

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]

    An auto-generated component, created by the Projucer.



    Describe your class and how it works here!

                                                                    //[/Comments]
*/
class ChannelStripComponent  : public juce::Component,
                               public juce::Slider::Listener,
                               public juce::Button::Listener,
                               public juce::ComboBox::Listener,
                               public juce::Label::Listener
{
public:
    //==============================================================================
    ChannelStripComponent ();
    ~ChannelStripComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setFaderVolume(double value);

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void labelTextChanged (juce::Label* labelThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> fader;
    std::unique_ptr<juce::TextButton> writeButton;
    std::unique_ptr<juce::Slider> channelPan;
    std::unique_ptr<juce::TextButton> selectBtn;
    std::unique_ptr<juce::TextButton> soloBtn;
    std::unique_ptr<juce::TextButton> muteBtn;
    std::unique_ptr<juce::TextButton> recordBtn;
    std::unique_ptr<juce::ComboBox> channelSelector;
    std::unique_ptr<juce::Slider> aux11_12Pan;
    std::unique_ptr<juce::ComboBox> postEqInsert;
    std::unique_ptr<juce::Slider> aux7Send;
    std::unique_ptr<juce::Slider> aux11_12Send;
    std::unique_ptr<juce::Slider> aux9_10Pan;
    std::unique_ptr<juce::Slider> aux9_10Send;
    std::unique_ptr<juce::Slider> aux8Send;
    std::unique_ptr<juce::Slider> aux6Send;
    std::unique_ptr<juce::Slider> aux4Send;
    std::unique_ptr<juce::Slider> aux5Send;
    std::unique_ptr<juce::Slider> aux3Send;
    std::unique_ptr<juce::Slider> aux2Send;
    std::unique_ptr<juce::Slider> aux1Send;
    std::unique_ptr<juce::Label> postEqInsertLabel;
    std::unique_ptr<juce::Label> chLabel;
    std::unique_ptr<juce::Component> eqThumbnail;
    std::unique_ptr<juce::TextButton> phaseBtn;
    std::unique_ptr<juce::TextButton> eqBtn;
    std::unique_ptr<juce::TextButton> compressorBtn;
    std::unique_ptr<juce::Label> preEqInsertLabel;
    std::unique_ptr<juce::ComboBox> preEqInsert;
    std::unique_ptr<juce::TextButton> gateBtn;
    std::unique_ptr<juce::TextButton> chAssignLRBtn;
    std::unique_ptr<juce::TextButton> bus7AssignBtn;
    std::unique_ptr<juce::TextButton> bus8AssignBtn;
    std::unique_ptr<juce::TextButton> bus5AssignBtn;
    std::unique_ptr<juce::TextButton> bus6AssignBtn;
    std::unique_ptr<juce::TextButton> bus3AssignBtn;
    std::unique_ptr<juce::TextButton> bus4AssignBtn;
    std::unique_ptr<juce::TextButton> bus1AssignBtn;
    std::unique_ptr<juce::TextButton> bus2AssignBtn;
    std::unique_ptr<juce::Slider> chDigitalTrim;
    std::unique_ptr<juce::ComboBox> juce__comboBox4;
    std::unique_ptr<juce::Slider> chLevelToTape;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelStripComponent)
};

//[EndFile] You can add extra defines here...

//[/EndFile]

