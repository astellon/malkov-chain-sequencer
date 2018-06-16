#include "mcs.h"

MalkovChainSequencer::MalkovChainSequencer() {
  // empty now
}

MalkovChainSequencer::~MalkovChainSequencer() {
  // empty now
}

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
    midi_.clear();
    midi_.readFrom(fis);
  }
}

