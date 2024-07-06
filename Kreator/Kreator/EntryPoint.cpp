//
//  EntryPoint.cpp
//  Kreator
//
//  Created by Ashish . on 05/07/24.
//

// IKan Engine namespace
using namespace IKan;

/// This function is the Entry point of the Kreator
/// - Parameters:
///   - argc: Number of arguments passed from binary
///   - argv: Arguments ...
int main(int argc, const char * argv[])
{
  // Debug print for executable arguments
  std::cout << "  Executing : " << argv[0] << "\n";
  
  // User arguments
  if (argc > 1)
  {
    for (uint8_t argIdx = 1; argIdx < argc; argIdx++)
    {
      std::cout << "    Arg[" << argIdx << "]   : " << argv[argIdx] << "\n";
    }
  }

  // Initialize the Core Engine
  {
    // Create Core logger
    for (uint8_t level = (uint8_t)LogLevel::Trace; level <= (uint8_t)LogLevel::Critical; level++)
    {
      Logger::Create(LoggerSpecification::Create().
                     Type(LogType::Core).
                     Level((LogLevel)level).
                     Name("KREATOR").
                     SaveAt("../../../Kreator/Log").
                     ShowOnConsole(level == (uint8_t)(LogLevel::Trace)));
    }
    // Profiler logger
    Logger::Create(LoggerSpecification::Create().
                   Type(LogType::Profiler).
                   Level(LogLevel::Trace).
                   Name("PROFILER").
                   SaveAt("../../../Kreator/Log"));
    
#define TEST_LOGGER 1
#if TEST_LOGGER
    {
      IK_LOG_TRACE(LogModule::None, "Test Logger TRACE {0}", 1);
      IK_LOG_DEBUG(LogModule::None, "Test Logger DEBUG {0}", 1);
      IK_LOG_INFO(LogModule::None, "Test Logger INFO {0}", 1);
      IK_LOG_WARN(LogModule::None, "Test Logger WARN {0}", 1);
      IK_LOG_ERROR(LogModule::None, "Test Logger ERROR {0}", 1);
      IK_LOG_CRITICAL(LogModule::None, "Test Logger CRITICAL {0}", 1);
      
      IK_PROFILE_TRACE("Test Profiler Log");
    }
#endif
  }
  
  // Shutdown the Core Engine
  {
    Logger::Shutdown();
  }
  return 0;
}
