#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "common.h"

namespace MCS {

class Sequencer {
 public:
  Sequencer();

  void setSampleRate(double rate, int num_samples);
  void setBPM(double bpm);
  void setStep(double step);

  void setTT(TransitionTable* tt) { tt_ = tt; }

  void process(MidiBuffer* midi);
 private:
  TransitionTable* tt_;
  double sample_rate_, bpm_, step_;
  double seconds_per_block_;
  double seconds_per_step_;
  double time_;
};

}  // namespace MCS