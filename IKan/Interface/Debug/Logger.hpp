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
    std::string_view name {"IKAN"};
    std::filesystem::path filePath {""};
    spdlog::sink_ptr overrideSink {nullptr};
    bool showOnConsole {false};
  };
  
  /// This class stores the APIs to use the SPD logger. Initializes the SPD logger and provides the API to save the logs
  /// in file and show in terminal.
  class Logger
  {
  public:
    // Fundamental APIs -----------------------------------------------------------
    /// This function creates the SPD logger instance from logger specification data
    /// - Parameter specification: logger specification data
    static void Create(const LoggerSpecification& specification);
    /// This function destroys all the spd logger instances created by logger
    static void Shutdown();
  };
} // namespace IKan
