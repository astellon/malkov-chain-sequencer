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

  bool is_pressed_;
  int last_note_;
  int note_number_;
  double press_time_;

  double transition_table_[129][129];  // (128 notes + rest)^2
  void readMidiFile();
  void analyse();
  void log(const std::string& str);
  void showTT();
};