#include "analyser.h"

namespace MCS {

Analyser::Analyser() {}

Analyser::~Analyser() {}

void Analyser::addMidiFile(const MidiFile& midi) {
  MidiFile m(midi);
  m.convertTimestampTicksToSeconds();
  midi_.push_back(m);
}

void Analyser::clearMidiFiles() {
  midi_.clear();
}

void Analyser::analyse(TransitionTable* tt, double step) {
  for (auto midi : midi_) {
    auto num_tracks = midi.getNumTracks();
    for (int i = 0; i != num_tracks; i++) {
      auto mseq = midi.getTrack(i);
      MidiMessageSequence achan;
      for (int j = 1; j != 17; j++) {
        achan.clear();
        mseq->extractMidiChannelMessages(j, achan, true);
        analyse(tt, achan, step);
      }
    }
  }
  // processTT(tt);
}

void Analyser::analyse(TransitionTable* tt, const MidiMessageSequence& midi, double step) {  // if 1/16 step is 0.25
  double seconds_per_quarter_note = 0.4;
  bool is_pressed = false;
  int last_note = -1;
  int last_last_note = -1;
  double last_time = 0;
  int max_rest = 4 * MAX_REST_LENGTH;;

  auto num_events = midi.getNumEvents();
  for (int k = 0; k != num_events; k++) {
    auto msg = midi.getEventPointer(k)->message;
    if (msg.isNoteOn()) {
      if (!is_pressed) {
        int n = msg.getNoteNumber();
        double t = msg.getTimeStamp();
        int length = (t-last_time)/seconds_per_quarter_note/step;
        if (last_note == -1) {
          (*tt)[last_note][n]++;
        } else if (last_note == SUSTAIN) {
          (*tt)[last_note][EXIT_SUSTAIN]++;
          (*tt)[last_last_note][n]++;
        } else if (length < 1) {
          (*tt)[last_note][n]++;
        } else if (length > max_rest) {
          (*tt)[last_note][REST]++;
          (*tt)[REST][REST] += (int)max_rest - 1;
          (*tt)[-1][n]++;
        } else {
          (*tt)[last_note][REST]++;
          (*tt)[REST][REST] += length - 1;
          (*tt)[last_note][n]++;
        }

        is_pressed = true;
        last_note = n;
        last_time = t;
      }
    } else if (msg.isNoteOff()) {
      int n = msg.getNoteNumber();
      if (last_note == n) {
        double t = msg.getTimeStamp();
        int length = (t-last_time)/seconds_per_quarter_note/step;
        if (length > 1) {
          (*tt)[n][SUSTAIN]++;
          (*tt)[SUSTAIN][SUSTAIN] += length - 2;
          last_last_note = n;
          n = SUSTAIN;
        }
        is_pressed = false;
        last_note = n;
        last_time = t;
      }
    } else if (msg.isTempoMetaEvent()) {
      seconds_per_quarter_note = msg.getTempoSecondsPerQuarterNote();
    }
  }
}

void Analyser::processTT(TransitionTable* tt) {
  // no process
}

}  // namespace MCS
