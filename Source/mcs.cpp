#include <string>

#include "mcs.h"

MalkovChainSequencer::MalkovChainSequencer() {
  log("Start MalKovChainSequencer");
  // empty now
}

MalkovChainSequencer::~MalkovChainSequencer() {
  log("End MalKovChainSequencer");
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
    auto num_events = mseq->getNumEvents();
    for (int j = 0; j != num_events; j++) {
      auto msg = mseq->getEventPointer(j)->message;
      if (msg.isNoteOn()) {
        log("NOTEON: NoteNumber:" + std::to_string(msg.getNoteNumber()) + " at: " + std::to_string(msg.getTimeStamp()));
      } else if (msg.isNoteOff()) {
        log("NOTEOFF: NoteNumber:" + std::to_string(msg.getNoteNumber()) + " at: " + std::to_string(msg.getTimeStamp()));
      } else if (msg.isTempoMetaEvent()) {
        seconds_per_quarter_note = msg.getTempoSecondsPerQuarterNote();
        log("TEMPO: Sec/QuarterNote: " + std::to_string(seconds_per_quarter_note));
      }
    }
  }
  log("============================================");
  log("End reading MIDI");
}

void MalkovChainSequencer::log(const std::string& str) {
  logger_.log(str);
}

