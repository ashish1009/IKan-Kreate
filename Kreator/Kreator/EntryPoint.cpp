//
//  main.cpp
//  Kreator
//
//  Created by Ashish . on 06/02/24.
//

#include <iostream>

using namespace IKan;

extern Scope<Application> CreateApplication();

/// This is the Entry point of the Kreator
/// - Parameters:
///   - argc: Number of arguments passed from binary
///   - argv: Arguments ...
int main(int argc, const char * argv[])
{
  // Initialize the Core Engine
  {
    LoggerSpecificaion coreLogger = LoggerSpecificaion::Create().Level(LogLevel::Trace).Type(LogType::Core).Name("KREATOR").SaveAt("../../../Kreator/Log/Kreator.log").ShowOnConsole();
    Logger::Add(coreLogger);

    LoggerSpecificaion profilerLogger = LoggerSpecificaion::Create().Type(LogType::Profiler).Name("KREATOR").SaveAt("../../../Kreator/Log/Profiler.log");
    Logger::Add(profilerLogger);
  }
  
  // Create Application
  Scope<Application> application = CreateApplication();
  application->Run();
  
  // Shutdown the Engine
  {
  }
  return 0;
}
