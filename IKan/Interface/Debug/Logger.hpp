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
  /// This enum stores the moule names of IKan Logs (Holds only IKan engine log module names)
#define LogModule(f) \
f(None) \
f(Profiler)
  
  /// Generates enum with elements from above MACRO. Also creates an array named "logModuleString[]" that stores all
  /// the element of enum as const char* (string)
  CreateEnum(LogModule);

  // Forward Declarations
  struct LoggerSpecBuilder;

  /// This enum stores the type of Logger to be used for spd
  enum class LogType : uint8_t
  {
    Core, Profiler, Editor
  };
  
  /// This enum stores the Log level of engine
  enum class LogLevel : uint8_t
  {
    Trace, Debug, Info, Warning, Error, Critical
  };

  /// This structure stores the logger specificaion
  struct LoggerSpecificaion
  {
    LogType type;
    LogLevel level = LogLevel::Trace;
    std::string loggerName = "IKAN";
    std::filesystem::path saveLogFilePath = "";
    spdlog::sink_ptr overrideSink = nullptr;
    bool showOnConsile = false;

    static LoggerSpecBuilder Create();
  };
  
  /// This class Initializes the SPD logger for different modules.
  class Logger
  {
  public:
    /// This structure stores the Log module details. A module log can be controlled independently
    struct TagDetails
    {
      bool isEnabled = true;
      LogLevel levelFilter = LogLevel::Trace;
    };

    /// This function adds the logger instance in map
    /// - Parameter loggerSpec: logger specificaion
    static void Add(const LoggerSpecificaion& loggerSpec);
    
    // Getters --------------------------------------------------------------------------------------------------------
    /// This function returns the name of Module in string. Converts from LogModule enum
    /// - Parameter tag: Tag name as enum
    /// - Note: Not returning the cost reference as the string is not stored, we need to extract the name from enum
    static std::string GetModuleName(LogModule tag);
    /// This function returns the name of Module in string. Converts from string_view enum
    /// - Parameter tag: Tag name as string_view
    /// - Note: Not returning the cost reference as the string is not stored, we need to extract the name from enum
    static std::string GetModuleName(const std::string_view& tag);
    /// This function returns the logger based on the type
    /// - Parameter type: Logger type
    static Ref<spdlog::logger> GetLogger(LogType type);

    // Print Log tempalted APIs ---------------------------------------------------------------------------------------
    /// This function Prints the Log based on the tag details, Type and Log level
    /// - Parameters:
    ///   - type: Type of logger (core, client ....)
    ///   - level: Log level (Trace, Info ....)
    ///   - moduleName: Tag of Module name as enum
    ///   - args: arguments (Log strings and other argumets to be printed via logs)
    template<typename... Args> static void PrintMessage(LogType type, LogLevel level, LogModule moduleName, Args... args)
    {
      PrintMessageImpl(type, level, GetModuleName(moduleName), std::forward<Args>(args)...);
    }
    /// This function Prints the Log based on the tag details, Type and Log level
    /// - Parameters:
    ///   - type: Type of logger (core, client ....)
    ///   - level: Log level (Trace, Info ....)
    ///   - moduleName: Tag of Module name as string view
    ///   - args: arguments (Log strings and other argumets to be printed via logs)
    template<typename... Args> static void PrintMessage(LogType type, LogLevel level, const std::string_view& moduleName, Args... args)
    {
      PrintMessageImpl(type, level, GetModuleName(moduleName), std::forward<Args>(args)...);
    }
    
    /// This function Prints the Log based on the tag details, Type and Log level
    /// - Parameters:
    ///   - type: Type of logger (core, client ....)
    ///   - level: Log level (Trace, Info ....)
    ///   - moduleName: Tag of Module name as string view
    ///   - args: arguments (Log strings and other argumets to be printed via logs)
    template<typename... Args> static void PrintMessageWithoutTag(LogType type, LogLevel level, Args... args)
    {
      // Get the Tag Details for a specific module
      Ref<spdlog::logger> logger = GetLogger(type);
      RETURN_IF(!logger);
      
      switch (level)
      {
        case LogLevel::Debug :
          logger->debug(fmt::format(std::forward<Args>(args)...));
          break;
        case LogLevel::Trace :
          logger->trace(fmt::format(std::forward<Args>(args)...));
          break;
        case LogLevel::Info :
          logger->info(fmt::format(std::forward<Args>(args)...));
          break;
        case LogLevel::Warning :
          logger->warn(fmt::format(std::forward<Args>(args)...));
          break;
        case LogLevel::Error :
          logger->error(fmt::format(std::forward<Args>(args)...));
          break;
        case LogLevel::Critical :
          logger->critical(fmt::format(std::forward<Args>(args)...));
          break;
          
        default:
          assert(false);
      } // Switch Log level
    }
    
    DELETE_ALL_CONSTRUCTORS(Logger);
    
  private:
    // Member Functions -----------------------------------------------------------------------------------------------
    /// This function returns the const reference of Tag Details of a module
    /// - Parameter moduleName: Log Module Name as string
    static const TagDetails& GetTagDetails(const std::string& moduleName);
    /// This function checks is enabled Tag map have this current tag
    static bool HasTag(const std::string& moduleName);

    /// This function Prints the Log based on the tag details, Type and Log level
    /// - Parameters:
    ///   - type: Type of logger (core, client ....)
    ///   - level: Log level (Trace, Info ....)
    ///   - moduleName: Tag of Module name
    ///   - args: arguments (Log strings and other argumets to be printed via logs)
    template<typename... Args> static void PrintMessageImpl(LogType type, LogLevel level, const std::string& moduleName, Args... args)
    {
      // Get the Tag Details for a specific module
      const TagDetails& tag = GetTagDetails(static_cast<std::string>(moduleName));
      Ref<spdlog::logger> logger = GetLogger(type);
      RETURN_IF (!tag.isEnabled or tag.levelFilter > level or !logger);

      // Print log module
      std::string logModuleFormat = "{0}"; // spdlogger will print Module Name as 0th argument

      // Max Tag String space to be reserved in log
      static constexpr uint32_t MaxTagLength = 25;
      
      // Align all the modules at 25 characters. If module name is smaller than 25 then add spaces
      if (MaxTagLength > moduleName.size())
      {
        logModuleFormat += std::string(static_cast<size_t>(MaxTagLength - moduleName.size()), ' ');
      }
      logModuleFormat += "] | {1}"; // Completing the log format
      
      switch (level)
      {
        case LogLevel::Debug :
          logger->debug(logModuleFormat, moduleName, fmt::format(std::forward<Args>(args)...));
          break;
        case LogLevel::Trace :
          logger->trace(logModuleFormat, moduleName, fmt::format(std::forward<Args>(args)...));
          break;
        case LogLevel::Info :
          logger->info(logModuleFormat, moduleName, fmt::format(std::forward<Args>(args)...));
          break;
        case LogLevel::Warning :
          logger->warn(logModuleFormat, moduleName, fmt::format(std::forward<Args>(args)...));
          break;
        case LogLevel::Error :
          logger->error(logModuleFormat, moduleName, fmt::format(std::forward<Args>(args)...));
          break;
        case LogLevel::Critical :
          logger->critical(logModuleFormat, moduleName, fmt::format(std::forward<Args>(args)...));
          break;
        default:
          assert(false);
      } // Switch Log level
    }
    
    // Member Functions -----------------------------------------------------------------------------------------------
    inline static std::unordered_map<LogType, Ref<spdlog::logger>> s_loggers;
    inline static std::map<std::string /* Module Name */, TagDetails> s_tags;
  };
} // namespace IKan

#include "Debug/LoggerAPI.h"
