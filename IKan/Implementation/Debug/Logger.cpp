//
//  Logger.cpp
//  IKan
//
//  Created by Ashish . on 05/07/24.
//

#include "Logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

#include "LoggerSpecificationBuilder.hpp"
#include "Utils/FileSystemUtils.hpp"

namespace IKan
{
  // Logger Utils ----------------------------------------------------------------------------------------------------
  namespace LoggerUtils
  {
    /// This funcion converts the IKan logger level to spd logger level
    /// - Parameter level: IKan Logger level enum
    static spdlog::level::level_enum GetSpdLevel(LogLevel level)
    {
      switch (level)
      {
        case LogLevel::Trace:     return spdlog::level::trace;
        case LogLevel::Debug:     return spdlog::level::debug;
        case LogLevel::Info:      return spdlog::level::info;
        case LogLevel::Warning:   return spdlog::level::warn;
        case LogLevel::Error:     return spdlog::level::err;
        case LogLevel::Critical:  return spdlog::level::critical;
        default:
          break;
      }
    }
    
    /// This function returns the log type string from LogType enum.
    /// - Parameter type: Log type enum
    static std::string_view GetLogTypeString(LogType level)
    {
      switch (level)
      {
        case LogType::Core:     return "Core";
        case LogType::Profiler: return "Profiler";
        default:
          assert(false);
      }
      return "";
    }
  } // namespace LoggerUtils
  
  // Logger Specification APIs ---------------------------------------------------------------------------------------
  LoggerSpecificationBuilder LoggerSpecification::Create()
  {
    return LoggerSpecificationBuilder();
  }
  
  // Logger APIs -----------------------------------------------------------------------------------------------------
  void Logger::Create(const LoggerSpecification &specification)
  {
    // 1. Validate the logger data
    if (!Validate(specification))
    {
      assert(false);
    }
    
    std::cout << "[Logger INFO] : Creating Logger \n";
    std::cout << "[Logger INFO] :   Name      : " << specification.name << " \n";
    std::cout << "[Logger INFO] :   Type      : " << LoggerUtils::GetLogTypeString(specification.type) << " \n";
    std::cout << "[Logger INFO] :   Level     : " << GetLogLevelString(specification.level) << " \n";
    
    // 2. Initialize the sings for logger
    std::vector<spdlog::sink_ptr> sinks;
    
    // Override the sink
    if (specification.overrideSink)
    {
      sinks.emplace_back(specification.overrideSink);
    }
    // Show on console
    if(specification.showOnConsole)
    {
      sinks.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>());
    }
    // Save file path
    if (specification.saveDirectory != "")
    {
      std::filesystem::path filepath = specification.saveDirectory / specification.name;
      filepath += "_";
      filepath += Logger::GetLogLevelString(specification.level);
      filepath += ".log";
      
      sinks.emplace_back(CreateRef<spdlog::sinks::basic_file_sink_mt>(filepath, true /* Truncste the Log file */));
      std::cout << "[Logger INFO] :   FilePath  : " << Utils::FileSystem::IKanAbsolute(filepath) << " \n";
    }
    
    // Set log patterns
    for (spdlog::sink_ptr& sink : sinks)
    {
      sink->set_pattern("[%T:%e : %-8l : %v%$");
    }
    
    // 3. Create the logger
    Ref<spdlog::logger> logger = CreateRef<spdlog::logger>(specification.name.data(), sinks.begin(), sinks.end());
    if (logger)
    {
      logger->set_level(LoggerUtils::GetSpdLevel(specification.level));
      logger->flush_on(LoggerUtils::GetSpdLevel(specification.level));
      
      s_loggerDataMap[specification.type].insert({specification.level, logger});
    }
    else
    {
      std::cout << " ERROR : Logger '" << specification.name << "' is NULL" << std::endl;
      assert(false);
    }
  }
  
  void Logger::Shutdown()
  {
    std::cout << "[Logger WARN] : Destroying Loggers !\n";
    s_loggerDataMap.clear();
  }
  
  bool Logger::Validate(const LoggerSpecification& specification)
  {
    // If in type is already present
    if (s_loggerDataMap.find(specification.type) != s_loggerDataMap.end())
    {
      const std::unordered_map<LogLevel, Ref<spdlog::logger>>& loggerForType = s_loggerDataMap.at(specification.type);
      if (loggerForType.find(specification.level) != loggerForType.end())
      {
        std::cout << "[Logger ERROR] : Invalid Logger speficiation Data. Level : " << Logger::GetLogLevelString(specification.level) << " already stored for curretn logger \n";
        return false;
      }
    }
    return true;
  }

  std::string_view Logger::GetLogLevelString(LogLevel level)
  {
    switch (level)
    {
      case LogLevel::Trace:     return "Trace";
      case LogLevel::Debug:     return "Debug";
      case LogLevel::Info:      return "Info";
      case LogLevel::Warning:   return "Warning";
      case LogLevel::Error:     return "Error";
      case LogLevel::Critical:  return "Critical";
      default:
        assert(false);
    }
    return "";
  }

  std::string_view Logger::GetModuleName(LogModule tag)
  {
    return LogModuleString[static_cast<size_t>(tag)];
  }
  
  Ref<spdlog::logger> Logger::GetLogger(LogType type, LogLevel logLevel)
  {
    if (s_loggerDataMap.find(type) != s_loggerDataMap.end())
    {
      const std::unordered_map<LogLevel, Ref<spdlog::logger>>& loggerForType = s_loggerDataMap.at(type);
      if (loggerForType.find(logLevel) != loggerForType.end())
      {
        return loggerForType.at(logLevel);
      }
    }
    return nullptr;
  }

  const Logger::TagDetails& Logger::GetTagDetails(std::string_view moduleName)
  {
    return (HasTag(moduleName)) ? s_tags.at(moduleName) : s_tags[moduleName];
  }
  
  bool Logger::HasTag(std::string_view moduleName)
  {
    return s_tags.find(moduleName) != s_tags.end();
  }
} // namespace IKan
