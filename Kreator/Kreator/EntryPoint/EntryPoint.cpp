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
    LoggerSpecificaion coreLogger = LoggerSpecificaion::Create().Type(LoggerType::Core).Name("KREATOR").SaveAt("").ShowOnConsole();
    Logger::Add(coreLogger);
  }
  return 0;
}
