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

#include "SettingsWindowComponent.h"
#include "SettingsClass.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SettingsWindowComponent::SettingsWindowComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    brainPortComboBox.reset (new juce::ComboBox ("new combo box"));
    dspPortComboBox.reset (new juce::ComboBox ("new combo box"));

    addAndMakeVisible (brainPortComboBox.get());
    brainPortComboBox->setEditableText (false);
    brainPortComboBox->setJustificationType (juce::Justification::centredLeft);
    brainPortComboBox->setTextWhenNothingSelected (juce::String());
    brainPortComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    brainPortComboBox->addListener (this);

    addAndMakeVisible (dspPortComboBox.get());
    dspPortComboBox->setEditableText (false);
    dspPortComboBox->setJustificationType (juce::Justification::centredLeft);
    dspPortComboBox->setTextWhenNothingSelected (juce::String());
    dspPortComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    dspPortComboBox->addListener (this);

    bootButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (bootButton.get());
    bootButton->addListener (this);

    populateUsbPortCompoBoxes();


    // brainPortComboBox->setBounds (10, 10, 150, 24);
    // dspPortComboBox->setBounds (10, 45, 150, 24);
    // bootButton->setBounds (10, 80, 150, 24);

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SettingsWindowComponent::~SettingsWindowComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    brainPortComboBox = nullptr;
    dspPortComboBox = nullptr;
    bootButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SettingsWindowComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]


    g.fillAll (juce::Colour (0xff323e44));
    //g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SettingsWindowComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]
    brainPortComboBox->setBounds (10, 10, 150, 24);
    dspPortComboBox->setBounds (10, 45, 150, 24);
    bootButton->setBounds (10, 80, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SettingsWindowComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == brainPortComboBox.get())
    {
        Settings::getInstance().brainPort = brainPortComboBox.get()->getSelectedItemIndex();
    }
    else if (comboBoxThatHasChanged == dspPortComboBox.get())
    {
        Settings::getInstance().dspPort = dspPortComboBox.get()->getSelectedItemIndex();
    }





    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void SettingsWindowComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == bootButton.get())
    {
        bootProcess();
        //[UserButtonCode_bootButton] -- add your button handler code here..
        //[/UserButtonCode_bootButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SettingsWindowComponent::populateUsbPortCompoBoxes()
{
    
}

void SettingsWindowComponent::bootProcess()
{
    // HERE GOES DA SCRIPT FOR BOOOOOOOTIN
}


//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SettingsWindowComponent"
                 componentName="" parentClasses="public juce::Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <COMBOBOX name="new combo box" id="c1022a9818edca80" memberName="juce__comboBox"
            virtualName="" explicitFocusOrder="0" pos="176 200 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="new button" id="2ffde614d17c6c92" memberName="juce__textButton"
              virtualName="" explicitFocusOrder="0" pos="360 200 150 24" buttonText="new button"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

