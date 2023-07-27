//
//  Common.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "Common.hpp"

namespace IKan
{
  /// Version number of IKan Engine
  ///- Note: To be updated manually
  static const std::string kVersion = "v1.0.0.0";
} // namespace IKan

/// Options
/// - -log <Save Log Direcgtory Path Path>
void InitializeEngine(int argc, const char * argv[])
{
#ifdef IK_ENABLE_LOG
  // Log File Path. If External arguments are not given then logs to be saved at binary dir
  std::filesystem::path logDirectoryPath = "Log";
#endif

  if (argc > 1)
  {
    for (int32_t i = 1; i < argc; i++)
    {
#ifdef IK_ENABLE_LOG
      // Save the log at Dir
      if (strcmp(argv[i], "-log") == 0)
      {
        logDirectoryPath = argv[++i];
      }
#endif
    }
  }
    
  // Date and Time
  time_t currentTime = time(0);             // get current dat/time with respect to system
  char* timeAsString = ctime(&currentTime); // convert it into string
  
  // Start the Engine
  std::cout << " ------------------------------------------------------------------------------------------------ \n";
  std::cout << "      Executing                           : " << argv[0] << std::endl;
  std::cout << "      Version                             : " << IKan::kVersion << std::endl;
  std::cout << "      Date and Time                       : " << timeAsString;
#ifdef IK_ENABLE_LOG
  std::cout << "      Saving Logs at (Relative to Binary) : " << logDirectoryPath.c_str() << std::endl;
#endif
  std::cout << " ------------------------------------------------------------------------------------------------ \n";
  
#ifdef IK_ENABLE_LOG
  // Initialise the IKan Logger
  IKan::Log::Inititialize(logDirectoryPath);
#endif
}

void ShutdownEngine()
{
#ifdef IK_ENABLE_LOG
  // Initialise the IKan Logger
  IKan::Log::Shutdown();
#endif
}
