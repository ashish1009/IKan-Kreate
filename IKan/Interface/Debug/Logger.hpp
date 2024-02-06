//
//  Logger.hpp
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "Base/Configurations.h"

namespace IKan
{
  // Forward Declarations
  struct LoggerSpecBuilder;

  /// This enum stores the type of Logger to be used for spd
  enum class LoggerType : uint8_t
  {
    Core
  };
  
  /// This enum stores the Log level of engine
  enum class LoggerLevel : uint8_t
  {
    Trace, Debug, Info, Warning, Error, Critical
  };

  /// This structure stores the logger specificaion
  struct LoggerSpecificaion
  {
    LoggerType type;
    LoggerLevel level = LoggerLevel::Trace;
    std::string loggerName = "IKAN";
    std::filesystem::path saveLogFilePath = "";
    spdlog::sink_ptr overrideSink = nullptr;
    bool showOnConsile = false;

    static LoggerSpecBuilder Create();
  };
  
  /// This class Initializes the SPD logger for different modules.
  class Logger
  {
  public:
    /// This function adds the logger instance in map
    /// - Parameter loggerSpec: logger specificaion
    static void Add(const LoggerSpecificaion& loggerSpec);
    
    DELETE_ALL_CONSTRUCTORS(Logger);
    
  private:
    inline static std::unordered_map<LoggerType, Ref<spdlog::logger>> s_loggers;
  };
} // namespace IKan
