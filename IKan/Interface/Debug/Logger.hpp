//
//  Logger.hpp
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#pragma once

#include <spdlog/spdlog.h>

#include "Base/Configurations.h"

namespace IKan
{
  // Forward Declarations
  struct LoggerSpecBuilder;

  /// This enum stores the type of Logger to be used for spd
  enum class LoggerType : uint8_t
  {
    Core
  };

  /// This structure stores the logger specificaion
  struct LoggerSpecificaion
  {
    static LoggerSpecBuilder Create();
  };
  
  /// This structure Builds the logger specificaion
  struct LoggerSpecBuilder
  {
    LoggerSpecificaion loggerSpecification;
    operator LoggerSpecificaion() const;
  };
  
  /// This class Initializes the SPD logger for different modules.
  class Logger
  {
  public:
    /// This function adds the logger instance in map
    /// - Parameter loggerSpec: logger specificaion
    static void Add(const LoggerSpecificaion& loggerSpec);
    
    DELETE_ALL_CONSTRUCTORS(Logger);
    
  private:
    inline static std::unordered_map<LoggerType, Ref<spdlog::logger>> s_loggers;
  };
} // namespace IKan
