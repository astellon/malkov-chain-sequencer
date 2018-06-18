#pragma once

#include <string>

#include "../JuceLibraryCode/JuceHeader.h"

#include "logger.h"

class LogWindow : public MyLogger::LogOutput, public juce::TextEditor {

 public:
  LogWindow() {
    setReadOnly(true);  //not Etitable
    setMultiLine(true);  // allow multiline
    setCaretVisible(false);
  }

  void log(std::string& str) override {
    insertTextAtCaret(str);
  }
};