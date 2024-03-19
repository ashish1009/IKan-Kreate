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
  KreatorApp::KreatorApp(const ApplicationSpecification& appSpec, const KreatorDirectories& kreatorDirectories)
  : Application(appSpec), m_kreatorDirectories(kreatorDirectories)
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
    IK_LOG_INFO("Kreator App", "Initializing the Kreator Application");
  }
  
  void KreatorApp::OnShutdown()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator App", "Shutting Down the Kreator Application");
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
  appSpec.windowSpecificaion.hideTitleBar = true;
#ifdef DEBUG
  appSpec.windowSpecificaion.isFullScreen = false;
#else
  appSpec.windowSpecificaion.isFullScreen = true;
#endif
  
  // Window Controller Data
  appSpec.resizable = true;
  appSpec.startMaximized = true;
  
  // Editor Data --------------------------------------------------------------------------
  // TODO: Extract from arguments
  Kreator::KreatorDirectories kreatorDirectories;
  kreatorDirectories.clientResourcePath = "../../../Kreator/Resources";
  kreatorDirectories.systemUserPath = "/Users/ashish./iKan_storage";
  kreatorDirectories.iKanKreatePath = "/Users/ashish./iKan_storage/Github/Product/IKan-Kreate";
  
  // Return the Application
  return IKan::Application::CreateApplication<Kreator::KreatorApp>(appSpec, kreatorDirectories);
}
