#include <string>
#include <iostream>
#include <fstream>

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
  tt_.clear();
  analyser_.analyse(&tt_, step_);
  seq_.setTT(&tt_);
  // showTT();
  exportTT();
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

void MalkovChainSequencer::exportTT() {
  std::cerr << "write file" << std::endl;
  std::ofstream ofs("malkov.dot");
  if (ofs.fail()) std::cerr << "cannot opne file" << std::endl;
  ofs << "digraph malkov {" << std::endl;
  ofs << "graph [ layout = circo ];" << std::endl;
  for (auto row : tt_) {
    juce::String from;
    if (row.first == -1) {
      from = "START";
    }else if (row.first == MCS::REST) {
      from = "REST";
    } else if (row.first == MCS::SUSTAIN) {
      from = "SUSTAIN";
    } else {
      from = MidiMessage::getMidiNoteName(row.first, true, true, 4);
    }
    for (auto value : row.second) {
      juce::String to;
      if (value.first == MCS::EXIT_SUSTAIN) {
        to = "EXIT SUSTAIN";
      } else if (value.first == MCS::REST) {
        to = "REST";
      } else if (value.first == MCS::SUSTAIN) {
        to = "SUSTAIN";
      } else {
        to = MidiMessage::getMidiNoteName(value.first, true, true, 4);
      }
      ofs << "\"" << from << "\" -> \"" << to << "\" [label = " << value.second << "];" << std::endl;;
    }
  }
  ofs << "}" << std::endl;
}
