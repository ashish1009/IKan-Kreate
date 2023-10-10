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

  // Panel IDs
#define CONSOLE_PANEL_ID "EditorConsolePanel"
#define PROJECT_SETTING_PANEL_ID "ProjectSetting"
#define ASSET_MANAGER_PANEL_ID "Assets"
#define CONTENT_BROWSER_PANEL_ID "ContentBrowserPanel"
#define SCENE_HIERARCHY_PANEL_ID "SceneHierarchyPanel"

  void Viewport::UpdateMousePos()
  {
    IK_PERFORMANCE_FUN();
    auto [mx, my] = ImGui::GetMousePos();
    mx -= bounds[0].x;
    my -= bounds[0].y;
    
    my = height - my;
    
    mousePosX = (int32_t)mx;
    mousePosY = (int32_t)my;
  }
  
  std::pair<float, float> Viewport::GetMouseSpace()
  {
    auto [mx, my] = ImGui::GetMousePos();
    mx -= bounds[0].x;
    my -= bounds[0].y;
    auto viewportWidth = bounds[1].x - bounds[0].x;
    auto viewportHeight = bounds[1].y - bounds[0].y;
    
    return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
  }
  
  std::pair<float, float> Viewport::GetMousePos()
  {
    return { mousePosX, mousePosY };
  }
  
  static constexpr float CamFarView = 1000.0f;
  RendererLayer* RendererLayer::s_instance = nullptr;
  
  RendererLayer& RendererLayer::Get()
  {
    return *s_instance;
  }

  RendererLayer::RendererLayer(Ref<UserPreferences> userPreference, const std::filesystem::path& clientResourcePath)
  : Layer("Kreator Renderer"), m_userPreferences(userPreference), m_clientResourcePath(clientResourcePath),
  m_editorCamera(45.0f, 1280.0f, 720.0f, 0.1f, CamFarView)  {
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
    
    // Adding Panels -----------------------------------------------------------------------------------------------
    m_panels.AddPanel<ProjectSettingsPanel>(PROJECT_SETTING_PANEL_ID, "Project Settings", true);
    m_panels.AddPanel<AssetPanel>(ASSET_MANAGER_PANEL_ID, "Assets", true);
    m_panels.AddPanel<ContentBrowserPanel>(CONTENT_BROWSER_PANEL_ID, "Content Browser", true);
    
    Ref<SceneHierarchyPanel> sceneHierarchyPanel = m_panels.AddPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL_ID,
                                                                                          "Scene Hierarchy", true,
                                                                                          m_editorScene);
    sceneHierarchyPanel->SetSelectionChangedCallback([this](SelectionContext entities) { OnEntitySelected(entities); });
    sceneHierarchyPanel->SetEntityDeletedCallback([this](SelectionContext entities) { OnEntityDeleted(entities); });
    
#ifdef DEBUG
    m_panels.AddPanel<KreatorConsolePanel>(CONSOLE_PANEL_ID, "Editor Log", true);
#endif

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
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::OnKeyPressedEvent));
    dispatcher.Dispatch<MouseButtonPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::OnMouseButtonPressed));
    
    m_panels.OnEvent(event);
  }

  bool RendererLayer::OnKeyPressedEvent(KeyPressedEvent& e)
  {
    return false;
  }
  
  bool RendererLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
  {
    return false;
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

  void RendererLayer::OnEntitySelected(SelectionContext entities)
  {
    if (entities.Size() == 0)
    {
      return;
    }
    
    m_selectionContext.clear();
    m_currentScene->ClearSelectedEntity();
    for (const auto& entity : entities)
    {
      SelectedSubmesh selection;
      selection.entity = entity;
      
      m_selectionContext.push_back(selection);
      if (m_currentScene != m_runtimeScene)
      {
        m_currentScene->SetSelectedEntity(entity);
      }
    }
  }
  void RendererLayer::OnEntityDeleted(SelectionContext entities)
  {
    if (m_selectionContext.size() == 0 or !entities.Find(m_selectionContext[0].entity))
    {
      return;
    }
    ClearSelectedEntity();
  }

  void RendererLayer::ClearSelectedEntity()
  {
    m_panels.GetPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSelectedEntity({});
    
    if (m_currentScene)
    {
      m_currentScene->ClearSelectedEntity();
    }
    
    m_selectionContext.clear();
  }
  
  void RendererLayer::SetSelectedEntity(Entity entity)
  {
    m_panels.GetPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSelectedEntity(entity);
  }

} // namespace Kreator
