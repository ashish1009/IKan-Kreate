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
  /// This is Pure static singleton class. This class instantiate the spd loger for Core, Client, Editor consol and
  /// profiler. Logs are controlled via Module. A single module log can be turned ON or OFF.
  class Log
  {
  public:
    // Fundamental APIs ----------------------------------------------------------------------------------------------
    /// This function initialise the spd logger. Creates the logger instances. Also flush the logs in files. Setup the
    /// log format to be used for each Instances
    /// - Parameter logDirectoryPath: file path where logs will be flushed
    static void Inititialize(const std::string& logDirectoryPath);
    /// This function shutdown the logger and destroy all the instances
    static void Shutdown();
    
    MAKE_PURE_STATIC(Log);
    
  private:
    inline static Ref<spdlog::logger> s_coreLogger;
  };
} // namespace IKan
