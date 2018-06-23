#include "analyser.h"

Analyser::Analyser() : {}

Analyser::~Analyser() {}

void Analyser::addMidiFile(const MidiFile& midi) {
  MidiFile m(midi);
  m.convertTimestampTicksToSeconds();
  midi_.push_back(m);
}

void Analyser::clearMidiFiles() {
  midi.clear();
}

TransitionTable Analyser::analyse(double step) {
  TransitionTable tt;

  for (auto midi : midi_) {
    auto num_tracks = midi.getNumTracks();
    for (int i = 0; i != num_tracks; i++) {
      auto mseq = midi.getTrack(i);
      MidiMessageSequence achan;
      for (int j = 1; j != 17; j++) {
        achan.clear();
        mseq->extractMidiChannelMessages(j, achan, true);
        analyse(&tt, achan, step)
      }
    }
  }

  return tt;
}

void Analyser::analyse(TransitionTable* tt, const MidiMessageSequence& midi, double step) {  // if 1/16 step is 0.25
  double seconds_per_quarter_note;

  auto num_events = midi.getNumEvents();
  for (int k = 0; k != num_events; k++) {
    auto msg = midi.getEventPointer(k)->message;
    if (msg.isNoteOn()) {
      if (!is_pressed_) {
        int n = msg.getNoteNumber();
        double t = msg.getTimeStamp();
        int length = (t-last_time_)/seconds_per_quarter_note/step;
  
        if (length < 1) {
          (*tt)[last_note_][n]++;
        } else {
          (*tt)[last_note_][REST]++;
          (*tt)[REST][REST] += length - 1;
          (*tt)[REST][n]++;
        }
  
        is_pressed_ = true;
        last_note_ = n;
        last_time_ = t;
      }
    } else if (msg.isNoteOff()) {
      int n = msg.getNoteNumber();
      if (last_note_ == n) {
        double t = msg.getTimeStamp();
        int length = (t-last_time_)/seconds_per_quarter_note/step;
        if (length > 1) {
          (*tt)[n][SUSTAIN]++;
          (*tt)[SUSTAIN][SUSTAIN] += length - 2;
          n = SUSTAIN;
        }
        is_pressed_ = false;
        last_note_ = n;
        last_time_ = t;
      }
    } else if (msg.isTempoMetaEvent()) {
      seconds_per_quarter_note = msg.getTempoSecondsPerQuarterNote();
    }
  }
}

void Analyser::processTT(TransitionTable* tt) {
    for (int i = 0; i < 130; i++) {
      for (int j = 0; j < 130-1; j++) {
        (*tt)[i][j+1] = (*tt)[i][j]+(*tt)[i][j+1];
      }
  }
}