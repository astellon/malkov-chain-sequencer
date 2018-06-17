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
    analyse();
  }
}

void MalkovChainSequencer::analyse() {
  double secons_per_quarter_note;
  midi_.convertTimestampTicksToSeconds();
  auto num_tracks = midi_.getNumTracks();

  for (int i = 0; i != num_tracks; i++) {
    auto mseq = midi_.getTrack(i);
    auto num_events = mseq->getNumEvents();
    for (int j = 0; j != num_events; j++) {
      auto msg = mseq->getEventPointer(j)->message;
      if (msg.isNoteOn()) {
        // note on event
      } else if (msg.isNoteOff()) {
        // note off event
      } else if (msg.isTempoMetaEvent()) {
        secons_per_quarter_note = msg.getTempoSecondsPerQuarterNote();
      }
    }
  }
}

