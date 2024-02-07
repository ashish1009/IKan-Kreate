//
//  Logger.cpp
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#include <spdlog/sinks/stdout_color_sinks.h>
#include "Logger.hpp"

namespace IKan
{
  // Logger Utils -----------------------------------------------------------------------------------------------------
  namespace LoggerUtils
  {
    /// This function Set the pattern in the sink pointers
    /// - Parameters:
    ///   - sinks: Sink pointer
    ///   - pattern: log pattern defined as SPD Logger
    void SetPatternInSinks(const std::vector<spdlog::sink_ptr>& sinks, const std::string& pattern)
    {
      for (auto& sink : sinks)
      {
        sink->set_pattern(pattern);
      }
    }
    
    /// This funcion converts the Ikan logger level to spd logger level
    /// - Parameter level: Ikan Logger level
    spdlog::level::level_enum GetSpdLevel(LogLevel level)
    {
      switch (level) 
      {
        case LogLevel::Trace: return spdlog::level::trace;
        case LogLevel::Debug: return spdlog::level::debug;
        case LogLevel::Info: return spdlog::level::info;
        case LogLevel::Warning: return spdlog::level::warn;
        case LogLevel::Error: return spdlog::level::err;
        case LogLevel::Critical: return spdlog::level::critical;
        default:
          break;
      }
    }

  } // namespace LoggerUtils
  
  // Logger APIs ------------------------------------------------------------------------------------------------------
  void Logger::Add(const LoggerSpecificaion &loggerSpec)
  {
    // 1. Reset the logger if already added
    if (s_loggers.find(loggerSpec.type) != s_loggers.end())
    {
      s_loggers.at(loggerSpec.type).reset();
    }
    
    // 2. Initialize the sings for logger
    std::vector<spdlog::sink_ptr> sink;
    if (loggerSpec.overrideSink)
    {
      sink.emplace_back(loggerSpec.overrideSink);
    }
    if(loggerSpec.showOnConsile)
    {
      sink.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>());
    }
    if (loggerSpec.saveLogFilePath != "")
    {
      sink.emplace_back(CreateRef<spdlog::sinks::basic_file_sink_mt>(loggerSpec.saveLogFilePath, true /* Truncste the Log file */));
    }
    LoggerUtils::SetPatternInSinks(sink, "[%T:%e : %-8l : %v%$");

    // 3. Create the logger
    Ref<spdlog::logger> logger = CreateRef<spdlog::logger>(loggerSpec.loggerName, sink.begin(), sink.end());
    logger->set_level(LoggerUtils::GetSpdLevel(loggerSpec.level));
    logger->flush_on(LoggerUtils::GetSpdLevel(loggerSpec.level));
    
    s_loggers[loggerSpec.type] = logger;
  }
  
  void Logger::Shutdown()
  {
    for (auto& [type, logger] : s_loggers)
    {
      logger.reset();
    }
    s_loggers.clear();
  }
    
  Ref<spdlog::logger> Logger::GetLogger(LogType type)
  {
    if (s_loggers.find(type) != s_loggers.end())
    {
      return s_loggers.at(type);
    }
    return nullptr;
  }
} // namespace IKan
