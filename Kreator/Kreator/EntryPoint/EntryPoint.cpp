//
//  main.cpp
//  Kreator
//
//  Created by Ashish . on 06/02/24.
//

#include <iostream>

/// This is the Entry point of the Kreator
/// - Parameters:
///   - argc: Number of arguments passed from binary
///   - argv: Arguments ...
int main(int argc, const char * argv[])
{
  // Initialize the Core Engine
  {
    IKan::LoggerSpecificaion coreLogger = IKan::LoggerSpecificaion::Create();
    IKan::Logger::Add(coreLogger);
  }
  return 0;
}
