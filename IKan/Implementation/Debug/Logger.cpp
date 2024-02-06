//
//  Logger.cpp
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

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

  // Logger APIs ------------------------------------------------------------------------------------------------------
  void Logger::Add(const LoggerSpecificaion &loggerSpec)
  {
    
  }
} // namespace IKan
