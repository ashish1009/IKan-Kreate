//
//  IKanEntryPoint.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

extern void InitializeEngine(int argc, const char * argv[]);
extern void ShutdownEngine();
extern IKan::Scope<IKan::Application> CreateApplication(int argc, const char** argv);

/// This function is the core entry point.
/// - Parameters:
///   - argc: num arguments
///   - argv: arguments
/// - Note: There should not be any other entry point if this file is included in the client
int main(int argc, const char * argv[])
{
  InitializeEngine(argc, argv);
  
  {
    IKan::Scope<IKan::Application> app = IKan::CreateApplication(argc, argv);
    app->Run();
    app.reset();
  }
  
  ShutdownEngine();
  return 0;
}
