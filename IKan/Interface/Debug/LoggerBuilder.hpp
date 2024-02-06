//
//  LoggerBuilder.hpp
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#pragma once

#include "Debug/Logger.hpp"

namespace IKan
{
  /// This structure Builds the logger specificaion
  struct LoggerSpecBuilder
  {
    LoggerSpecBuilder& Type(LoggerType type);
    LoggerSpecBuilder& level(LoggerLevel level);
    LoggerSpecBuilder& Name(const std::string& loggerName);
    LoggerSpecBuilder& SaveAt(const std::filesystem::path& saveLogFilePath);
    LoggerSpecBuilder& OverrideSink(spdlog::sink_ptr sink);
    LoggerSpecBuilder& ShowOnConsole();
    operator LoggerSpecificaion() const;
    
  private:
    LoggerSpecificaion loggerSpecification;
  };
} // namespace IKan
