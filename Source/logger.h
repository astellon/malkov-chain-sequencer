#pragma once

#include <iostream>
#include <string>
#include <vector>

class LogOutput {
 public:
  virtual ~LogOutput(){};
  virtual void log(std::string& str) = 0;
};

class ErrOut : public LogOutput {
 public:
  void log(std::string& str) override {
    std::cerr << str << std::endl;
  }
};

class Logger {

 public:
  Logger() {
    outputs_.push_back(&err);
  }

  void log(std::string& str) {
    for (auto out : outputs_) {
      out->log(str);
    }
  }

 private:
  ErrOut err;
  std::vector<LogOutput*> outputs_;
};