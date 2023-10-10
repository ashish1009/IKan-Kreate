//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#include "RendererLayer.hpp"
#include "FolderExplorer.hpp"
#include "KreatorConsolePanel.hpp"
#include "ProjectSettingPanel.hpp"
#include "AssetPanel.hpp"
#include "ContentBrowserPanel.hpp"
#include "DefaultAssetViewer.hpp"
#include "SceneHierarchyPanel.hpp"
#include "DefaultMeshes.hpp"

extern std::string IKanVersion;

namespace Kreator
{
  // Kretor Resource Path
#define KreatorResourcePath(path) Utils::FileSystem::Absolute(m_clientResourcePath / path)

  RendererLayer* RendererLayer::s_instance = nullptr;
  
  RendererLayer& RendererLayer::Get()
  {
    return *s_instance;
  }

  RendererLayer::RendererLayer(Ref<UserPreferences> userPreference, const std::filesystem::path& clientResourcePath)
  : Layer("Kreator Renderer"), m_clientResourcePath(clientResourcePath)
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "RendererLayer instance already created");
    // Copy the single instance of application
    s_instance = this;
    
    IK_LOG_TRACE("Kreator Layer", "Creating Kreator Renderer Layer instance");
    
    // Save All Project Path
#ifdef DEBUG
    m_allProjectsPath = m_clientResourcePath / "../Projects";
#else
    m_allProjectsPath = m_clientResourcePath / "../Projects";
#endif
    
    // Save the default project path
    auto fullAllProjectPath =  Utils::FileSystem::IKanAbsolute(m_allProjectsPath);
    m_projectFilePathBuffer.MemCpy(fullAllProjectPath.data(), 0, fullAllProjectPath.size());
    
    // Save the template project dir
    m_templateProjectDir = m_clientResourcePath / "TemplateProject";
    
    // Set the Application Icon
    m_applicationIcon = Image::Create(KreatorResourcePath("Textures/Logo/IKan.png"));
    m_welcomeIcon = Image::Create(KreatorResourcePath("Textures/Logo/WelcomeIKan.png"));
    
    // Window Icons
    m_iconClose = Image::Create(KreatorResourcePath("Textures/Icons/Close.png"));
    m_iconMinimize = Image::Create(KreatorResourcePath("Textures/Icons/Minimize.png"));
    m_iconMaximize = Image::Create(KreatorResourcePath("Textures/Icons/Maximize.png"));
    m_iconRestore = Image::Create(KreatorResourcePath("Textures/Icons/Restore.png"));
    
    // Shadow Icon
    m_shadowTexture = Image::Create(KreatorResourcePath("Textures/Icons/ShadowLineTop.png"));
    
    // Other Icons
    m_newProject = Image::Create(KreatorResourcePath("Textures/Icons/NewProject.png"));
    m_folder = Image::Create(KreatorResourcePath("Textures/Icons/Folder.png"));
    m_projectIcon = Image::Create(KreatorResourcePath("Textures/Icons/Project.png"));
    m_cameraIcon = Image::Create(KreatorResourcePath("Textures/Icons/Camera.png"));
    m_lightIcon = Image::Create(KreatorResourcePath("Textures/Icons/PointLight.png"));
    m_settingIcon = Image::Create(KreatorResourcePath("Textures/Icons/Gear.png"));
    
    // Scene Button
    m_playButtonTex = Image::Create(KreatorResourcePath("Textures/Icons/Play.png"));
    m_stopButtonTex = Image::Create(KreatorResourcePath("Textures/Icons/Stop.png"));
    m_simulateButtonTex = Image::Create(KreatorResourcePath("Textures/Icons/Simulate.png"));
    m_pauseButtonTex = Image::Create(KreatorResourcePath("Textures/Icons/Pause.png"));
    
    // Guizmo Button
    m_selectToolTex = Image::Create(KreatorResourcePath("Textures/Icons/Pointer.png"));
    m_moveToolTex = Image::Create(KreatorResourcePath("Textures/Icons/Move.png"));
    m_rotateToolTex = Image::Create(KreatorResourcePath("Textures/Icons/Rotate.png"));
    m_scaleToolTex = Image::Create(KreatorResourcePath("Textures/Icons/Scale.png"));
    m_gizmoModeTex = Image::Create(KreatorResourcePath("Textures/Icons/GuizmoMode.png"));
  }
  
  RendererLayer::~RendererLayer()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void RendererLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("Kreator Layer", "Attaching Kreator Renderer Layer to application");
  }
  
  void RendererLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Detaching Kreator Renderer Layer from application");
  }
  
  void RendererLayer::OnUpdate(TimeStep ts)
  {
    
  }
  
  void RendererLayer::OnEvent(Event& event)
  {
  }
  
  void RendererLayer::OnImGuiRender()
  {

  }

  void RendererLayer::OpenScene(const std::string& filepath)
  {
    
  }
  
  std::filesystem::path RendererLayer::GetClientResorucePath() const
  {
    return m_clientResourcePath;
  }

} // namespace Kreator
