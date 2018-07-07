#pragma once

#include <string>

#include "../JuceLibraryCode/JuceHeader.h"

#include "analyser.h"
#include "common.h"
#include "sequencer.h"

class MalkovChainSequencer : public juce::Button::Listener {
 public:
  MalkovChainSequencer();
  ~MalkovChainSequencer();

  void buttonClicked(juce::Button* button) override;

  void setSampleRate(double rate, int num_samples) { seq_.setSampleRate(rate, num_samples); }
  void setBPM(double bpm) { seq_.setBPM(bpm); }
  void setStep(double step) { seq_.setStep(step); step_ = step; }

  void start();
  void stop();
  void process(MidiBuffer* midi);

 private:
  MCS::Analyser analyser_;
  MCS::Sequencer seq_;
  MCS::TransitionTable tt_;
  double step_;

  void readMidiFile();
  void analyse();
  void showTT();
  void exportTT();
};