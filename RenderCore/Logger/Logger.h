#ifndef LOGGER_H
#define LOGGER_H

#define RCORE_LOG(level, msg) rcore::Logger::log(level, msg, __FUNCSIG__, __LINE__)

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
    static void log(LogLevel level, std::string text, const char* func = nullptr, int line = 0);
  };

}

#endif