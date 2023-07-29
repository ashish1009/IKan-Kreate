//
//  Common.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "Common.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Timer.hpp"

/// Version number of IKan Engine
///- Note: To be updated manually
std::string IKanVersion = "v3.1.0.0";

void InitializeEngine(const std::string logDirectoryPath)
{    
#ifdef IK_ENABLE_LOG
  // Initialise the IKan Logger
  IKan::Log::Inititialize(logDirectoryPath);
#endif
  
  // Initialise the Memory Allocator
  IKan::Allocator::Initialize();
  
  IK_LOG_INFO(IKan::LogModule::IKan, "Initialized the Engine");
}

void ShutdownEngine()
{
  IK_LOG_WARN(IKan::LogModule::IKan, "Shutting down the Engine");
  
  // Destroy the Memory Allocator
  IKan::Allocator::Shutdown();
  
#ifdef IK_ENABLE_LOG
  // Destroy the IKan Logger
  IKan::Log::Shutdown();
#endif
  
  // Destroy the IKan Performance instance
  IKan::PerformanceProfiler::Destroy();
}
