#pragma once

#include <iostream>
#include <string>
#include <vector>


namespace MyLogger{

class LogOutput {
 public:
  virtual ~LogOutput(){};
  virtual void log(const std::string& str) = 0;
};

class ErrOut : public LogOutput {
 public:
  void log(const std::string& str) override {
    std::cerr << str << std::endl;
  }
};

class Logger {

 public:
  Logger() {
    outputs_.push_back(&err);
  }

  void log(const std::string& str) {
    for (auto out : outputs_) {
      out->log(str);
    }
  }

  void addOutput(LogOutput* out) {
    outputs_.push_back(out);
  }

 private:
  ErrOut err;
  std::vector<LogOutput*> outputs_;
};

}  // MyLogger namespace