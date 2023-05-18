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

#include "SettingsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SettingsComponent::SettingsComponent (MixerManager& mixerManagerInstance)
    : mixerManager(mixerManagerInstance)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    brainPortComboBox.reset (new juce::ComboBox ("brainPortComboBox"));
    addAndMakeVisible (brainPortComboBox.get());
    brainPortComboBox->setEditableText (false);
    brainPortComboBox->setJustificationType (juce::Justification::centredLeft);
    brainPortComboBox->setTextWhenNothingSelected (juce::String());
    brainPortComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    brainPortComboBox->addListener (this);

    brainPortComboBox->setBounds (8, 32, 150, 24);

    brainPortLabel.reset (new juce::Label ("brainPortLabel",
                                           TRANS("Brain Port\n")));
    addAndMakeVisible (brainPortLabel.get());
    brainPortLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    brainPortLabel->setJustificationType (juce::Justification::centredLeft);
    brainPortLabel->setEditable (false, false, false);
    brainPortLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    brainPortLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    brainPortLabel->setBounds (8, 8, 79, 24);

    dspPortComboBox.reset (new juce::ComboBox ("dspPortComboBox"));
    addAndMakeVisible (dspPortComboBox.get());
    dspPortComboBox->setEditableText (false);
    dspPortComboBox->setJustificationType (juce::Justification::centredLeft);
    dspPortComboBox->setTextWhenNothingSelected (juce::String());
    dspPortComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    dspPortComboBox->addListener (this);

    dspPortComboBox->setBounds (176, 32, 150, 24);

    dspPortLabel.reset (new juce::Label ("dspPortLabel",
                                         TRANS("DSP Port\n")));
    addAndMakeVisible (dspPortLabel.get());
    dspPortLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    dspPortLabel->setJustificationType (juce::Justification::centredLeft);
    dspPortLabel->setEditable (false, false, false);
    dspPortLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    dspPortLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    dspPortLabel->setBounds (176, 8, 79, 24);

    deviceListLabel.reset (new juce::Label ("deviceListLabel",
                                            juce::String()));
    addAndMakeVisible (deviceListLabel.get());
    deviceListLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    deviceListLabel->setJustificationType (juce::Justification::topLeft);
    deviceListLabel->setEditable (false, false, false);
    deviceListLabel->setColour (juce::Label::outlineColourId, juce::Colour (0x9ec4c4c4));
    deviceListLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    deviceListLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    deviceListLabel->setBounds (8, 96, 320, 104);

    comPortListLabel.reset (new juce::Label ("comPortListLabel",
                                             TRANS("COM port device list\n")));
    addAndMakeVisible (comPortListLabel.get());
    comPortListLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    comPortListLabel->setJustificationType (juce::Justification::centredLeft);
    comPortListLabel->setEditable (false, false, false);
    comPortListLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    comPortListLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    comPortListLabel->setBounds (8, 72, 176, 24);

    brainBaudRateLabel.reset (new juce::Label ("brainBaudRateLabel",
                                               TRANS("Brain Baudrate\n")));
    addAndMakeVisible (brainBaudRateLabel.get());
    brainBaudRateLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    brainBaudRateLabel->setJustificationType (juce::Justification::centredLeft);
    brainBaudRateLabel->setEditable (false, false, false);
    brainBaudRateLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    brainBaudRateLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    brainBaudRateLabel->setBounds (8, 224, 176, 24);

    brainBaudRateComboBox.reset (new juce::ComboBox ("brainBaudRateComboBox"));
    addAndMakeVisible (brainBaudRateComboBox.get());
    brainBaudRateComboBox->setEditableText (false);
    brainBaudRateComboBox->setJustificationType (juce::Justification::centredLeft);
    brainBaudRateComboBox->setTextWhenNothingSelected (juce::String());
    brainBaudRateComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    brainBaudRateComboBox->addItem (TRANS("115200"), 1);
    brainBaudRateComboBox->addItem (TRANS("230400"), 2);
    brainBaudRateComboBox->addListener (this);

    brainBaudRateComboBox->setBounds (8, 248, 150, 24);

    initMixerBtn.reset (new juce::TextButton ("initMixerBtn"));
    addAndMakeVisible (initMixerBtn.get());
    initMixerBtn->setButtonText (TRANS("Initialize Mixer"));
    initMixerBtn->addListener (this);

    initMixerBtn->setBounds (200, 248, 150, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (400, 300);


    //[Constructor] You can add your own custom stuff here..

    // Add the USB devices to the comboboxes, and populate the deviceListLabel with descriptions
    updatePortInfo();

    // std::string deviceInfoText;
    // for (const auto &device : mixerManager.getSettings().getUSBDevices())
    // {
    //     // brainPortComboBox->addItem(device.first + " (" + device.second + ")", brainPortComboBox->getNumItems() + 1);
    //     // dspPortComboBox->addItem(device.first + " (" + device.second + ")", dspPortComboBox->getNumItems() + 1);
    //     brainPortComboBox->addItem(device.first, brainPortComboBox->getNumItems() + 1);
    //     dspPortComboBox->addItem(device.first, dspPortComboBox->getNumItems() + 1);

    //     //kurt.append(device.first + ": " + device.second + "\n");

    //     deviceInfoText.append(device.first + ":\n" + device.second + "\n");

    // }
    // deviceListLabel->setText(deviceInfoText, juce::dontSendNotification);

    //[/Constructor]
}

SettingsComponent::~SettingsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    brainPortComboBox = nullptr;
    brainPortLabel = nullptr;
    dspPortComboBox = nullptr;
    dspPortLabel = nullptr;
    deviceListLabel = nullptr;
    comPortListLabel = nullptr;
    brainBaudRateLabel = nullptr;
    brainBaudRateComboBox = nullptr;
    initMixerBtn = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SettingsComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SettingsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SettingsComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == brainPortComboBox.get())
    {
        //[UserComboBoxCode_brainPortComboBox] -- add your combo box handling code here..
        // If  this option is chosen in the other combobox, reset.
        if (brainPortComboBox->getSelectedId() == dspPortComboBox->getSelectedId())
            brainPortComboBox->setSelectedId(0);

        // Get the chosen string, and update our settings.
        const std::string portString = brainPortComboBox.get()->getText().toStdString();
        mixerManager.setBrainPort(portString);
        //[/UserComboBoxCode_brainPortComboBox]
    }
    else if (comboBoxThatHasChanged == dspPortComboBox.get())
    {
        //[UserComboBoxCode_dspPortComboBox] -- add your combo box handling code here..
        // If  this option is chosen in the other combobox, reset.
        if (dspPortComboBox->getSelectedId() == brainPortComboBox->getSelectedId())
            dspPortComboBox->setSelectedId(0);

        const std::string portString = dspPortComboBox.get()->getText().toStdString();
        mixerManager.setDspPort(portString);
        //[/UserComboBoxCode_dspPortComboBox]
    }
    else if (comboBoxThatHasChanged == brainBaudRateComboBox.get())
    {
        //[UserComboBoxCode_brainBaudRateComboBox] -- add your combo box handling code here..
        int baudRateID = brainBaudRateComboBox.get()->getSelectedId();
        std::cout << "id for rate selected: " << baudRateID << std::endl;
        if (baudRateID == 1)
            mixerManager.setBrainBoostState(false);
        else if (baudRateID == 2)
            mixerManager.setBrainBoostState(true);
        else
        {
            printf("SOMETHINGS WRONG WITH OUR TURBO COMBO");
            exit(1);
        }
        //[/UserComboBoxCode_brainBaudRateComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void SettingsComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == initMixerBtn.get())
    {
        //[UserButtonCode_initMixerBtn] -- add your button handler code here..

        // TODO:
        // FIRST CHECK IF WE HAVE SET THE USB PORTS!!!!!
        // IF NOT, LET THE USER KNOW OF THEIR INCOMPETENCE.


        initMixerBtn->setEnabled(false);    // Disable the button until init has finished.
        
        // Call the init method in mixermanager - provide it with a pointer to the button.
        mixerManager.initMixer(initMixerBtn.get());
        //[/UserButtonCode_initMixerBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SettingsComponent::updatePortInfo()
{
    // Fetch previously chosen ports, if any.
    std::string chosenBrainPort = mixerManager.getBrainPort();
    std::string chosenDspPort = mixerManager.getDspPort();
    brainPortComboBox->setText(chosenBrainPort);
    dspPortComboBox->setText(chosenDspPort);

    // Query the usb ports, and get a map of devices.
    std::string deviceInfoText = "";
    for (const auto &device : mixerManager.getUsbPortMap())
    {
        // Add the devices to the comboboxes, BUT only if they are not already chosen in the other box.
        if (device.first != chosenDspPort)
            brainPortComboBox->addItem(device.first, brainPortComboBox->getNumItems() + 1);
        if (device.first != chosenBrainPort)
            dspPortComboBox->addItem(device.first, dspPortComboBox->getNumItems() + 1);

        // Add the device to the device list in any case.
        deviceInfoText.append(device.first + ":\n" + device.second + "\n");
    }
    deviceListLabel->setText(deviceInfoText, juce::dontSendNotification);


}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SettingsComponent" componentName=""
                 parentClasses="public juce::Component" constructorParams="MixerManager&amp; mixerManagerInstance"
                 variableInitialisers="mixerManager(mixerManagerInstance)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="1"
                 initialWidth="400" initialHeight="300">
  <BACKGROUND backgroundColour="ff323e44"/>
  <COMBOBOX name="brainPortComboBox" id="44672d4cfe2fd1bd" memberName="brainPortComboBox"
            virtualName="" explicitFocusOrder="0" pos="8 32 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="brainPortLabel" id="4c9641d1878b25a9" memberName="brainPortLabel"
         virtualName="" explicitFocusOrder="0" pos="8 8 79 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Brain Port&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="dspPortComboBox" id="94f6057b2d158aff" memberName="dspPortComboBox"
            virtualName="" explicitFocusOrder="0" pos="176 32 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="dspPortLabel" id="9692f8471029b67d" memberName="dspPortLabel"
         virtualName="" explicitFocusOrder="0" pos="176 8 79 24" edTextCol="ff000000"
         edBkgCol="0" labelText="DSP Port&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="deviceListLabel" id="cafbb817d33d86b3" memberName="deviceListLabel"
         virtualName="" explicitFocusOrder="0" pos="8 96 320 104" outlineCol="9ec4c4c4"
         edTextCol="ff000000" edBkgCol="0" labelText="" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="9"/>
  <LABEL name="comPortListLabel" id="a94e6088f71a5c15" memberName="comPortListLabel"
         virtualName="" explicitFocusOrder="0" pos="8 72 176 24" edTextCol="ff000000"
         edBkgCol="0" labelText="COM port device list&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="brainBaudRateLabel" id="4ca2f2a44624c82d" memberName="brainBaudRateLabel"
         virtualName="" explicitFocusOrder="0" pos="8 224 176 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Brain Baudrate&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="brainBaudRateComboBox" id="e1b7741da95828a" memberName="brainBaudRateComboBox"
            virtualName="" explicitFocusOrder="0" pos="8 248 150 24" editable="0"
            layout="33" items="115200&#10;230400" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="initMixerBtn" id="733323d221d57460" memberName="initMixerBtn"
              virtualName="" explicitFocusOrder="0" pos="200 248 150 24" buttonText="Initialize Mixer"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

