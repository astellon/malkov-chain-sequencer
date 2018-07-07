#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "common.h"

namespace MCS {

class Sequencer {
 public:
  enum class StepState {
    started,
    playing,
    stopped,
    stop
  };

  Sequencer();

  void setSampleRate(double rate, int num_samples);
  void setBPM(double bpm);
  void setStep(double step);

  void setTT(TransitionTable* tt);

  void start() { state_ = StepState::started; }
  void stop() { state_ = StepState::stopped; }

  void process(MidiBuffer* midi);
 private:
  TransitionTable tt_;
  double sample_rate_, bpm_, step_;
  double seconds_per_block_;
  double seconds_per_step_;
  double time_;
  int note_;
  StepState state_;

  void goNextStep(MidiBuffer* midi);
  int transition(int num);
  void showTT();
};

}  // namespace MCS
