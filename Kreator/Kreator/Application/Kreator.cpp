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
  KreatorApp::KreatorApp(const ApplicationSpecification& appSpec, const std::filesystem::path& clientResourcePath)
  : Application(appSpec), m_clientResourcePath(clientResourcePath)
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator App", "Creating Kreator Application");
    
    // Check Client Path is valid
    bool exist = std::filesystem::exists(m_clientResourcePath);
    bool tempalateProj = std::filesystem::exists(m_clientResourcePath / "TemplateProject");
    bool fonts = std::filesystem::exists(m_clientResourcePath / "Fonts");
    bool textures = std::filesystem::exists(m_clientResourcePath / "Textures");
    
    IK_ASSERT(exist and tempalateProj and fonts and textures, "Invalid Client Directory");
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
    IK_LOG_INFO("Kreator App", "  Kreator Resources Path          : {0}", IKan::Utils::FileSystem::IKanAbsolute(m_clientResourcePath));

#if 0
    // Create and Push the Rendere Layer --------------------------------------------------------
    m_kreatorLayer = CreateRef<KreatorLayer>(m_clientResourcePath);
    PushLayer(m_kreatorLayer);
#endif
  }
  
  void KreatorApp::OnShutdown()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator App", "Shutting Down the Renderer Application");

#if 0
    // Destroy and Pop the Rendere Layer --------------------------------------------------------
    PopLayer(m_kreatorLayer);
    m_kreatorLayer.reset();
#endif
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
  std::filesystem::path clientResourcePath = "../../../Kreator/Resources";
  
  // Return the Application
  return IKan::Application::CreateApplication<Kreator::KreatorApp>(appSpec, clientResourcePath);
}
