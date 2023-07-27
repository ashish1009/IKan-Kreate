//
//  Logger.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "Logger.hpp"

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

    // Add the file name in the log directory to save logs in file
    std::string logFilePath = logDirectoryPath + "/IKan.log";

    // Core Logger sink ----------------------------------------------------------------------------------------------
    std::vector<spdlog::sink_ptr> coreSink =
    {
      CreateRef<spdlog::sinks::stdout_color_sink_mt>(),
      CreateRef<spdlog::sinks::basic_file_sink_mt>(logFilePath, true /* Truncste the Log file */)
    };
    LoggerUtils::SetSamePatternInSinks(coreSink, "[%T | %-8l | %-4n: %v%$");

    s_coreLogger = CreateRef<spdlog::logger>("IKAN ", coreSink.begin(), coreSink.end());
    s_coreLogger->set_level(spdlog::level::trace);
    s_coreLogger->flush_on(spdlog::level::trace);
  }
  
  void Log::Shutdown()
  {
    s_coreLogger.reset();
    spdlog::drop_all();
  }
} // namespace IKan
