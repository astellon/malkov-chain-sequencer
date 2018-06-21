#include <string>
#include <iostream>

#include "mcs.h"

MalkovChainSequencer::MalkovChainSequencer() : is_pressed_(false), last_note_(-1) {
  log("Start MalKovChainSequencer");
}

MalkovChainSequencer::~MalkovChainSequencer() {
  log("End MalKovChainSequencer");
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
  log("Start analysis");
  
  double seconds_per_quarter_note;
  midi_.convertTimestampTicksToSeconds();
  auto num_tracks = midi_.getNumTracks();

  log("Num Tracks = " + std::to_string(num_tracks));
  log("Start reading MIDI ...");
  log("============================================");

  for (int i = 0; i != num_tracks; i++) {
    log("Read Track: " + std::to_string(i));
    auto mseq = midi_.getTrack(i);
    MidiMessageSequence achan;
    for (int j = 1; j != 17; j++) {
      log("Read channel: " + std::to_string(j));
      achan.clear();
      mseq->extractMidiChannelMessages(j, achan, true);
      auto num_events = achan.getNumEvents();
      for (int k = 0; k != num_events; k++) {
        auto msg = achan.getEventPointer(k)->message;
        if (msg.isNoteOn()) {

        } else if (msg.isNoteOdff()) {

        } else if (msg.isTempoMetaEvent()) {
          seconds_per_quarter_note = msg.getTempoSecondsPerQuarterNote();
        }
      }
    }
  }
  log("============================================");
  log("End reading MIDI");
  showTT();
}

void MalkovChainSequencer::log(const std::string& str) {
  logger_.log(str);
}

void MalkovChainSequencer::showTT() {
  for (int i = 0; i < 129; i++) {
    for (int j = 0; j < 129; j++) {
      std::cout << transition_table_[i][j] << " ";
    }
    std::cout << std::endl;
  }
}
