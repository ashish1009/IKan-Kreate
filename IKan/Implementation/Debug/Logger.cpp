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
    spdlog::level::level_enum GetSpdLevel(LoggerLevel level)
    {
      switch (level) 
      {
        case LoggerLevel::Trace: return spdlog::level::trace;
        case LoggerLevel::Debug: return spdlog::level::debug;
        case LoggerLevel::Info: return spdlog::level::info;
        case LoggerLevel::Warning: return spdlog::level::warn;
        case LoggerLevel::Error: return spdlog::level::err;
        case LoggerLevel::Critical: return spdlog::level::critical;
        default:
          break;
      }
    }

  } // namespace LoggerUtils
  
  // Logger Builder ---------------------------------------------------------------------------------------------------
  LoggerSpecBuilder LoggerSpecificaion::Create()
  {
    return LoggerSpecBuilder();
  }
  
  LoggerSpecBuilder::operator LoggerSpecificaion() const
  {
    return loggerSpecification;
  }
  
  LoggerSpecBuilder& LoggerSpecBuilder::Type(LoggerType type)
  {
    loggerSpecification.type = type;
    return *this;
  }
  
  LoggerSpecBuilder& LoggerSpecBuilder::level(LoggerLevel level)
  {
    loggerSpecification.level = level;
    return *this;
  }

  LoggerSpecBuilder& LoggerSpecBuilder::Name(const std::string& loggerName)
  {
    loggerSpecification.loggerName = loggerName;
    return *this;
  }
  
  LoggerSpecBuilder& LoggerSpecBuilder::ShowOnConsole()
  {
    loggerSpecification.showOnConsile = true;
    return *this;
  }
  
  LoggerSpecBuilder& LoggerSpecBuilder::SaveAt(const std::filesystem::path& saveLogFilePath)
  {
    loggerSpecification.saveLogFilePath = saveLogFilePath;
    return *this;
  }

  LoggerSpecBuilder& LoggerSpecBuilder::OverrideSink(spdlog::sink_ptr sink)
  {
    loggerSpecification.overrideSink = sink;
    return *this;
  }

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
    LoggerUtils::SetPatternInSinks(sink, "[%T%e : %-8l] : %v%$");

    // 3. Create the logger
    Ref<spdlog::logger> logger = CreateRef<spdlog::logger>(loggerSpec.loggerName, sink.begin(), sink.end());
    logger->set_level(LoggerUtils::GetSpdLevel(loggerSpec.level));
    logger->flush_on(LoggerUtils::GetSpdLevel(loggerSpec.level));
  }
} // namespace IKan
