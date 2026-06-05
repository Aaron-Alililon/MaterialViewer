#include "pch.h"
#include "Logger.h"

namespace rcore {

  const char* Logger::logFile = "data/log.txt";

  std::string Logger::levelToString(LogLevel level) {
    switch (level) {
    case LogLevel::INFO:
      return "Info   ";
    case LogLevel::WARN:
      return "Warning";
    case LogLevel::ERR:
      return "Error  ";
    case LogLevel::DEBUG:
      return "Debug  ";
    }

    return "UNKNOWN";
  }

  void Logger::log(LogLevel level, std::string text, const char* func, int line) {
    #ifndef _DEBUG
    if (level == LogLevel::DEBUG) return;
    #endif

    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    char timeCStr[32];
    ctime_s(timeCStr, 32, &time);
    std::string timeString = { timeCStr };

    std::string location = "";
    if (func) location = std::string(func) + " (" + std::to_string(line) + ")";

    std::ofstream log(Logger::logFile, std::ios_base::app);

    log << timeString.substr(0, timeString.length() - 1) << " | " << levelToString(level) << " | " << location << " | " << text << "\n";
    log.close();
  }

}