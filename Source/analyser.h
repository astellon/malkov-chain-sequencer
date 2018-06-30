#pragma once

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

#include "common.h"

namespace MCS {

class Analyser {
 public:
  Analyser();
  ~Analyser();
  void addMidiFile(const MidiFile& midi);
  void clearMidiFiles();
  void analyse(TransitionTable* tt, double step);

 private:
  std::vector<MidiFile> midi_;

  // sub routin
  void analyse(TransitionTable* tt, const MidiMessageSequence& midi, double step);
  void processTT(TransitionTable* tt);
};

}  // namespace MCS