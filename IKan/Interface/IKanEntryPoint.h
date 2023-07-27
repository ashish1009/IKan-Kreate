//
//  IKanEntryPoint.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

extern void InitializeEngine(int argc, const char * argv[]);
extern void ShutdownEngine();

/// This function is the core entry point.
/// - Parameters:
///   - argc: num arguments
///   - argv: arguments
/// - Note: There should not be any other entry point if this file is included in the client
int main(int argc, const char * argv[])
{
  InitializeEngine(argc, argv);
  ShutdownEngine();
  return 0;
}
