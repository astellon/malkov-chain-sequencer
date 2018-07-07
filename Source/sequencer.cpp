#include <random>

#include "sequencer.h"

namespace MCS {

Sequencer::Sequencer() : sample_rate_(DEFAULT_SAMPLE_RATE), bpm_(DEFAULT_BPM),
                         step_(DEFAULT_STEP), time_(0) {
  seconds_per_step_ = 60 / bpm_ * step_;
}

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

void Sequencer::setTT(TransitionTable* tt) {
  tt_.clear();
  for (auto dists : *tt) {
    int start = dists.first;
    int prev = 0;
    for (auto dist : dists.second) {
      int n = dist.second + prev;
      tt_[start][n] = dist.first;
      prev = n;
    }
    tt_[start][-1] = prev;
  }
  // showTT();
}

void Sequencer::process(MidiBuffer* midi) {
  if (time_ + seconds_per_block_ > seconds_per_step_) {
    time_ = seconds_per_step_ - time_;
    goNextStep(midi);
  }
  time_ += seconds_per_block_;
}

void Sequencer::goNextStep(MidiBuffer* midi) {
  int offset = time_ / sample_rate_;
  int next_note = -1;
  switch (state_) {
    case StepState::started:
      held_note_ = note_ = transition(-1);
      midi->addEvent(MidiMessage::noteOn(1, note_, 1.0f), offset);
      state_ = StepState::playing;
      break;
    case StepState::playing:
      next_note = transition(note_);
      if (next_note != MCS::SUSTAIN && note_ != MCS::REST) {
        midi->addEvent(MidiMessage::noteOff(1, held_note_), offset);
      }
      if (next_note != MCS::SUSTAIN && next_note != MCS::REST) {
        midi->addEvent(MidiMessage::noteOn(1, next_note, 1.0f), offset);
        held_note_ = next_note;
      }
      note_ = next_note;
      break;
    case StepState::stopped:
      midi->addEvent(MidiMessage::noteOff(1, note_), offset);
      note_ = -1;
      held_note_ = -1;
      state_ = StepState::stop;
      break;
    case StepState::stop:
      break;
  }
  time_ = 0;
}

int Sequencer::transition(int num) {
  int n = tt_[num][-1];
  if (n == 1) {
    return tt_[num].lower_bound(n)->second;
  }
  std::random_device seed_gen;
  std::default_random_engine engine(seed_gen());
  std::uniform_int_distribution<> dist(1, n);
  n = dist(engine);
  auto to = tt_[num].lower_bound(n);
  return to->second;
}

void Sequencer::showTT() {
  for (auto row : tt_) {
    std::cout << row.first << ": ";
    for (auto value : row.second) {
      std::cout << "->(" << value.first << ": " << value.second << ") ";
    }
    std::cout << std::endl;
  }
}

}  // namespace MCS
