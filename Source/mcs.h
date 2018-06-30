#pragma once

#include <string>

#include "../JuceLibraryCode/JuceHeader.h"

#include "analyser.h"
#include "common.h"

class MalkovChainSequencer : public juce::Button::Listener {
 public:
  MalkovChainSequencer();
  ~MalkovChainSequencer();

  void buttonClicked(juce::Button* button) override;

  void process(MidiBuffer* midi);

 private:
  MCS::Analyser analyser_;
  MCS::TransitionTable tt_;
  double step_;

  void readMidiFile();
  void analyse();
  void showTT();
};