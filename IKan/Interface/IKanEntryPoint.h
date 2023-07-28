//
//  IKanEntryPoint.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

extern void InitializeEngine(const std::string logDirectoryPath);
extern void ShutdownEngine();
extern IKan::Scope<IKan::Application> CreateApplication(int argc, const char** argv);
extern std::string IKanVersion;

/// This function is the core entry point.
/// - Parameters:
///   - argc: num arguments
///   - argv: arguments
/// - Note: There should not be any other entry point if this file is included in the client
int main(int argc, const char * argv[])
{
#ifdef IK_ENABLE_LOG
  // Log File Path. If External arguments are not given then logs to be saved at binary dir
  std::filesystem::path logDirectoryPath = "Log";
#endif
  std::filesystem::path engineInstallPath = "";

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
      else if (strcmp(argv[i], "-core") == 0)
      {
        engineInstallPath = argv[++i];
      }
#endif
    }
  }
  
  if (engineInstallPath == "")
  {
    IK_ASSERT(false, "Pass Engine Install Path in argument");
  }
  
  // Date and Time
  time_t currentTime = time(0);             // get current dat/time with respect to system
  char* timeAsString = ctime(&currentTime); // convert it into string
  
  // Start the Engine
  std::cout << " ------------------------------------------------------------------------------------------------ \n";
  std::cout << "      Executing                           : " << argv[0] << std::endl;
  std::cout << "      Version                             : " << IKanVersion << std::endl;
  std::cout << "      Date and Time                       : " << timeAsString;
#ifdef IK_ENABLE_LOG
  std::cout << "      Saving Logs at (Relative to Binary) : " << logDirectoryPath.c_str() << std::endl;
  std::cout << "      Engine Install Path                 : " << engineInstallPath.c_str() << std::endl;
#endif
  std::cout << " ------------------------------------------------------------------------------------------------ \n";

  InitializeEngine(logDirectoryPath.string());
  
  {
    IKan::Scope<IKan::Application> app = IKan::CreateApplication({engineInstallPath});
    app->Run();
    app.reset();
  }
  
  ShutdownEngine();
  return 0;
}
