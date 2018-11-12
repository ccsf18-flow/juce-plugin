/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Vst_midiAudioProcessorEditor::Vst_midiAudioProcessorEditor (Vst_midiAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    //Just general parameters or something
    midiVolume.setSliderStyle (Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled (true, false, this);
    midiVolume.setTextValueSuffix (" Volume");
    midiVolume.setValue(1.0);
    midiVolume.addListener (this);
    // build in that adds actual object
    addAndMakeVisible (&midiVolume);
}

void Vst_midiAudioProcessorEditor::resized()
{
    // sets the position and size of the slider with arguments (x, y, width, height)
    midiVolume.setBounds (40, 30, 20, getHeight() - 60);
}

Vst_midiAudioProcessorEditor::~Vst_midiAudioProcessorEditor()
{
}

//==============================================================================
void Vst_midiAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("V1.0", getLocalBounds(), Justification::centred, 1);
}

void Vst_midiAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    processor.noteOnVel = midiVolume.getValue();
}
