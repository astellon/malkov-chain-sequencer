#include "sequencer.h"

namespace MCS {

Sequencer::Sequencer() : sample_rate_(DEFAULT_SAMPLE_RATE), bpm_(DEFAULT_BPM),
                         step_(DEFAULT_STEP), time_(0) {}

void Sequencer::setSampleRate(double sample_rate, int num_samples) {
  sample_rate_ = sample_rate;
  seconds_per_block_ = num_samples / sample_rate;
}

void Sequencer::setBPM(double bpm) {
  bpm_ = bpm;
  seconds_per_step_ = 60 / bpm_ * step_;
}

void Sequencer::setStep(double step) {
  step_ = step;
  seconds_per_step_  = 60 / bpm_ * step;
}

void Sequencer::process(MidiBuffer* midi) {
  
}

}  // namespace MCS