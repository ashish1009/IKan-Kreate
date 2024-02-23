//
//  KreatorLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/02/24.
//

#include "KreatorLayer.hpp"
#include "Panel/KreatorConsolePanel.hpp"
#include "Panel/ContentBrowserPanel.hpp"
#include "Panel/ProjectSettingPanel.hpp"
#include "Panel/SceneHierarchyPanel.hpp"
#include "Panel/AssetPanel.hpp"
#include "Editor/AssetViewer.hpp"
#include "Editor/FolderExplorer.hpp"

namespace Kreator
{
#define RETRUN_IF_NO_PROJECT() \
if (!Project::GetActive()) return

#define RETRUN_IF_NO_SCENE() \
if (!m_currentScene) return

  // Kretor Resource Path
#define KreatorResourcePath(path) std::filesystem::absolute(m_clientResourcePath / path)
  
  // Panel IDs
#define CONSOLE_PANEL_ID "EditorConsolePanel"
#define CONTENT_BROWSER_PANEL_ID "ContentBrowserPanel"
#define PROJECT_SETTING_PANEL_ID "ProjectSetting"
#define ASSET_MANAGER_PANEL_ID "Assets"
#define SCENE_HIERARCHY_PANEL_ID "SceneHierarchyPanel"

  namespace KreatorUtils
  {
    /// This function replace the project name
    /// - Parameters:
    ///   - str: Project file content
    ///   - projectName: New project name
    static void ReplaceProjectName(std::string& str, const std::string& projectName)
    {
      static const char* projectNameToken = "$PROJECT_NAME$";
      size_t pos = 0;
      while ((pos = str.find(projectNameToken, pos)) != std::string::npos)
      {
        str.replace(pos, strlen(projectNameToken), projectName);
        pos += strlen(projectNameToken);
      }
    }
  } // namespace Utils

  // Viewport -----------------------------------------------------------------------------------------------------
  void Viewport::GetMouseViewportSpace()
  {
    IK_PERFORMANCE("Viewport::GetMouseViewportSpace");
    auto [mx, my] = ImGui::GetMousePos();
    const auto& viewportBounds = bounds;
    mx -= viewportBounds[0].x;
    my -= viewportBounds[0].y;
    auto viewportWidth = viewportBounds[1].x - viewportBounds[0].x;
    auto viewportHeight = viewportBounds[1].y - viewportBounds[0].y;
    
    mouseViewportSpace = { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
  }
  bool Viewport::IsMouseSpaceInViewport()
  {
    return (mouseViewportSpace.spaceMouseX >= -1.0f and mouseViewportSpace.spaceMouseX <= 1.0f and
            mouseViewportSpace.spaceMouseY >= -1.0f and mouseViewportSpace.spaceMouseY <= 1.0f);
  }

  // Kreator Layer ------------------------------------------------------------------------------------------------
  namespace KreatorUtils
  {
    static auto CheckRayPassMesh = [](AssetHandle meshHandle, Entity entity, const glm::vec3& origin, const glm::vec3& direction) -> float {
      const auto& mesh = AssetManager::GetAsset<Mesh>(meshHandle);
      if (!mesh || mesh->IsFlagSet(AssetFlag::Missing))
      {
        return -1;
      }
      
      const glm::mat4& transform = entity.GetComponent<TransformComponent>().Transform();
      auto& submeshes = mesh->GetSubMeshes();
      for (uint32_t i = 0; i < submeshes.size(); i++)
      {
        auto& submesh = submeshes[i];
        Ray ray =
        {
          glm::inverse(transform * submesh.transform) * glm::vec4(origin, 1.0f),
          glm::inverse(glm::mat3(transform * submesh.transform)) * direction
        };
        
        float distance;
        if (ray.IntersectsAABB(submesh.boundingBox, distance))
        {
          const auto& triangleCache = mesh->GetTriangleCache(i);
          for (const auto& triangle : triangleCache)
          {
            if (ray.IntersectsTriangle(triangle.V0.position, triangle.V1.position, triangle.V2.position, distance))
            {
              return distance;
            }
          } // For each triangle cache
        } // Bounding box intersect
      } // Each Submesh
      return -1;
    };
  } // namespace KreatorUtils
  
  KreatorLayer* KreatorLayer::s_instance = nullptr;
  KreatorLayer& KreatorLayer::Get()
  {
    return *s_instance;
  }

  KreatorLayer::KreatorLayer(const std::filesystem::path& clientResourcePath, const std::filesystem::path& systemUserPath, 
                             const std::filesystem::path& iKanKreatePath, Ref<UserPreferences> userPreferences)
  : Layer("Kreator Renderer"), m_clientResourcePath(clientResourcePath), m_systemUserPath(systemUserPath), m_iKanKreatePath(iKanKreatePath), 
  m_userPreferences(userPreferences), m_viewportRenderer("Viewport"), m_miniViewportRenderer("Mini Viewport"),
  m_editorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 10000.0f)
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "RendererLayer instance already created");
    
    // Copy the single instance of application
    s_instance = this;
    
    IK_LOG_INFO("Kreator Layer", "Creating Kreator Renderer Layer instance");
    
    // Set debug renderer callback
    m_viewportRenderer.SetDebugRenderer([this]() { RenderDebug(); });
    
    // Load Textures --------------------
    // Set the Application Icon
    m_applicationIcon = TextureFactory::Create(KreatorResourcePath("Textures/Logo/IKan.png"));
    m_welcomeIcon = TextureFactory::Create(KreatorResourcePath("Textures/Logo/WelcomeIKan.png"));
    
    // Other Textures
    m_newProjectIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/NewProject.png"));
    m_folderIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Folder.png"));
    m_gridTexture = TextureFactory::Create(KreatorResourcePath("Textures/Editor/Grid.png"));
    
    // Shadow Image
    m_shadowTexture = TextureFactory::Create(KreatorResourcePath("Textures/Icons/ShadowLineTop.png"));
    
    // Project Icon
    m_projectIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Project.png"));
    
    // Window Icons
    m_iconClose = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Close.png"));
    m_iconMinimize = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Minimize.png"));
    m_iconMaximize = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Maximize.png"));
    m_iconRestore = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Restore.png"));
  }
  
  KreatorLayer::~KreatorLayer()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void KreatorLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Attaching '{0} Layer' to application", GetName());
    
    // Decorate the Application --------------------------------------------------------------------------------------
    // Set all the required Fonts
    IKan::UI::ImGuiFont regularFontFilePath = {KreatorResourcePath("Fonts/Opensans/Regular.ttf"), 14};
    IKan::UI::ImGuiFont boldFontFilePath = {KreatorResourcePath("Fonts/Opensans/ExtraBold.ttf"), 14};
    IKan::UI::ImGuiFont italicFontFilePath = {KreatorResourcePath("Fonts/Opensans/Italic.ttf"), 14};
    IKan::UI::ImGuiFont sameWidthFont = {KreatorResourcePath("Fonts/HfMonorita/Regular.ttf"), 10};
    IKan::UI::ImGuiFont hugeheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 40};
    IKan::UI::ImGuiFont semiheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 18};
    
    // Note: This API should be called before any other ImGui Decoration API
    Kreator::UI::LoadFonts({regularFontFilePath, boldFontFilePath, italicFontFilePath, sameWidthFont, hugeheader, semiheader});
    
    // Set the Theme of ImGui as user preference
    Kreator::UI::SetThemeColors(m_userPreferences->theme);
    
    // Save the default project path
    std::filesystem::path defulatPath =  Utils::FileSystem::IKanAbsolute(m_clientResourcePath);
    m_projectFilePathBuffer.MemCpy(defulatPath.string().data(), 0, defulatPath.string().size());
    
    // Open or Create Project ---------------------------------------------------------------------------------------
    if (std::filesystem::exists(m_userPreferences->startupProject))
    {
      if (m_userPreferences->showWelcomeScreen)
      {
        m_showWelcomePopup = true;
      }
      else
      {
        IK_ASSERT(false, "Open Project. TODO: Implement Later ...");
      }
    }
    else
    {
      m_showWelcomePopup = true;
    }
    
    // Add Panels -----------------------
    m_panels.AddPanel<ContentBrowserPanel>(CONTENT_BROWSER_PANEL_ID, "Content Browser", true);
    m_panels.AddPanel<ProjectSettingsPanel>(PROJECT_SETTING_PANEL_ID, "Project Setting", true);
    m_panels.AddPanel<AssetPanel>(ASSET_MANAGER_PANEL_ID, "Assets", true);
    
    Ref<SceneHierarchyPanel> sceneHierarchyPanel = m_panels.AddPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL_ID, "Scene Hierarchy", true, m_editorScene);
    sceneHierarchyPanel->SetSelectionChangedCallback([this](SelectionContext entities) { OnEntitySelected(entities); });
    sceneHierarchyPanel->SetEntityDeletedCallback([this](SelectionContext entities) { OnEntityDeleted(entities); });

#if DEBUG == 1
    m_panels.AddPanel<KreatorConsolePanel>(CONSOLE_PANEL_ID, "Editor Log", true);
#endif
    
    // Register Default Asset Editor
    AssetEditorManager::RegisterEditor<ImageViewer>(AssetType::Image);
  }
  
  void KreatorLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Detaching '{0} Layer' from application", GetName());
    
    // Clear the Asset Editor
    AssetEditorManager::Clear();
  }
  
  void KreatorLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    RETRUN_IF_NO_SCENE();

    switch (m_sceneState)
    {
      case SceneState::Edit:
      {
        // Update Asset Viewer
        AssetEditorManager::OnUpdate(ts);
        
        // Update Data
        m_viewport.GetMouseViewportSpace();
        m_editorCamera.SetActive(m_allowViewportCameraEvents or Input::GetCursorMode() == CursorMode::Locked);
        m_editorCamera.OnUpdate(ts);
        
        // Render Main Viewport
        m_editorScene->OnUpdateEditor();
        m_editorScene->OnRenderEditor(m_editorCamera, m_viewportRenderer);
        
        // Hover Mesh
        {
          if (m_viewport.IsMouseSpaceInViewport())
          {
            
          }
        }

        // Save Scene Auto
        if (const auto& project = Project::GetActive(); project and project->GetConfig().enableAutoSave)
        {
          IK_PERFORMANCE("KreatorLayer::AutoSaveScene");
          m_timeSinceLastSave += ts;
          if (m_timeSinceLastSave > project->GetConfig().autoSaveIntervalSeconds)
          {
            SaveSceneAuto();
          }
        }
        break;
      }
      case SceneState::Simulate:
      {
        // Update Data
        m_viewport.GetMouseViewportSpace();
        m_editorCamera.SetActive(m_allowViewportCameraEvents or Input::GetCursorMode() == CursorMode::Locked);
        m_editorCamera.OnUpdate(ts);
        
        // Render Main Viewport
        m_simulationScene->OnUpdateRuntime(ts);
        m_simulationScene->OnRenderSimulation(ts, m_editorCamera, m_viewportRenderer);
        
        break;
      }
      case SceneState::Play:
      {
        Input::SetCursorMode(CursorMode::Locked);
        
        // Render Main Viewport
        m_runtimeScene->OnUpdateRuntime(ts);
        m_runtimeScene->OnRenderRuntime(ts, m_viewportRenderer);

        break;
      }
      case SceneState::Pause:
      {
        break;
      }
      default:
        break;
    };
  }
  
  void KreatorLayer::OnEvent(Event& event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(KreatorLayer::OnKeyPressedEvent));
    dispatcher.Dispatch<MouseButtonPressedEvent>(IK_BIND_EVENT_FN(KreatorLayer::OnMouseButtonPressed));

    if (m_sceneState != SceneState::Play)
    {
      m_panels.OnEvent(event);
      
      if (m_viewport.panelMouseHover)
      {
        m_editorCamera.OnEvent(event);
      }
      
      if (m_sceneState == SceneState::Edit)
      {
        AssetEditorManager::OnEvent(event);
      }
    }
  }
  
  void KreatorLayer::OnImGuiRender()
  {
    // Pop-up ------------------
    UI_WelcomePopup();
    UI_NewProjectPopup();
    UI_FolderExplorer();
    
    RETRUN_IF_NO_PROJECT();

    // Docked Windows-----------
    UI_StartMainWindowDocking();
    UI_Viewport();
    
    if (m_sceneState != SceneState::Play)
    {
      m_panels.OnImGuiRender();

      // Should be rendered last inside docker
      UI_StatisticsPanel();
    }

    UI_EndMainWindowDocking();
    
    UI_NewScenePopup();
    AssetEditorManager::OnImGuiRender();
  }
  
  bool KreatorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
  {
    bool leftCtrl = Input::IsKeyPressed(Key::LeftControl);
    bool rightCtrl = Input::IsKeyPressed(Key::RightControl);
    bool leftCmd = Input::IsKeyPressed(Key::LeftSuper);
    bool rightCmd = Input::IsKeyPressed(Key::RightSuper);
    bool leftShift = Input::IsKeyPressed(Key::LeftShift);
    bool rightShift = Input::IsKeyPressed(Key::RightShift);

    if (m_sceneState == SceneState::Edit)
    {
      // Scene -----------------------------------------------------------
      if ((leftCmd or rightCmd) and !Input::IsMouseButtonPressed(MouseButton::Right) and (!leftShift or !rightShift))
      {
        switch (e.GetKeyCode())
        {
          case Key::N:
            m_showNewScenePopup = true;
            break;
          case Key::O:
            OpenScene();
            break;
          case Key::S:
            SaveScene();
            break;
          default:
            break;
        }
      } // Scenes
      
      // Project -----------------------------------------------------------
      if ((leftCmd or rightCmd) and (leftShift or rightShift))
      {
        switch (e.GetKeyCode())
        {
          case Key::N:
            m_showCreateNewProjectPopup = true;
            break;
          case Key::O:
            FolderExplorer::Open(ProjectExtension, "");
            m_folderExplorerAction = FolderExplorerAction::OpenProject;
            break;
            
          default:
            break;
        }
      }

      // Entity -----------------------------------------------------------
      if (leftCtrl or rightCtrl)
      {
        switch (e.GetKeyCode())
        {
          case Key::D:
          {
            for (const auto& context : m_selectionContext)
            {
              SetSelectedEntity(m_currentScene->DuplicateEntity(context.entity));
            }
            break;
          }
          default:
            break;
        }
      }
      if (leftCmd)
      {
        switch (e.GetKeyCode())
        {
          case Key::D:
          {
            for (const auto& context : m_selectionContext)
            {
              m_currentScene->DestroyEntity(context.entity);
            }
            ClearSelectedEntity();
            break;
          }
          default:
            break;
        }
      }

    }
    return false;
  }
  
  bool KreatorLayer::OnMouseButtonPressed(MouseButtonPressedEvent &e)
  {
    if (m_sceneState != SceneState::Play)
    {
      if (Input::IsKeyPressed(IKan::Key::LeftControl) or ImGuizmo::IsOver() or m_hoveredGuizmoToolbar or !m_viewport.panelMouseHover)
      {
        return false;
      }
      
      if (!m_viewport.IsMouseSpaceInViewport())
      {
        return false;
      }
      
      // Clear all selected entity
      ClearSelectedEntity();

      auto [origin, direction] = CastRay(m_editorCamera);
      
      // for each mesh entity
      auto meshEntities = m_currentScene->GetAllEntitiesWith<MeshComponent>();
      for (auto e : meshEntities)
      {
        Entity entity = { e, m_currentScene.get() };
        auto& mc = entity.GetComponent<MeshComponent>();
        if (float distance = KreatorUtils::CheckRayPassMesh(mc.mesh, entity, origin, direction); distance != -1)
        {
          m_selectionContext.push_back({entity, distance});
        }
      } // Each Mesh Comp

      std::sort(m_selectionContext.begin(), m_selectionContext.end(), [](auto& a, auto& b) {
        return a.distance < b.distance;
      });

      if (m_selectionContext.size())
      {
        SetSelectedEntity(m_selectionContext[0].entity);
      }
    }
    return false;
  }

  void KreatorLayer::UpdateViewportSize()
  {
    RETRUN_IF_NO_SCENE();
    m_editorCamera.SetViewportSize(m_viewport.width, m_viewport.height);
    m_viewportRenderer.SetViewportSize(m_viewport.width, m_viewport.height);
    m_miniViewportRenderer.SetViewportSize(m_viewport.width, m_viewport.height);
    FixedCamera::SetViewport(m_viewport.width, m_viewport.height);
  }
  
  Ray KreatorLayer::CastRay(const EditorCamera& camera)
  {
    glm::vec4 mouseClipPos = { m_viewport.mouseViewportSpace.spaceMouseX, m_viewport.mouseViewportSpace.spaceMouseY, -1.0f, 1.0f };
    
    auto inverseProj = glm::inverse(camera.GetProjectionMatrix());
    auto inverseView = glm::inverse(glm::mat3(camera.GetViewMatrix()));
    
    glm::vec4 ray = inverseProj * mouseClipPos;
    glm::vec3 rayPos = camera.GetPosition();
    glm::vec3 rayDir = inverseView * glm::vec3(ray);
    
    return { rayPos, rayDir };
  }

  void KreatorLayer::RenderDebug()
  {
    IK_PERFORMANCE("KreatorLayer::RenderDebug");
    if (m_sceneState != SceneState::Play)
    {
      Renderer2D::BeginBatch(m_editorCamera.GetUnReversedViewProjection(), m_editorCamera.GetViewMatrix());
      {
      }
      Renderer2D::EndBatch();

      // Shows System info : Frame rate and Client name
      if (m_renderSystemInfo)
      {
        RenderSystemInfo();
      }
    }
  }
  
  void KreatorLayer::RenderSystemInfo()
  {
    IK_PERFORMANCE("KreatorLayer::RenderSystemInfo");
    static constexpr glm::vec3 position = { 5.0f, 5.0f, 0.3f };
    static constexpr glm::vec2 size = {0.3f, 0.3f};
    static constexpr glm::vec4 color = { 0.63f, 0.52f, 0.32f, 0.6f};
    
    TextRenderer::BeginBatch(FixedCamera::s_projection);
    TextRenderer::RenderFixedViewText("(c) IKAN", { m_viewport.width - 80, 5.0f, 0.3f }, size, color, Font::GetDefaultFont());
    TextRenderer::RenderFixedViewText(std::to_string((uint32_t)(ImGui::GetIO().Framerate)), position, size, color, Font::GetDefaultFont());
    TextRenderer::EndBatch();
  }
  
  void KreatorLayer::CreateProject(const std::filesystem::path& projectDir)
  {
    IK_PROFILE();
    if (!std::filesystem::exists(projectDir))
    {
      IK_LOG_INFO("Kreator Layer", "Creating Project at {0} ", Utils::FileSystem::IKanAbsolute(projectDir).string());
      
      // Close the current Project
      if (Project::GetActive())
      {
        CloseProject();
      }
      
      // Copy the template files
      std::filesystem::path templateProjectDir = m_clientResourcePath / "TemplateProject";
      if (Utils::FileSystem::Copy(templateProjectDir, projectDir))
      {
        // Open Template Project file
        std::ifstream stream(projectDir / "TemplateProject.ikproj");
        std::stringstream ss;
        ss << stream.rdbuf();
        stream.close();
        
        // Rename the Project name in file
        std::string str = ss.str();
        KreatorUtils::ReplaceProjectName(str, m_projectNameBuffer);
        
        // Open Project file again
        std::ofstream ostream(projectDir / "TemplateProject.ikproj");
        ostream << str;
        ostream.close();
        
        // Rename the file name
        std::string newProjectFileName = std::string(m_projectNameBuffer) + ProjectExtension;
        Utils::FileSystem::Rename(projectDir / "TemplateProject.ikproj", projectDir / newProjectFileName);
        
        // Create Required Directories
        std::filesystem::create_directory(projectDir / "Assets/Textures");
        std::filesystem::create_directory(projectDir / "Assets/Fonts");
        std::filesystem::create_directory(projectDir / "Assets/Scenes");
        std::filesystem::create_directory(projectDir / "Assets/Meshes");
        std::filesystem::create_directory(projectDir / "Assets/Materials");
      }
      else
      {
        IK_LOG_CRITICAL("Kreator Layer", "Directory could not be opened. Source directory : {0}. Destination directory : {1}",
                        templateProjectDir.string(), projectDir.string());
      }
    } // If project not exist
    
    std::filesystem::path projectFile = projectDir / (std::string(m_projectNameBuffer) + ProjectExtension);
    OpenProject(projectFile);
  }
  
  void KreatorLayer::CloseProject()
  {
    IK_ASSERT(false);
  }
  
  void KreatorLayer::OpenProject(const std::filesystem::path &filepath)
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Opening Project {0}", Utils::FileSystem::IKanAbsolute(filepath).string());
    
    // Check if File exists
    if (!std::filesystem::exists(filepath))
    {
      IK_ASSERT(false, "Tried to open a project that doesn't exist. Need to fix ...");
    }

    // Close the current Project
    if (Project::GetActive())
    {
      CloseProject();
    }

    // Create new project fill the config with file data
    Ref<Project> project = CreateRef<Project>();
    ProjectSerializer serializer(project);
    serializer.Deserialize(filepath);
    Project::SetActive(project);
    
    // Update all panel project
    m_panels.OnProjectChanged(project);

    // Create or open Scene
    if (!project->GetConfig().startScene.empty())
    {
      OpenScene((Project::GetAssetDirectory() / project->GetConfig().startScene).string());
    }
    else
    {
      NewScene();
    }

    // Push the current project in recent list
    PushProjectToRecentProjects(filepath);
  }
  
  void KreatorLayer::OpenProject()
  {
    IK_PROFILE();
    RETURN_IF (m_openProjectPath == "")
    
    // stash the filepath away.  Actual opening of project is deferred until it is "safe" to do so.
    m_projectFilePathBuffer.StrCpy(m_openProjectPath.string());
    OpenProject(m_openProjectPath);
  }
  
  void KreatorLayer::PushProjectToRecentProjects(const std::filesystem::path &projectPath)
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Pushing Project {0} in Recent list", Utils::FileSystem::IKanAbsolute(projectPath).string());
    RecentProject projectEntry;
    projectEntry.name = Utils::String::RemoveExtension(projectPath.filename().string());
    projectEntry.filePath = projectPath;
    projectEntry.lastOpened = time(NULL);
    
    for (auto it = m_userPreferences->recentProjects.begin(); it != m_userPreferences->recentProjects.end(); it++)
    {
      if (it->second.name == projectEntry.name)
      {
        m_userPreferences->recentProjects.erase(it);
        break;
      }
    }
    
    m_userPreferences->recentProjects[projectEntry.lastOpened] = projectEntry;
    UserPreferencesSerializer serializer(m_userPreferences);
    serializer.Serialize(m_userPreferences->filePath);
  }
  
  void KreatorLayer::NewScene(const std::string& name)
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Creating new scene: {0}", name);
    
    // Close current scene
    CloseCurrentScene();
    
    // Create new scene
    m_editorScene = Scene::Create(name);
    m_sceneFilePath = std::string();
    m_currentScene = m_editorScene;
    
    // Add Default entities
    {
      // Default Cube Entity
      static const std::string DefaultEntityMesh = "Meshes/Default/Cube.fbx";
      Entity cubeEntity = m_currentScene->CreateEntity("Cube");
      auto& mc = cubeEntity.AddComponent<MeshComponent>();
      mc.mesh = AssetManager::GetAsset<Mesh>(DefaultEntityMesh)->handle;
    }

    // Update the scenes in Panels
    m_panels.SetSceneContext(m_currentScene);
  }
  
  void KreatorLayer::CloseCurrentScene()
  {
    IK_PROFILE();
    if (m_currentScene)
    {
      m_currentScene->OnClose();
    }
  }
  
  void KreatorLayer::SaveSceneAs()
  {
    FolderExplorer::Save(Project::GetSceneDirectory());
    m_folderExplorerAction = FolderExplorerAction::SaveScene;
  }
  
  void KreatorLayer::SaveScene()
  {
    IK_PROFILE();
    if (!m_sceneFilePath.empty())
    {
      IK_LOG_INFO("Kreator Layer", "Saving the scene: {0}", m_sceneFilePath.c_str());
      SceneSerializer serializer(m_editorScene);
      serializer.Serialize(m_sceneFilePath);
      m_timeSinceLastSave = 0.0f;
    }
    else
    {
      SaveSceneAs();
    }
  }
  void KreatorLayer::SaveSceneAuto()
  {
    IK_PROFILE();
    if (!m_sceneFilePath.empty())
    {
      SceneSerializer serializer(m_editorScene);
      std::string modFilePath = m_sceneFilePath.string() + ".auto";
      serializer.Serialize(modFilePath);
      m_timeSinceLastSave = 0.0f;
    }
  }

  void KreatorLayer::OpenScene()
  {
    FolderExplorer::Open(SceneExtension, Project::GetSceneDirectory());
    m_folderExplorerAction = FolderExplorerAction::OpenScene;
  }

  void KreatorLayer::OpenScene(const std::filesystem::path &filepath)
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Opening scene: {0}", filepath.string());

    CloseCurrentScene();

    if (!std::filesystem::exists(filepath))
    {
      IK_LOG_ERROR("Kreator Layer" ,"Tried loading a non-existing scene: {0}", filepath.string());
      NewScene(filepath.filename());
      m_sceneFilePath = filepath;
      SaveScene();
      return;
    }

    Ref<Scene> newScene = Scene::Create("New Scene");
    SceneSerializer serializer(newScene);
    serializer.Deserialize(filepath);

    m_editorScene = newScene;
    m_currentScene = m_editorScene;
    m_sceneFilePath = filepath;
    
    // Update Panel Scene
    m_panels.SetSceneContext(m_currentScene);
  }
  
  void KreatorLayer::OnScenePlay()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Scene Set to Play");
    
    // 0. Clear all selected Entities
    ClearSelectedEntity();

    // 1. Update Scene state
    m_sceneState = SceneState::Play;
    
    // 2. Create Runtime Scene
    m_runtimeScene = Scene::Create();
    m_editorScene->CopyTo(m_runtimeScene);
    m_runtimeScene->OnRuntimeStart();
    
    // 3. Update current scene reference
    m_currentScene = m_runtimeScene;
    
    // 4. Update Panels
    m_panels.SetSceneContext(m_currentScene);
  }
  
  void KreatorLayer::OnSceneStop()
  {
    IK_PROFILE();
    RETURN_IF(!m_runtimeScene);
    
    IK_LOG_INFO("Kreator Layer", "Scene Set to Edit");
    
    // 0. Clear all selected Entities
    ClearSelectedEntity();

    // 1. Update Scene state
    m_sceneState = SceneState::Edit;
    
    // 2. Stop Runtime Scene
    m_runtimeScene->OnRuntimeStop();
    m_runtimeScene.reset();
    m_runtimeScene = nullptr;
    
    // 3. Update current scene reference
    m_currentScene = m_editorScene;
    
    // 4. Update Panels
    m_panels.SetSceneContext(m_currentScene);
  }
  
  void KreatorLayer::OnScenePause()
  {
    IK_PROFILE();
  }
  
  void KreatorLayer::OnSceneResume()
  {
    IK_PROFILE();
  }
  
  void KreatorLayer::OnSceneStartSimulation()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Scene Set for Simulation");
    
    // 0. Clear all selected Entities
    ClearSelectedEntity();

    // 1. Update Scene state
    m_sceneState = SceneState::Simulate;
    
    // 2. Stop Runtime Scene
    m_simulationScene = Scene::Create();
    m_editorScene->CopyTo(m_simulationScene);
    m_simulationScene->OnSimulationStart();
    
    // 3. Update current scene reference
    m_currentScene = m_simulationScene;
    
    // 4. Update Panels
    m_panels.SetSceneContext(m_currentScene);
  }
  
  void KreatorLayer::OnSceneStopSimulation()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Scene Simulation stopped");
    
    // 0. Clear all selected Entities
    ClearSelectedEntity();
    
    // 1. Update Scene state
    m_sceneState = SceneState::Edit;
    
    // 2. Stop Runtime Scene
    m_simulationScene->OnRuntimeStop();
    m_simulationScene.reset();
    m_simulationScene = nullptr;
    
    // 3. Update current scene reference
    m_currentScene = m_editorScene;
    
    // 4. Update Panels
    m_panels.SetSceneContext(m_currentScene);
  }
  
  void KreatorLayer::OnEntitySelected(const SelectionContext& entities)
  {
    RETURN_IF (entities.Size() == 0)
    
    m_selectionContext.clear();
    m_currentScene->ClearSelectedEntity();
    for (const auto& entity : entities)
    {
      SelectedEntity selection;
      selection.entity = entity;
      
      m_selectionContext.push_back(selection);
      if (m_currentScene != m_runtimeScene)
      {
        m_currentScene->SetSelectedEntity(entity);
      }
    }
  }
  void KreatorLayer::OnEntityDeleted(const SelectionContext& entities)
  {
    IK_PROFILE();
    RETURN_IF (m_selectionContext.size() == 0 or !entities.Find(m_selectionContext[0].entity))
    ClearSelectedEntity();
  }
  
  void KreatorLayer::ClearSelectedEntity()
  {
    IK_PROFILE();
    m_panels.GetPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSelectedEntity({});
    
    if (m_currentScene)
    {
      m_currentScene->ClearSelectedEntity();
    }
    m_selectionContext.clear();
  }
  
  void KreatorLayer::SetSelectedEntity(const Entity& entity)
  {
    IK_PROFILE();
    m_panels.GetPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSelectedEntity(entity);
  }
  bool KreatorLayer::IsEntitySelected(Entity e) const
  {
    IK_ASSERT(false)
  }

  const std::filesystem::path& KreatorLayer::GetClientResorucePath() const
  {
    return m_clientResourcePath;
  }
  const std::filesystem::path& KreatorLayer::GetSystemUserPath() const
  {
    return m_systemUserPath;
  }
  const std::filesystem::path& KreatorLayer::GetIKanKreatorPath() const
  {
    return m_iKanKreatePath;
  }
} // namespace Kreator
