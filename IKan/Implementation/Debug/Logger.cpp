//
//  Logger.cpp
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#include "Logger.hpp"

namespace IKan
{
  // Logger Builder ---------------------------------------------------------------------------------------------------
  LoggerSpecBuilder LoggerSpecificaion::Create()
  {
    return LoggerSpecBuilder();
  }
  
  LoggerSpecBuilder::operator LoggerSpecificaion() const
  {
    return loggerSpecification;
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
    
  }
} // namespace IKan
