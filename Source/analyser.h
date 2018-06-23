#pragma once

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

class Analyser() {
 public:
  using TransitionTable = std::vector<std::vector<int>>;
  const SIZE = 130;
  const SUSTAIN = 128;
  const REST = 129;

  Analyser();
  ~Analyser();
  void addMidiFile(const MidiFile& midi);
  void clearMidiFiles();
  TransitionTable analyse(double step);

 private:
  std::vector<MidiFile> midi_;

  // sub routin
  void analyse(TransitionTable* tt, const MidiMessageSequence& midi, double step);
  void processTT(TransitionTable* tt);
};