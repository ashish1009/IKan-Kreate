//
//  LoggerBuilder.cpp
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#include "LoggerBuilder.hpp"

namespace IKan
{
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
} // namespace IKan
