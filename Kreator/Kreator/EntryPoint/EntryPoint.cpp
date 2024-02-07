//
//  main.cpp
//  Kreator
//
//  Created by Ashish . on 06/02/24.
//

#include <iostream>

using namespace IKan;

/// This is the Entry point of the Kreator
/// - Parameters:
///   - argc: Number of arguments passed from binary
///   - argv: Arguments ...
int main(int argc, const char * argv[])
{
  // Initialize the Core Engine
  {
    LoggerSpecificaion coreLogger = LoggerSpecificaion::Create().Type(LogType::Core).Name("KREATOR").SaveAt("../../../Log/Kreator.log").ShowOnConsole();
    Logger::Add(coreLogger);

    LoggerSpecificaion profilerLogger = LoggerSpecificaion::Create().Type(LogType::Profiler).Name("KREATOR").SaveAt("../../../Log/Profiler.log");
    Logger::Add(profilerLogger);
  }
  
  IK_LOG_INFO(LogModule::None, "Testing Logger{0}", 1);
  IK_PROFILE_INFO("xc {0}", 1);
  return 0;
}
