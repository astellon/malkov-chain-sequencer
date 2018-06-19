#pragma once

#include <string>

#include "../JuceLibraryCode/JuceHeader.h"

#include "logger.h"

class MalkovChainSequencer : public juce::Button::Listener {
 public:
  MalkovChainSequencer();
  ~MalkovChainSequencer();

  void buttonClicked(juce::Button* button) override;
  MyLogger::Logger* getLogger() { return &logger_;};

 private:
  MidiFile midi_;
  MyLogger::Logger logger_;
  double transition_table_[130][130];  // (128 notes + rest + sustain)^2
  void readMidiFile();
  void analyse();
  void log(const std::string& str);
};