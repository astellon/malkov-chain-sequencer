/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MalkovAudioProcessorEditor::MalkovAudioProcessorEditor (MalkovAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);

    choose_button_.setName("choose_button");
    choose_button_.setButtonText("Choose MIDI File to analyse");
    choose_button_.addListener(processor.getSequencer());
    addAndMakeVisible(choose_button_);

    start_button_.setName("start_button");
    start_button_.setButtonText("START");
    start_button_.addListener(processor.getSequencer());
    addAndMakeVisible(start_button_);
}

MalkovAudioProcessorEditor::~MalkovAudioProcessorEditor()
{
}

//==============================================================================
void MalkovAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void MalkovAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    choose_button_.setBounds(20, 20, 100, 30);
    start_button_.setBounds(150, 20, 100, 30);
}
