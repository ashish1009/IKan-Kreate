//
//  LoggerspecificationBuilder.hpp
//  IKan
//
//  Created by Ashish . on 05/07/24.
//

#pragma once

#include "Debug/Logger.hpp"

namespace IKan
{
  /// This structure builds the logger specification instance
  struct LoggerSpecificationBuilder
  {
  public:
    /// This function updates the log type in logger specification
    /// - Parameter type: Log type
    /// - Returns: Logger specification builder reference
    LoggerSpecificationBuilder& Type(LogType type);
    /// This function updates the log level in logger specification
    /// - Parameter level: Log level
    /// - Returns: Logger specification builder reference
    LoggerSpecificationBuilder& Level(LogLevel level);
    /// This function updates the logger name in logger specification
    /// - Parameter loggerName: Logger name
    /// - Returns: Logger specification builder reference
    LoggerSpecificationBuilder& Name(std::string_view loggerName);
    /// This function updates the log save file path in logger specification
    /// - Parameter saveLogFilePath: Log save file path
    /// - Returns: Logger specification builder reference
    LoggerSpecificationBuilder& SaveAt(const std::filesystem::path& saveLogFilePath);
    /// This function updates the overriden logger sink pointer in logger specification
    /// - Parameter sink: overriden logger sing pointer
    /// - Returns: Logger specification builder reference
    LoggerSpecificationBuilder& OverrideSink(const spdlog::sink_ptr sink);
    /// This function sets the flag to show logs in console in logger specification
    /// - Returns: Logger specification builder reference
    LoggerSpecificationBuilder& ShowOnConsole();
    
    /// This operator overload returns the logger specification instance
    operator LoggerSpecification() const;

  private:
    LoggerSpecification loggerSpecification;
  };
} // namespace IKan
