//
//  Logger.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

#include <spdlog/spdlog.h>

namespace IKan
{
  /// This enum stores the moule names of IKan Logs (Holds only IKan engine log module names)
#define LogModule(f) \
f(None) f(IKan) \
  
  /// Generates enum with elements from above MACRO. Also creates an array named "logModuleString[]" that stores all
  /// the element of enum as const char* (string)
  CreateEnum(LogModule);

  /// This is Pure static singleton class. This class instantiate the spd loger for Core, Client, Editor consol and
  /// profiler. Logs are controlled via Module. A single module log can be turned ON or OFF.
  class Log
  {
  public:
    /// This enum stores the type of Logger to be used for spd
    enum class Type : uint8_t
    {
      Core
    };
    /// This enum stores the Log level of engine
    enum class Level : uint8_t
    {
      Trace, Debug, Info, Warning, Error, Critical
    };
    /// This structure stores the Log module details. A module log can be controlled independently
    struct TagDetails
    {
      bool isEnabled = true;
      Level levelFilter = Level::Trace;
    };
    
    // Getters -------------------------------------------------------------------------------------------------------
    /// This function checks is enabled Tag map have this current tag
    static bool HasTag(const std::string& moduleName);
    /// This function returns the logger based on the type
    static Ref<spdlog::logger> GetLogger(Type type);
    /// This function returns the name of Module in string. Converts from LogModule enum
    /// - Parameter tag: Tag name as enum
    static std::string GetModuleName(LogModule tag);
    /// This function returns the name of Module in string. Converts from string_view enum
    /// - Parameter tag: Tag name as string_view
    static std::string GetModuleName(const std::string_view& tag);
    /// This function returns the const reference of Tag Details of a module
    /// - Parameter moduleName: Log Module Name as string
    static const TagDetails& GetTagDetails(const std::string moduleName);
    /// This function returns the enabled tags
    static std::map<std::string, TagDetails>& EnabledTags();

    // Fundamental APIs ----------------------------------------------------------------------------------------------
    /// This function initialise the spd logger. Creates the logger instances. Also flush the logs in files. Setup the
    /// log format to be used for each Instances
    /// - Parameter logDirectoryPath: file path where logs will be flushed
    static void Inititialize(const std::string& logDirectoryPath);
    /// This function shutdown the logger and destroy all the instances
    static void Shutdown();
    
    // Print Log tempalted APIs --------------------------------------------------------------------------------------
    /// This function Prints the Log based on the tag details, Type and Log level
    /// - Parameters:
    ///   - type: Type of logger (core, client ....)
    ///   - level: Log level (Trace, Info ....)
    ///   - moduleName: Tag of Module name as enum
    ///   - args: arguments (Log strings and other argumets to be printed via logs)
    template<typename... Args> static void PrintMessage(Type type, Level level, LogModule moduleName, Args... args)
    {
      PrintMessageImpl(type, level, GetModuleName(moduleName), std::forward<Args>(args)...);
    }
    /// This function Prints the Log based on the tag details, Type and Log level
    /// - Parameters:
    ///   - type: Type of logger (core, client ....)
    ///   - level: Log level (Trace, Info ....)
    ///   - moduleName: Tag of Module name as string view
    ///   - args: arguments (Log strings and other argumets to be printed via logs)
    template<typename... Args> static void PrintMessage(Type type, Level level,
                                                        const std::string_view& moduleName, Args... args)
    {
      PrintMessageImpl(type, level, GetModuleName(moduleName), std::forward<Args>(args)...);
    }

    MAKE_PURE_STATIC(Log);
    
  private:
    // Static Member Functions ---------------------------------------------------------------------------------------
    /// This function Prints the Log based on the tag details, Type and Log level
    /// - Parameters:
    ///   - type: Type of logger (core, client ....)
    ///   - level: Log level (Trace, Info ....)
    ///   - moduleName: Tag of Module name
    ///   - args: arguments (Log strings and other argumets to be printed via logs)
    template<typename... Args> static void PrintMessageImpl(Type type, Level level, const std::string& moduleName, Args... args)
    {
      // Max Tag String space to be reserved in log
      static constexpr uint32_t kMaxTagLength = 20;
      
      // Get the Tag Details for a specific module
      const TagDetails& details = GetTagDetails(static_cast<std::string>(moduleName));
      if (details.isEnabled and details.levelFilter <= level)
      {
        Ref<spdlog::logger> logger = GetLogger(type);
        std::string logModuleFormat = "{0}"; // spdlogger will print Module Name as 0th argument
        // Align all the modules at 20 characters. If module name is smaller than 20 then add spaces
        if (kMaxTagLength > moduleName.size())
        {
          logModuleFormat += std::string(static_cast<size_t>(kMaxTagLength - moduleName.size()), ' ');
        }
        logModuleFormat += "] | {1}"; // Completing the log format pending in Log::Init()
        
        switch (level)
        {
          case Level::Debug :
            logger->debug(logModuleFormat, moduleName, fmt::format(std::forward<Args>(args)...));
            break;
          case Level::Trace :
            logger->trace(logModuleFormat, moduleName, fmt::format(std::forward<Args>(args)...));
            break;
          case Level::Info :
            logger->info(logModuleFormat, moduleName, fmt::format(std::forward<Args>(args)...));
            break;
          case Level::Warning :
            logger->warn(logModuleFormat, moduleName, fmt::format(std::forward<Args>(args)...));
            break;
          case Level::Error :
            logger->error(logModuleFormat, moduleName, fmt::format(std::forward<Args>(args)...));
            break;
          case Level::Critical :
            logger->critical(logModuleFormat, moduleName, fmt::format(std::forward<Args>(args)...));
            break;
            
          default:
            assert(false);
        } // Switch Log level
      } // If Log enabled and passed level filter
    }
    
    // Static Member Variables ---------------------------------------------------------------------------------------
    inline static Ref<spdlog::logger> s_coreLogger;
    inline static std::map<std::string /* Module Name */, TagDetails> s_tags;
  };
} // namespace IKan

#ifdef IK_ENABLE_LOG
#define IK_LOG_TRACE(tag, ...)  \
::IKan::Log::PrintMessage(::IKan::Log::Type::Core, ::IKan::Log::Level::Trace, tag, __VA_ARGS__); \

#define IK_LOG_DEBUG(tag, ...)    \
::IKan::Log::PrintMessage(::IKan::Log::Type::Core, ::IKan::Log::Level::Debug, tag, __VA_ARGS__); \

#define IK_LOG_INFO(tag, ...)     \
::IKan::Log::PrintMessage(::IKan::Log::Type::Core, ::IKan::Log::Level::Info, tag, __VA_ARGS__); \

#define IK_LOG_WARN(tag, ...)     \
::IKan::Log::PrintMessage(::IKan::Log::Type::Core, ::IKan::Log::Level::Warning, tag, __VA_ARGS__); \

#define IK_LOG_ERROR(tag, ...)    \
::IKan::Log::PrintMessage(::IKan::Log::Type::Core, ::IKan::Log::Level::Error, tag, __VA_ARGS__); \

#define IK_LOG_CRITICAL(tag, ...) \
::IKan::Log::PrintMessage(::IKan::Log::Type::Core, ::IKan::Log::Level::Critical, tag, __VA_ARGS__); \

#define IK_LOG_VERIFY_MESSAGE_INTERNAL(...)  ::IKan::Log::PrintMessage(::IKan::Log::Type::Client, ::IKan::Log::Level::Debug, "Verify Fail", __VA_ARGS__)
#define IK_LOG_VERIFY(condition, ...) { if(!(condition)) { IK_LOG_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); } }

#else

#define IK_LOG_TRACE(...)
#define IK_LOG_DEBUG(...)
#define IK_LOG_INFO(...)
#define IK_LOG_WARN(...)
#define IK_LOG_ERROR(...)
#define IK_LOG_CRITICAL(...)

#define IK_LOG_VERIFY(condition, ...)

#endif
