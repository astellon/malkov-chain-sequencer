#pragma once

#include <map>

namespace MCS {

using TransitionTable = std::map<int, std::map<int, int>>;

enum NOTE {
  START = -1,
  SUSTAIN = 128,
  REST,
  EXIT_SUSTAIN,
  SIZE
};

const int MAX_REST_LENGTH = 1;

const double DEFAULT_SAMPLE_RATE = 44100;
const double DEFAULT_BPM = 120;
const double DEFAULT_STEP = 0.25;  // 1/16

}  // namespace MCS (Malkov Chain Sequencer)
