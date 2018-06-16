#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MalkovChainSequencer : public juce::Button::Listener {
 public:
  MalkovChainSequencer();
  ~MalkovChainSequencer();

  void buttonClicked(juce::Button* button) override;

 private:
  MidiFile midi_;
  void readMidiFile();
  void analyse();
};