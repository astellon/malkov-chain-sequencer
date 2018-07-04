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
  if (time_ + seconds_per_block_ > seconds_per_step_) {
    time_ = seconds_per_step_ - time_;
    goNextStep(midi);
  }
  time_ += seconds_per_block_;
}

void Sequencer::goNextStep(MidiBuffer* midi) {
  int ofset = time_ / sample_rate_;
  switch (state_) {
    case StepState::started:
      note_ = transition(-1);
      midi.addEvent(MidiMessage.noteOn(1, note_, offset), offset);
      state_ = StepState::playing;
      break;
    case StepState::playing:
      midi.addEvent(MidiMessage.noteOff(1, note_), offset);
      note_ = transition(note_);
      midi.addEvent(MidiMessage.noteOn(1, note_, offset), offset);
      break;
    case StepState::stoped:
      midi.addEvent(MidiMessage.noteOff(1, note_), offset);
      note_ = -1;
      state_ = StepState::stop;
      break;
    case StepState::stop:
      break;
  }
  time_ = 0;
}

int Sequencer::transition(int num) {
  return 66;
}

}  // namespace MCS
