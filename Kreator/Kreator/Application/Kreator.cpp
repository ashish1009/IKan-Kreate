//
//  Kreator.cpp
//  Kreator
//
//  Created by Ashish . on 06/07/24.
//

#include "Kreator.hpp"
#include "Layers/RendererLayer.hpp"

namespace Kreator
{
  /// IKan Engine Version
  static const std::string KreatorVersion = "2.0";
  
  KreatorApp::KreatorApp(const ApplicationSpecification& appSpec)
  : Application(appSpec)
  {
    IK_PROFILE();
    IK_LOG_INFO("KreatorApp", "Creating 'Kreator' Application. Version : {0}", KreatorVersion);
  }
  
  KreatorApp::~KreatorApp()
  {
    IK_PROFILE();
    IK_LOG_WARN("KreatorApp", "Destroying 'Kreator' Application");
  }
  
  void KreatorApp::OnInit()
  {
    IK_PROFILE();

    // Creating Renderer layer and push in application stack
    m_rendererLayer = CreateScope<RendererLayer>();
    PushLayer(m_rendererLayer.get());
  }
  
  void KreatorApp::OnShutdown()
  {
    IK_PROFILE();
    IK_LOG_WARN("KreatorApp", "Shutting Down the 'Kreator' Application");

    // Removing Renderer layer from application stack and destroying its instance
    PopLayer(m_rendererLayer.get());
    m_rendererLayer.reset();
  }
  
  void KreatorApp::OnUpdate([[maybe_unused]] TimeStep ts)
  {
    IK_PERFORMANCE("KreatorApp::OnUpdate");
  }
  
  void KreatorApp::OnImGuiRender()
  {
    IK_PERFORMANCE("KreatorApp::OnImGuiRender");
  }
  
  const std::string& KreatorApp::GetVersion()
  {
    return KreatorVersion;
  }
} // namespace Kreator

Scope<Application> CreateApplication([[maybe_unused]] int argc, [[maybe_unused]] const char * argv[])
{
  // Create application specification ------------------------------
  ApplicationSpecification applicationSpec;
  applicationSpec.name = "Kreator";
  applicationSpec.rendererType = RendererType::OpenGL;

  // Window Specification Data
  applicationSpec.windowSpecification.title = "Kreator";
  applicationSpec.windowSpecification.width = 1600;
  applicationSpec.windowSpecification.height = 900;
  applicationSpec.windowSpecification.hideTitleBar = false;
  applicationSpec.windowSpecification.resizable = true;
  applicationSpec.windowSpecification.startMaximized = false;
#ifdef DEBUG
  applicationSpec.windowSpecification.isFullScreen = false;
#else
  applicationSpec.windowSpecification.isFullScreen = true;
#endif
  
  // Ini file
  applicationSpec.iniFilePath = "../../../Kreator/Kreator.ini";

  return Application::CreateApplication<Kreator::KreatorApp>(applicationSpec);
}
