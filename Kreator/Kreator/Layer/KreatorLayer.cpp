//
//  KreatorLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/02/24.
//

#include "KreatorLayer.hpp"
#include "KreatorUtils.h"
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
    
    // Increase Line render limit for Colliders
    Renderer2D::AddLineData(100000);

    // Set debug renderer callback
    m_viewportRenderer.SetDebugRenderer([this]() { DebugRenderer(); });
    
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
    
    // Guizmo Button
    m_selectToolTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Pointer.png"));
    m_moveToolTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Move.png"));
    m_rotateToolTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Rotate.png"));
    m_scaleToolTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Scale.png"));
    m_gizmoModeTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/GuizmoMode.png"));
    
    // Scene Button
    m_playButtonTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Play.png"));
    m_stopButtonTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Stop.png"));
    m_simulateButtonTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Simulate.png"));
    m_pauseButtonTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Pause.png"));

    // Camera Editor Icon
    m_XIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/X.png"));
    m_YIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Y.png"));
    m_ZIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Z.png"));
    m_cameraIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Camera.png"));
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
    AssetEditorManager::RegisterEditor<MaterialEditor>(AssetType::Material);
    AssetEditorManager::RegisterEditor<PrefabEditor>(AssetType::Prefab);

    SceneRenderer::Initialize();
  }
  
  void KreatorLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Detaching '{0} Layer' from application", GetName());
    
    // Clear the Asset Editor
    AssetEditorManager::Clear();
    
    // Close current Project
    Project::CloseActive();
    
    SceneRenderer::Shutdown();
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
        m_sceneHaveMainCamera = m_currentScene->GetMainCameraEntity() ? true : false;
        m_viewport.GetMouseViewportSpace();
        m_editorCamera.SetActive(m_allowViewportCameraEvents or Input::GetCursorMode() == CursorMode::Locked);
        m_editorCamera.OnUpdate(ts);
        
        // Render Main Viewport
        m_editorScene->OnUpdateEditor();
        m_editorScene->OnRenderEditor(m_editorCamera, m_viewportRenderer);
        
        // Mini viewport data render
        if (m_showMiniViewport and m_sceneHaveMainCamera)
        {
          m_editorScene->OnRenderRuntime(ts, m_miniViewportRenderer);
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
        m_sceneHaveMainCamera = m_currentScene->GetMainCameraEntity() ? true : false;
        m_viewport.GetMouseViewportSpace();
        m_editorCamera.SetActive(m_allowViewportCameraEvents or Input::GetCursorMode() == CursorMode::Locked);
        m_editorCamera.OnUpdate(ts);
        
        // Render Main Viewport
        m_simulationScene->OnUpdateRuntime(ts);
        m_simulationScene->OnRenderSimulation(ts, m_editorCamera, m_viewportRenderer);
        
        // Mini viewport data render
        if (m_showMiniViewport and m_sceneHaveMainCamera)
        {
          m_simulationScene->OnRenderRuntime(ts, m_miniViewportRenderer);
        }

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
    RETRUN_IF_NO_PROJECT();

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
      if (m_sceneState == SceneState::Edit)
      {
        m_currentScene->OnEditorEventHandler(event);
      }
      else
      {
        m_currentScene->OnRuntimeEventHandler(event);
      }
    }
    else
    {
      m_currentScene->OnRuntimeEventHandler(event);
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
      // Scene setting panel : TODO: Move to Editor panel?
      UI_SceneSettings();
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

    if (m_sceneState != SceneState::Play)
    {
      // Guizmo -----------------------------------------------------------
      if (leftCtrl and m_viewport.panelMouseHover and !Input::IsMouseButtonPressed(MouseButton::Right))
      {
        switch (e.GetKeyCode())
        {
          case Key::Q:
            m_gizmoType = -1;
            break;
          case Key::W:
            m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
          case Key::E:
            m_gizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
          case Key::R:
            m_gizmoType = ImGuizmo::OPERATION::SCALE;
            break;
          case Key::F:
          {
            if (m_selectionContext.size() == 0)
            {
              break;
            }
            
            Entity selectedEntity = m_selectionContext[0].entity;
            m_editorCamera.Focus(selectedEntity.GetTransform().Position());
            break;
          }
            
          default:
            break;
        }
      }
      
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
              if (m_selectionContext.size())
              {
                std::vector<Entity> duplicateEntities;
                duplicateEntities.resize(m_selectionContext.size());
                for (const auto& context : m_selectionContext)
                {
                  duplicateEntities.push_back(m_currentScene->DuplicateEntity(context.entity));
                }
                
                ClearSelectedEntity();
                for (const auto& entity : duplicateEntities)
                {
                  SetSelectedEntity(entity, true);
                }
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
      } // if Edit
      else if (m_sceneState == SceneState::Simulate)
      {
        if (Input::IsKeyPressed(Key::Escape))
        {
          OnSceneStopSimulation();
        }
      }
    } // if not Play
    else
    {
      if (Input::IsKeyPressed(Key::Escape))
      {
        OnSceneStop();
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
      if (!Input::IsKeyPressed(IKan::Key::LeftSuper))
      {
        ClearSelectedEntity();
      }
      else
      {
        m_selectionContext.clear();
      }

      auto [origin, direction] = CastRay(m_editorCamera);
      
      // for each mesh entity
      auto meshEntities = m_currentScene->GetAllEntitiesWith<MeshComponent>();
      for (auto e : meshEntities)
      {
        Entity entity = { e, m_currentScene.get() };
        auto& mc = entity.GetComponent<MeshComponent>();
        auto& vc = entity.GetComponent<VisibilityComponent>();
        if (float distance = KreatorUtils::CheckRayPassMesh(mc.mesh, entity, origin, direction); distance != -1 and vc.isVisible)
        {
          m_selectionContext.push_back({entity, distance});
        }
      } // Each Mesh Comp

      std::sort(m_selectionContext.begin(), m_selectionContext.end(), [](auto& a, auto& b) {
        return a.distance < b.distance;
      });

      if (m_selectionContext.size())
      {
        SetSelectedEntity(m_selectionContext[0].entity, Input::IsKeyPressed(Key::LeftSuper));
      }
    }
    return false;
  }

  void KreatorLayer::UpdateViewportSize()
  {
    IK_PROFILE()
    RETRUN_IF_NO_SCENE();
    m_editorCamera.SetViewportSize(m_viewport.width, m_viewport.height);
    m_currentScene->SetViewportSize(m_viewport.width, m_viewport.height);
    m_viewportRenderer.SetViewportSize(m_viewport.width, m_viewport.height);
    m_miniViewportRenderer.SetViewportSize(m_viewport.width, m_viewport.height);
    FixedCamera::SetViewport(m_viewport.width, m_viewport.height);
  }
  
  Ray KreatorLayer::CastRay(const EditorCamera& camera)
  {
    IK_PROFILE()
    glm::vec4 mouseClipPos = { m_viewport.mouseViewportSpace.spaceMouseX, m_viewport.mouseViewportSpace.spaceMouseY, -1.0f, 1.0f };
    
    auto inverseProj = glm::inverse(camera.GetProjectionMatrix());
    auto inverseView = glm::inverse(glm::mat3(camera.GetViewMatrix()));
    
    glm::vec4 ray = inverseProj * mouseClipPos;
    glm::vec3 rayPos = camera.GetPosition();
    glm::vec3 rayDir = inverseView * glm::vec3(ray);
    
    return { rayPos, rayDir };
  }

  void KreatorLayer::DebugRenderer()
  {
    IK_PERFORMANCE("KreatorLayer::RenderDebug");
    if (m_sceneState != SceneState::Play)
    {
      Renderer2D::BeginBatch(m_editorCamera.GetUnReversedViewProjection(), m_editorCamera.GetViewMatrix());
      {
        if (m_showRelationshipConnection)
        {
          RenderRelationshipConnection();
        }
        
        if (m_showCameraAxis)
        {
          RenderCameraAxis();
        }
        
        if (m_showColliders)
        {
          ShowColliders();
        }
        
        if (m_showDebugCameraControllerPath and m_sceneHaveMainCamera)
        {
          ShowCameraControllerPath();
        }

      }
      Renderer2D::EndBatch();
      
      if (m_showMiniViewport and m_sceneHaveMainCamera)
      {
        IK_PERFORMANCE("KreatorLayer::MiniViewport");

        static constexpr glm::mat4 unitMat4 = glm::mat4(1.0f);
        static constexpr glm::vec3 size = {1.0f/4.0f, -1.0f/4.0f, 1.0f};
        static constexpr glm::vec3 borderSize = {1.0f/4.0f + 0.01, -1.0f/4.0f - 0.01, 1.0f};
        static constexpr glm::vec3 position = {0.85f, -0.85f, 0.0f};
        
        Renderer2D::BeginBatch(unitMat4, unitMat4);
        Renderer2D::DrawQuad(position, size, Utils::Math::ZeroVec3, Utils::Math::UnitVec4, m_miniViewportRenderer.GetFinalImage());
        Renderer2D::DrawQuad(position, borderSize, Utils::Math::ZeroVec3, Utils::Math::UnitVec4);
        Renderer2D::EndBatch();
      }
      
      // CameraIcon
      if (m_sceneHaveMainCamera)
      {
        IK_PERFORMANCE("KreatorLayer::CameraIcon");
        Renderer2D::BeginBatch(m_editorCamera.GetUnReversedViewProjection(), m_editorCamera.GetViewMatrix());
        auto cameraEntities = m_currentScene->GetAllEntitiesWith<CameraComponent>();
        for (auto e : cameraEntities)
        {
          Entity entity = { e, m_currentScene.get() };
          const auto& tc = entity.GetComponent<TransformComponent>();
          
          Renderer2D::DrawFixedViewQuad(tc.Transform(), m_cameraIcon, Utils::Math::UnitVec4, 1, entity);
        } // Each Mesh Comp
        Renderer2D::EndBatch();
      }
    }
    
#ifdef DEBUG
      // Shows System info : Frame rate and Client name
      if (m_renderSystemInfo)
      {
        RenderSystemInfo();
      }
#endif
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
  
  void KreatorLayer::RenderRelationshipConnection()
  {
    IK_PERFORMANCE("KreatorLayer::RenderRelationshipConnection");
    auto relationshipView = m_currentScene->GetAllEntitiesWith<RelationshipComponent>();
    for (auto entityHandle : relationshipView)
    {
      Entity entity = { entityHandle, m_currentScene.get() };
      const auto& vc = entity.GetComponent<VisibilityComponent>();
      if (vc.isVisible)
      {
        const auto& tc = entity.GetComponent<TransformComponent>();
        const auto& startPos = tc.Position();
        
        for (const auto& child : entity.Children())
        {
          Entity childEntity = m_currentScene->TryGetEntityWithUUID(child);
          const auto& childTc = childEntity.GetComponent<TransformComponent>();
          const auto& endPos = childTc.Position();
          
          Renderer2D::DrawLine(startPos, endPos, m_relationshipColor);
        }
      }
    }
  }
  
  void KreatorLayer::RenderCameraAxis()
  {
    IK_PERFORMANCE("KreatorLayer::RenderCameraAxis");
    static constexpr float Limit = 10000.0f;
    
    static const glm::vec3 XMin = {-Limit, 0.0f, 0.0f};
    static const glm::vec3 XMax = { Limit, 0.0f, 0.0f};
    static const glm::vec4 XCol = {0.5f, 0.0f, 0.0f, 0.5f};
    
    static const glm::vec3 YMin = {0.0f, -Limit, 0.0f};
    static const glm::vec3 YMax = {0.0f,  Limit, 0.0f};
    static const glm::vec4 YCol = {0.0f, 0.5f, 0.0f, 0.5f};

    static const glm::vec3 ZMin = {0.0f, 0.0f, -Limit};
    static const glm::vec3 ZMax = {0.0f, 0.0f,  Limit};
    static const glm::vec4 ZCol = {0.0f, 0.0f, 0.5f, 0.5f};

    if (!m_editorCamera.IsRight())
    {
      Renderer2D::DrawLine(XMin, XMax, XCol);
    }
    if (!m_editorCamera.IsTop() and !m_editorCamera.IsFree())
    {
      Renderer2D::DrawLine(YMin, YMax, YCol);
    }
    if (!m_editorCamera.IsFront())
    {
      Renderer2D::DrawLine(ZMin, ZMax, ZCol);
    }
  }
  
  void KreatorLayer::ShowColliders()
  {
    IK_PERFORMANCE("KreatorLayer::ShowColliders");
    
    auto debugRenderer = m_currentScene->GetPhysicsDebugRenderer();
    auto triangle = debugRenderer.getTriangles();
    
    for (auto i = 0; i < debugRenderer.getNbTriangles(); i++)
    {
      Renderer2D::DrawLine({triangle[i].point1.x, triangle[i].point1.y, triangle[i].point1.z}, {triangle[i].point2.x, triangle[i].point2.y, triangle[i].point2.z}, m_colliderColor);
      Renderer2D::DrawLine({triangle[i].point2.x, triangle[i].point2.y, triangle[i].point2.z}, {triangle[i].point3.x, triangle[i].point3.y, triangle[i].point3.z}, m_colliderColor);
      Renderer2D::DrawLine({triangle[i].point3.x, triangle[i].point3.y, triangle[i].point3.z}, {triangle[i].point1.x, triangle[i].point1.y, triangle[i].point1.z}, m_colliderColor);
    }
  }
  
  void KreatorLayer::ShowCameraControllerPath()
  {
    IK_PERFORMANCE("KreatorLayer::ShowCameraControllerPath");
    auto cameraEntities = m_currentScene->GetAllEntitiesWith<CameraComponent>();
    for (auto e : cameraEntities)
    {
      Entity entity = { e, m_currentScene.get() };
      const auto& cc = entity.GetComponent<CameraComponent>();
      
      Entity FollowEntity = cc.controller.GetFollowEntity();
      if (FollowEntity and cc.controller.GetCameraViewType() == CameraController::ViewType::TPP)
      {
        const auto& followTc = FollowEntity.GetComponent<TransformComponent>();
        
        auto drawCircle = [this](const glm::vec3& position, float radius) {
          static glm::vec3 rotation = {glm::radians(90.0f), 0.0f, 0.0f};
          Renderer2D::DrawCircle(position, 2.0f * radius, rotation, m_cameraControllerColor, nullptr, 1.0f, 0.01f);
        };
        
        const auto& topOrbit = cc.controller.GetTopOrbit();
        const auto& midOrbit = cc.controller.GetMidOrbit();
        const auto& bottomOrbit = cc.controller.GetBottomOrbit();
        
        drawCircle({followTc.Position().x, followTc.Position().y + topOrbit.height, followTc.Position().z}, topOrbit.radius);
        drawCircle({followTc.Position().x, followTc.Position().y + midOrbit.height, followTc.Position().z}, midOrbit.radius);
        drawCircle({followTc.Position().x, followTc.Position().y + bottomOrbit.height, followTc.Position().z}, bottomOrbit.radius);
      }
    } // Each Mesh Comp
  }

  float KreatorLayer::GetSnapValue()
  {
    switch (m_gizmoType)
    {
      case ImGuizmo::OPERATION::TRANSLATE: return 0.5f;
      case ImGuizmo::OPERATION::ROTATE: return 45.0f;
      case ImGuizmo::OPERATION::SCALE: return 0.5f;
    }
    return 0.0f;
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
        std::filesystem::create_directory(projectDir / "Assets/Prefabs");
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
      NewScene(Project::GetActive()->GetConfig().name);
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
      cubeEntity.AddComponent<RigidBodyComponent>();
      cubeEntity.AddComponent<Box3DColliderComponent>();
    }

    // Update the scenes in Panels
    m_panels.SetSceneContext(m_currentScene);
  }
  
  void KreatorLayer::CloseCurrentScene()
  {
    IK_PROFILE();
    ClearSelectedEntity();
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
  
  void KreatorLayer::OpenScene(const AssetMetadata& assetMetadata)
  {
    std::filesystem::path workingDirPath = Project::GetAssetDirectory() / assetMetadata.filePath;
    OpenScene(workingDirPath.string());
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
    
#if 0 // hack to manage components
    for (auto e : m_currentScene->GetAllEntitiesWith<IDComponent>())
    {
      Entity en {e, m_currentScene.get()};
      en.AddComponent<VisibilityComponent>();
    }
#endif

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
  
  void KreatorLayer::SetSelectedEntity(const Entity& entity, bool multipleSelection)
  {
    IK_PROFILE();
    m_panels.GetPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSelectedEntity(entity, multipleSelection);
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
