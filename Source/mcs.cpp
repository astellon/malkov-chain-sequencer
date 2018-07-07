#include <string>
#include <iostream>

#include "mcs.h"

MalkovChainSequencer::MalkovChainSequencer() : step_(0.25) {}

MalkovChainSequencer::~MalkovChainSequencer() {}

void MalkovChainSequencer::buttonClicked(Button* button) {
  auto name = button->getName();

  if (name == "choose_button") {
    readMidiFile();
  } else if (name == "start_button") {
    seq_.start();
  }
}

void MalkovChainSequencer::process(MidiBuffer* midi) {
  seq_.process(midi);
}

void MalkovChainSequencer::readMidiFile() {

  FileChooser chooser("Choose MIDI File", File::nonexistent, "*.mid");

  if (chooser.browseForFileToOpen()) {
    const File file(chooser.getResult());
    FileInputStream fis(file);
    MidiFile m;
    m.readFrom(fis);
    analyser_.addMidiFile(m);
    analyse();
  }
}

void MalkovChainSequencer::analyse() {
  analyser_.analyse(&tt_, step_);
  seq_.setTT(&tt_);
  showTT();
}

void MalkovChainSequencer::showTT() {
  for (auto row : tt_) {
    std::cout << row.first << ": ";
    for (auto value : row.second) {
      std::cout << "->(" << value.first << ": " << value.second << ") ";
    }
    std::cout << std::endl;
  }
}
