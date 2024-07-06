//
//  Logger.hpp
//  IKan
//
//  Created by Ashish . on 05/07/24.
//

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace IKan
{
  // Forward Declarations
  class LoggerSpecificationBuilder;

  /// This enum stores the type of logger
  enum class LogType : uint8_t
  {
    Core
  };
  
  enum class LogLevel : uint8_t
  {
    Trace, Debug, Info, Warning, Error, Critical
  };
  
  /// This structure stores the specification of logger instance
  struct LoggerSpecification
  {
    LogType type {LogType::Core};
    LogLevel level {LogLevel::Trace};
    std::string name {"IKAN"};
    std::filesystem::path saveDirectory {""};
    spdlog::sink_ptr overrideSink {nullptr};
    bool showOnConsole {false};
    
    /// This funcion cretes the logger builder instance
    static LoggerSpecificationBuilder Create();
  };
  
  /// This class stores the APIs to use the SPD logger. Initializes the SPD logger and provides the API to save the logs
  /// in file and show in terminal.
  class Logger
  {
  public:
    // Fundamental APIs ----------------------------------------------------------------------------------------------
    /// This function creates the SPD logger instance from logger specification data
    /// - Parameter specification: logger specification data
    static void Create(const LoggerSpecification& specification);
    /// This function destroys all the spd logger instances created by logger
    static void Shutdown();
    
    // Getters --------------------------------------------------------------------------------------------------------
    /// This function returns the log level string from LogLevel enum.
    /// - Parameter level: Log level enum
    /// - Returns: Log level in string view
    static std::string_view GetLogLevelString(LogLevel level);
    /// This function returns the log type string from LogType enum.
    /// - Parameter type: Log type enum
    /// - Returns: Log type in string view
    static std::string_view GetLogTypeString(LogType type);

  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function validates the logger specification data
    /// - Parameter specification: logger specification data
    /// - Returns: true of valide spefication data
    static bool Validate(const LoggerSpecification& specification);
    
    // Member Variables ----------------------------------------------------------------------------------------------
    inline static std::unordered_map<LogType, std::unordered_map<LogLevel, std::shared_ptr<spdlog::logger>>> s_loggerDataMap;
  };
} // namespace IKan
