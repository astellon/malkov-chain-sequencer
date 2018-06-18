#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "logger.h"

class MalkovChainSequencer : public juce::Button::Listener {
 public:
  MalkovChainSequencer();
  ~MalkovChainSequencer();

  void buttonClicked(juce::Button* button) override;

 private:
  MidiFile midi_;
  MyLogger::Logger logger_;
  double transition_table_[130][130];  // (128 notes + rest + sustain)^2
  void readMidiFile();
  void analyse();
};