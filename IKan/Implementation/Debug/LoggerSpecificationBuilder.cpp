//
//  LoggerspecificationBuilder.cpp
//  IKan
//
//  Created by Ashish . on 05/07/24.
//

#include "LoggerSpecificationBuilder.hpp"

namespace IKan
{
  LoggerSpecificationBuilder::operator LoggerSpecification() const
  {
    return loggerSpecification;
  }
  
  LoggerSpecificationBuilder& LoggerSpecificationBuilder::Type(LogType type)
  {
    loggerSpecification.type = type;
    return *this;
  }
  LoggerSpecificationBuilder& LoggerSpecificationBuilder::Level(LogLevel level)
  {
    loggerSpecification.level = level;
    return *this;
  }
  LoggerSpecificationBuilder& LoggerSpecificationBuilder::Name(std::string_view loggerName)
  {
    loggerSpecification.name = loggerName;
    return *this;
  }
  LoggerSpecificationBuilder& LoggerSpecificationBuilder::ShowOnConsole()
  {
    loggerSpecification.showOnConsole = true;
    return *this;
  }
  LoggerSpecificationBuilder& LoggerSpecificationBuilder::SaveAt(const std::filesystem::path& saveLogDirectoryPath)
  {
    loggerSpecification.saveDirectory = saveLogDirectoryPath;
    return *this;
  }
  LoggerSpecificationBuilder& LoggerSpecificationBuilder::OverrideSink(const spdlog::sink_ptr sink)
  {
    loggerSpecification.overrideSink = sink;
    return *this;
  }
} // namespace IKan
