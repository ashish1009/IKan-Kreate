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

  /// This structure stores the logger specificaion
  struct LoggerSpecificaion
  {
    LoggerType type;
    std::string loggerName = "IKAN";
    std::filesystem::path saveLogFilePath = "";
    spdlog::sink_ptr overrideSink = nullptr;
    bool showOnConsile = false;

    static LoggerSpecBuilder Create();
  };
  
  /// This structure Builds the logger specificaion
  struct LoggerSpecBuilder
  {
    LoggerSpecBuilder& Type(LoggerType type);
    LoggerSpecBuilder& Name(const std::string& loggerName);
    LoggerSpecBuilder& SaveAt(const std::filesystem::path& saveLogFilePath);
    LoggerSpecBuilder& OverrideSink(spdlog::sink_ptr sink);
    LoggerSpecBuilder& ShowOnConsole();
    operator LoggerSpecificaion() const;
    
  private:
    LoggerSpecificaion loggerSpecification;
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
