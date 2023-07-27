//
//  Base.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

namespace IKan
{
  /// Version number of IKan Engine
  ///- Note: To be updated manually
  static const std::string kVersion = "v3.3.3.0";
} // namespace IKan

void InitializeEngine(int argc, const char * argv[])
{
  // Date and Time
  time_t currentTime = time(0);             // get current dat/time with respect to system
  char* timeAsString = ctime(&currentTime); // convert it into string
  
  // Start the Engine
  std::cout << " ------------------------------------------------------------------------------------------------ \n";
  std::cout << "      Executing      : " << argv[0] << std::endl;
  std::cout << "      Version        : " << IKan::kVersion << std::endl;
  std::cout << "      Date and Time  : " << timeAsString;
  std::cout << " ------------------------------------------------------------------------------------------------ \n";
}

void ShutdownEngine()
{
  
}
