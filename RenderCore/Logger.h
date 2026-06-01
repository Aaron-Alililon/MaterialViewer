#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

namespace rcore {

  enum LogLevel {
    INFO,
    WARN,
    ERR,
    DEBUG
  };

  class Logger {
  private:
    static const char* logFile;

  private:
    static std::string levelToString(LogLevel level);

  public:
    static void log(LogLevel level, std::string text);
  };

}

#endif