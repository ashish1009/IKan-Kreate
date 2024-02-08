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
    IK_PROFILE();
    IK_LOG_INFO("Kreator App", "Creating Kreator Application");
  }
  
  KreatorApp::~KreatorApp()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator App", "Destroying Kreator Application");
  }
  
  void KreatorApp::OnInit()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator App", "Initializing the Renderer Application");
    
    // Create and Push the Rendere Layer --------------------------------------------------------
    m_kreatorLayer = CreateRef<KreatorLayer>();
    PushLayer(m_kreatorLayer);
  }
  
  void KreatorApp::OnShutdown()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator App", "Shutting Down the Renderer Application");

    // Destroy and Pop the Rendere Layer --------------------------------------------------------
    PopLayer(m_kreatorLayer);
    m_kreatorLayer.reset();
  }
} // namespace Kreator

Scope<Application> CreateApplication()
{
  IK_PROFILE();
  
  // Core Application Specificaion --------------------------------------------------------
  ApplicationSpecification appSpec;

  // Core Data
  appSpec.title = "Kreator";
  appSpec.coreAssetPath = "../../../IKan/Assets";
  
  // Renderer Data
  appSpec.rendererType = RendererType::OpenGL;
  
  // Window Specification Data
  appSpec.windowSpecificaion.title = "Kreator";
  appSpec.windowSpecificaion.width = 2500;
  appSpec.windowSpecificaion.height = 1000;
  appSpec.windowSpecificaion.hideTitleBar = false;
  appSpec.windowSpecificaion.isFullScreen = false;
  
  // Window Controller Data
  appSpec.resizable = true;
  appSpec.startMaximized = true;

  // Return the Application
  return Application::CreateApplication<Kreator::KreatorApp>(appSpec);
}
