#include <string>
#include <iostream>

#include "mcs.h"

MalkovChainSequencer::MalkovChainSequencer() : step_(0.25) {}

MalkovChainSequencer::~MalkovChainSequencer() {}

void MalkovChainSequencer::buttonClicked(Button* button) {
  if (button->getName() == "choose_button") {
    readMidiFile();
  }
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
