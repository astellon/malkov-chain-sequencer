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

  void process(MidiBuffer* midi);

 private:
  MidiFile midi_;
  MyLogger::Logger logger_;

  bool is_pressed_;
  int last_note_;
  double last_time_;

  double transition_table_[130][130];  // (128 notes + + suspend rest)^2
  void readMidiFile();
  void analyse();
  void processTT();
  void log(const std::string& str);
  void showTT();
};