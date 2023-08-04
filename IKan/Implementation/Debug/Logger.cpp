//
//  Logger.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "Logger.hpp"
#include "Debug/Timer.hpp"
#include "Editor/EditorConsoleSink.hpp"

namespace IKan
{
  namespace LoggerUtils
  {
    /// This function Set the pattern in the sink pointers
    void SetSamePatternInSinks(const std::vector<spdlog::sink_ptr>& sinks, const std::string& pattern)
    {
      for (auto sink : sinks)
      {
        sink->set_pattern(pattern);
      }
    }
  } // namespace LoggerUtils
  
  void Log::Inititialize(const std::string& logDirectoryPath)
  {
    // Create the directory if doesnt exists
    if (!Utils::FileSystem::Exists(logDirectoryPath))
    {
      Utils::FileSystem::CreateDirectory(logDirectoryPath);
    }

    // Core Logger sink ----------------------------------------------------------------------------------------------
    // Add the file name in the log directory to save logs in file
    std::string logFilePath = logDirectoryPath + "/IKan.log";
    std::vector<spdlog::sink_ptr> coreSink =
    {
      CreateRef<spdlog::sinks::stdout_color_sink_mt>(),
      CreateRef<spdlog::sinks::basic_file_sink_mt>(logFilePath, true /* Truncste the Log file */)
    };
    LoggerUtils::SetSamePatternInSinks(coreSink, "[%T%e : %-8l : %v%$");

    s_coreLogger = CreateRef<spdlog::logger>("IKAN ", coreSink.begin(), coreSink.end());
    s_coreLogger->set_level(spdlog::level::trace);
    s_coreLogger->flush_on(spdlog::level::trace);
    
    // Profiler Logger sink -------------------------------------------------------------------------------------------
    // Store the Profiler results in separate file too
    std::string profilerLogFilePath = logDirectoryPath + "/Profiler.log";
    std::vector<spdlog::sink_ptr> profilerSink =
    {
      CreateRef<spdlog::sinks::basic_file_sink_mt>(profilerLogFilePath, true /* Truncste the Log file */)
    };
    LoggerUtils::SetSamePatternInSinks(profilerSink, "[%T%e : %v %$");

    s_profilerLogger = CreateRef<spdlog::logger>("PROFILE", profilerSink.begin(), profilerSink.end());
    s_profilerLogger->set_level(spdlog::level::trace);
    s_profilerLogger->flush_on(spdlog::level::trace);
    
    // Editor Logger sink --------------------------------------------------------------------------------------------
    std::vector<spdlog::sink_ptr> edirorSink =
    {
      std::make_shared<EditorConsoleSink>(1)
    };
    LoggerUtils::SetSamePatternInSinks(edirorSink, "[%T | %-8l | %-4n: %v%$");
    
    s_editorConsoleLogger = CreateRef<spdlog::logger>("EDITOR ", edirorSink.begin(), edirorSink.end());
    s_editorConsoleLogger->set_level(spdlog::level::trace);
    s_editorConsoleLogger->flush_on(spdlog::level::trace);
  }
  
  void Log::Shutdown()
  {
    s_coreLogger.reset();
    spdlog::drop_all();
  }
  
  Ref<spdlog::logger> Log::GetLogger(Type type)
  {
    switch (type) {
      case Type::Core:      return s_coreLogger;
      case Type::Profiler:  return s_profilerLogger;
      case Type::Editor:    return s_editorConsoleLogger;
      default:
        assert(false);
    }
  }
  
  std::string Log::GetModuleName(LogModule tag)
  {
    return LogModuleString[static_cast<size_t>(tag)];
  }
  
  std::string Log::GetModuleName(const std::string_view& tag)
  {
    return static_cast<std::string>(tag);
  }
  
  const Log::TagDetails& Log::GetTagDetails(const std::string moduleName)
  {
    return (HasTag(moduleName)) ? s_tags.at(moduleName) : s_tags[static_cast<std::string>(moduleName)];
  }
  
  bool Log::HasTag(const std::string &moduleName)
  {
    return s_tags.find(moduleName) != s_tags.end();
  }
  
  std::map<std::string, Log::TagDetails>& Log::EnabledTags()
  {
    return s_tags;
  }

} // namespace IKan
