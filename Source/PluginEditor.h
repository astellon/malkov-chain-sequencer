/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "logwindow.h"

//==============================================================================
/**
*/
class MalkovAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    MalkovAudioProcessorEditor (MalkovAudioProcessor&);
    ~MalkovAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MalkovAudioProcessor& processor;

    TextButton choose_button_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MalkovAudioProcessorEditor)
};
