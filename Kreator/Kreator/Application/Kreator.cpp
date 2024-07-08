//
//  Kreator.cpp
//  Kreator
//
//  Created by Ashish . on 06/07/24.
//

#include "Kreator.hpp"

std::unique_ptr<Application> CreateApplication(int argc, const char * argv[])
{
  // Create application specification ------------------------------
  ApplicationSpecification applicationSpec;

  return Application::CreateApplication<Application>(applicationSpec);
}
