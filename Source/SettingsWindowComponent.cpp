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


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SettingsWindowComponent::SettingsWindowComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    USBcomboBox.reset (new juce::ComboBox ("new combo box"));
    addAndMakeVisible (USBcomboBox.get());
    USBcomboBox->setEditableText (false);
    USBcomboBox->setJustificationType (juce::Justification::centredLeft);
    USBcomboBox->setTextWhenNothingSelected (TRANS("kurt"));
    USBcomboBox->setTextWhenNoChoicesAvailable (juce::String());
    USBcomboBox->addItem (TRANS("item1"), 1);
    USBcomboBox->addItem (TRANS("item2"), 2);
    USBcomboBox->addItem (TRANS("item3"), 3);
    USBcomboBox->addItem (TRANS("item4"), 4);
    USBcomboBox->addListener (this);

    USBcomboBox->setBounds (168, 128, 150, 24);

    juce__textButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (juce__textButton.get());
    juce__textButton->addListener (this);

    juce__textButton->setBounds (336, 128, 150, 24);


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

    USBcomboBox = nullptr;
    juce__textButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SettingsWindowComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SettingsWindowComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SettingsWindowComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == USBcomboBox.get())
    {
        //[UserComboBoxCode_USBcomboBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_USBcomboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void SettingsWindowComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == juce__textButton.get())
    {
        //[UserButtonCode_juce__textButton] -- add your button handler code here..
        //[/UserButtonCode_juce__textButton]
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

<JUCER_COMPONENT documentType="Component" className="SettingsWindowComponent"
                 componentName="" parentClasses="public juce::Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <COMBOBOX name="new combo box" id="c1022a9818edca80" memberName="USBcomboBox"
            virtualName="" explicitFocusOrder="0" pos="168 128 150 24" editable="0"
            layout="33" items="item1&#10;item2&#10;item3&#10;item4" textWhenNonSelected="kurt"
            textWhenNoItems=""/>
  <TEXTBUTTON name="new button" id="2ffde614d17c6c92" memberName="juce__textButton"
              virtualName="" explicitFocusOrder="0" pos="336 128 150 24" buttonText="new button"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

