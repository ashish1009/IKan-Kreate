//
//  Kreator.cpp
//  Kreator
//
//  Created by Ashish . on 07/02/24.
//

#include "Kreator.hpp"
#include "Layer/KreatorLayer.hpp"

namespace Kreator
{
  KreatorApp::KreatorApp(const ApplicationSpecification& appSpec)
  : Application(appSpec)
  {
  }
  
  KreatorApp::~KreatorApp()
  {
  }
  
  void KreatorApp::OnInit()
  {
  }
  
  void KreatorApp::OnShutdown()
  {
  }
  
  void KreatorApp::OnUpdate(TimeStep ts)
  {

  }
  
  void KreatorApp::OnImGuiRender()
  {

  }
} // namespace Kreator

Scope<Application> CreateApplication()
{
  return nullptr;
}
