#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Sequencer {
 public:
  Sequencer();
  ~Sequencer();

  void process(MidiBuffer* midi);
 private:
  int buffer_size_;
};
