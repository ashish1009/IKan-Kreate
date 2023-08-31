//
//  main.cpp
//  Kreator
//
//  Created by Ashish . on 27/07/23.
//

#include <IKanEntryPoint.h>
#include "RendererLayer.hpp"
#include "FolderExplorer.hpp"
#include "Widget.hpp"
#include "ApplicationSettings.hpp"
#include "SceneHierarchyPanel.hpp"

namespace Kreator
{  
  class KreatorApp : public Application
  {
  public:
    KreatorApp(const Application::Specification& appSpec)
    : Application(appSpec)
    {
      IK_LOG_TRACE("Kreator App", "Creating Kreator Application");
    }
    
    ~KreatorApp()
    {
      IK_LOG_WARN("Kreator App", "Destroying Kreator Application");
    }
    
    void OnInit() override
    {

    }
    
    void OnShutdown() override
    {

    }
    
  private:
    Ref<Layer> m_rendereLayer;
  };
} // namespace Kreator

IKan::Scope<IKan::Application> IKan::CreateApplication(const std::filesystem::path& startupProject)
{
  // Set up all the applicaiton specification
  IKan::Application::Specification applicationSpec;
  applicationSpec.renderingApi = Renderer::Api::OpenGl;
  applicationSpec.engineResourcesPath = "../../../IKan/Assets";
  return IKan::CreateScope<IKan::Application>(applicationSpec);
}
