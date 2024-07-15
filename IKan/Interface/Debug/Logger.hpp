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
  /// This enum stores the module names of IKan Logs (Holds only IKan engine log module names)
#define LogModule(f) \
f(None) \
f(Application) f(Layers) f(Window) \
f(Renderer) f(Renderer2D) f(TextRenderer) f(Font) f(Mesh) f(Material) \
f(Texture) f(VertexBuffer) f(IndexBuffer) f(BufferLayout) f(Shader) f(Pipeline) f(FrameBuffer) \
f(UI) \
f(SceneCamera) f(EditorCamera) \

  /// Generates enum with elements from above MACRO. Also creates an array named "LogModuleString[]" that stores all
  /// the element of enum as const char* (string)
  CreateEnum(LogModule);
  
  // Forward Declarations
  class LoggerSpecificationBuilder;

  /// This enum stores the type of logger
  enum class LogType : uint8_t
  {
    Core, Profiler
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
    std::string name {"IKAN"};
    std::filesystem::path saveDirectory {""};
    spdlog::sink_ptr overrideSink {nullptr};
    bool showOnConsole {false};
    
    /// This funcion cretes the logger builder instance
    static LoggerSpecificationBuilder Create();
  };
  
  /// This class stores the APIs to use the SPD logger. Initializes the SPD logger and provides the API to save the logs
  /// in file and show in terminal.
  class Logger
  {
  public:
    /// This structure stores the Log module details. A module log can be controlled independently
    struct TagDetails
    {
      bool isEnabled {true};
      LogLevel levelFilter {LogLevel::Trace};
    };

    // Fundamental APIs ----------------------------------------------------------------------------------------------
    /// This function creates the SPD logger instance from logger specification data
    /// - Parameter specification: logger specification data
    static void Create(const LoggerSpecification& specification);
    /// This function destroys all the spd logger instances created by logger
    static void Shutdown();
    
    // Getters --------------------------------------------------------------------------------------------------------
    /// This function returns the log level string from LogLevel enum.
    /// - Parameter level: Log level enum
    static std::string_view GetLogLevelString(LogLevel level);
    /// This function returns the name of Module in string. Converts from LogModule enum
    /// - Parameter tag: Tag name as enum
    static std::string_view GetModuleName(LogModule tag);
    /// This function returns the logger based on the type and level
    /// - Parameters:
    ///   - type: Logger type
    ///   - level: log level of current type
    static Ref<spdlog::logger> GetLogger(LogType type, LogLevel level);

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
    template<typename... Args> static void PrintMessage(LogType type, LogLevel level, std::string_view moduleName, Args... args)
    {
      PrintMessageImpl(type, level, moduleName, std::forward<Args>(args)...);
    }
    
    /// This function Prints the Log based on the tag details, Type and Log level
    /// - Parameters:
    ///   - type: Type of logger (core, client ....)
    ///   - level: Log level (Trace, Info ....)
    ///   - moduleName: Tag of Module name as string view
    ///   - args: arguments (Log strings and other argumets to be printed via logs)
    template<typename... Args> static void PrintMessageWithoutTag(LogType type, LogLevel logLevel, Args... args)
    {
      for (int8_t level = (int8_t)logLevel; level >= (int8_t)LogLevel::Trace; level--)
      {
        // Get the Tag Details for a specific module
        Ref<spdlog::logger> logger = GetLogger(type, (LogLevel)level);
        if (!logger)
        {
          continue;
        }
        
        switch (logLevel)
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
    }
    
    DELETE_ALL_CONSTRUCTORS(Logger);
    
  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function validates the logger specification data
    /// - Parameter specification: logger specification data
    /// - Returns: true of valide spefication data
    static bool Validate(const LoggerSpecification& specification);
    /// This function returns the const reference of Tag Details of a module.
    /// - Parameter moduleName: Log Module Name as string
    /// - Note: This creates new tag if not present in Map
    static const TagDetails& GetTagDetails(std::string_view moduleName);
    /// This function checks is enabled Tag map have this current tag
    /// - Parameter moduleName: Log Module Name as string
    static bool HasTag(std::string_view moduleName);

    /// This function Prints the Log based on the tag details, Type and Log level
    /// - Parameters:
    ///   - type: Type of logger (core, client ....)
    ///   - level: Log level (Trace, Info ....)
    ///   - moduleName: Tag of Module name
    ///   - args: arguments (Log strings and other argumets to be printed via logs)
    template<typename... Args> static void PrintMessageImpl(LogType type, LogLevel logLevel, std::string_view moduleName, Args... args)
    {
      // Get the Tag Details for a specific module
      const TagDetails& tag = GetTagDetails(moduleName);
      if (!tag.isEnabled)
      {
        return;
      }
      
      for (int8_t level = (int8_t)logLevel; level >= (int8_t)LogLevel::Trace; level--)
      {
        // Get the logger
        Ref<spdlog::logger> logger = GetLogger(type, (LogLevel)level);
        if (tag.levelFilter > (LogLevel)level or !logger)
        {
          continue;
        }
        
        // Print log module
        std::string logModuleFormat {"{0}"}; // spdlogger will print Module Name as 0th argument
        
        // Max Tag String space to be reserved in log
        static constexpr uint32_t MaxTagLength {25};
        
        // Align all the modules at 25 characters. If module name is smaller than 25 then add spaces
        if (MaxTagLength > moduleName.size())
        {
          logModuleFormat += std::string(static_cast<size_t>(MaxTagLength - moduleName.size()), ' ');
        }
        logModuleFormat += "] | {1}"; // Completing the log format
        
        switch (logLevel)
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
    }

    // Member Variables ----------------------------------------------------------------------------------------------
    inline static std::unordered_map<LogType, std::unordered_map<LogLevel, Ref<spdlog::logger>>> s_loggerDataMap;
    inline static std::map<std::string_view /* Module Name */, TagDetails> s_tags;
  };
} // namespace IKan
