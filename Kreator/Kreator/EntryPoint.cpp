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
                     ShowOnConsole());
    }
  }
  
  // Shutdown the Core Engine
  {
    Logger::Shutdown();
  }
  return 0;
}
