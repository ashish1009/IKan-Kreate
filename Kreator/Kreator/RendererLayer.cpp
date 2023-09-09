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
#define RETRUN_IF_NO_PROJECT() \
if (!Project::GetActive()) return
  
  // Kretor Resource Path
#define KreatorResourcePath(path) Utils::FileSystem::Absolute(m_clientResourcePath / path)
  
  // Panel IDs
#define CONSOLE_PANEL_ID "EditorConsolePanel"
#define PROJECT_SETTING_PANEL_ID "ProjectSetting"
#define ASSET_MANAGER_PANEL_ID "Assets"
#define CONTENT_BROWSER_PANEL_ID "ContentBrowserPanel"
#define SCENE_HIERARCHY_PANEL_ID "SceneHierarchyPanel"
  
  static constexpr float CamFarView = 1000.0f;
  
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
  
  // Kreator UI utils
  namespace UI_Utils
  {
    // Hovered Item Color
    static const ImU32 s_hoveredColor = Kreator_UI::Color::HoveredItem;
    
    // Function to push Dark color on active
    static const auto pushDarkTextIfActive = [](const char* menuName)
    {
      if (ImGui::IsPopupOpen(menuName))
      {
        ImGui::PushStyleColor(ImGuiCol_Text, UI::Theme::Color::BackgroundDark);
        return true;
      }
      return false;
    };
    
    /// This function Create template for Menu Item in Menu Bar
    /// - Parameters:
    ///   - title: Title of Menu
    ///   - func: function of Menu
    ///   - pop_func: Pop item highlight
    template<typename PopFunction, typename UIFunction>
    void AddMenu(const char* title, PopFunction popItemHighlight, UIFunction menuFunc)
    {
      // Change Text colored to dark if item is opened
      bool pushItemColor = pushDarkTextIfActive(title);
      
      if (ImGui::BeginMenu(title))
      {
        popItemHighlight();
        {
          UI::ScopedColor hovered(ImGuiCol_HeaderHovered, s_hoveredColor);
          menuFunc();
        }
        ImGui::EndMenu();
      }
      if (pushItemColor)
      {
        ImGui::PopStyleColor();
      }
    }
    
  } // namespace UI_Utils
  
  void Viewport::UpdateMousePos()
  {
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
  
  RendererLayer* RendererLayer::s_instance = nullptr;
  
  RendererLayer& RendererLayer::Get()
  {
    return *s_instance;
  }
  
  RendererLayer::RendererLayer(Ref<UserPreferences> userPreference, const std::filesystem::path& clientResourcePath)
  : Layer("Kreator Renderer"), m_userPreferences(userPreference), m_clientResourcePath(clientResourcePath),
  m_editorCamera(45.0f, 1280.0f, 720.0f, 0.1f, CamFarView)
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
    
    // Overriden Shader
    m_jointShader = Shader::Create(KreatorResourcePath("Shader/JointMeshShader.glsl"));
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
    
    // Decorate the Theme ------------------------------------------------------------------------------------------
    // Set all the required Fonts
    UI::Font regularFontFilePath = {KreatorResourcePath("Fonts/Opensans/Regular.ttf"), 14};
    UI::Font boldFontFilePath = {KreatorResourcePath("Fonts/Opensans/ExtraBold.ttf"), 14};
    UI::Font italicFontFilePath = {KreatorResourcePath("Fonts/Opensans/Italic.ttf"), 14};
    UI::Font sameWidthFont = {KreatorResourcePath("Fonts/HfMonorita/Regular.ttf"), 10};
    UI::Font hugeheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 40};
    UI::Font semiheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 18};
    UI::Theme::ChangeFont({regularFontFilePath, boldFontFilePath, italicFontFilePath, sameWidthFont, hugeheader, semiheader});
    
    // Set the Theme of ImGui as user preference
    Kreator_UI::SetTheme(m_userPreferences->theme);
    
    // Open or Create Project ---------------------------------------------------------------------------------------
    if (Utils::FileSystem::Exists(m_userPreferences->startupProject))
    {
      if (m_userPreferences->showWelcomeScreen)
      {
        m_showWelcomePopup = true;
      }
      else
      {
        IK_ASSERT(false, "Open the Project");
      }
    }
    else
    {
      m_showWelcomePopup = true;
    }
    
    // Initilaize the Renderers
    Renderer2D::Initialize({1000, 1000, 1000000});
    m_viewportRenderer = CreateRef<SceneRenderer>(m_currentScene);
    m_miniViewportRenderer = CreateRef<SceneRenderer>(m_currentScene);

    // Register Default Asset Editor
    AssetEditorManager::RegisterEditor<ImageViewer>(AssetType::Image);
  }
  
  void RendererLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Detaching Kreator Renderer Layer from application");
    
    // Shutdown the renderers
    Renderer2D::Shutdown();
    m_viewportRenderer.reset();
    m_miniViewportRenderer.reset();
    
    // Close the project
    Project::CloseActive();
    
    // Destroy Default mesh if left
    DefaultMesh::Shutdown();
  }
  
  void RendererLayer::OnUpdate(TimeStep ts)
  {
    RETRUN_IF_NO_PROJECT();
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    
    switch (m_sceneState)
    {
      case SceneState::Edit:
      {
        // Update Data
        AssetEditorManager::OnUpdate(ts);
        m_viewport.UpdateMousePos();
        m_viewport.UpdateMousePos();
        m_editorCamera.SetActive(m_allowViewportCameraEvents or Input::GetCursorMode() == CursorMode::Locked);
        m_editorCamera.OnUpdate(ts);
        
        // Render Main Viewport
        {
          m_viewportRenderer->BeginRenderPass();
          Renderer::Clear({0.26f, 0.26f, 0.29f, 1.0f});
          
          m_editorScene->OnUpdateEditor(ts);
          m_editorScene->OnRenderEditor(m_editorCamera, m_viewportRenderer);
          
          RenderDebug();
          RenderFixTexts();
          UpdateHoveredEntity();
          
          if (m_showMiniViewport)
          {
            RenderMiniViewport();
          }
          
          m_viewportRenderer->EndRenderPass();
        }
        
        // Render Mini Viewport
        if (m_showMiniViewport)
        {
          m_miniViewportRenderer->BeginRenderPass();
          Renderer::Clear({0.26f, 0.26f, 0.29f, 0.3f});

          m_editorScene->OnRenderRuntime(ts, m_viewportRenderer);
          
          m_miniViewportRenderer->EndRenderPass();
        }
        
        // Save Scene Auto
        if (const auto& project = Project::GetActive(); project and project->GetConfig().enableAutoSave)
        {
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
        AssetEditorManager::OnUpdate(ts);
        m_viewport.UpdateMousePos();
        m_editorCamera.SetActive(m_allowViewportCameraEvents or Input::GetCursorMode() == CursorMode::Locked);
        m_editorCamera.OnUpdate(ts);
        
        // Renderer for Main Viewport
        m_viewportRenderer->BeginRenderPass();
        Renderer::Clear({0.26f, 0.26f, 0.29f, 1.0f});

        m_simulationScene->OnUpdateRuntime(ts);
        m_simulationScene->OnRenderSimulation(ts, m_editorCamera, m_viewportRenderer);
        
        RenderDebug();
        RenderFixTexts();

        m_viewportRenderer->EndRenderPass();
        break;
      }
      case SceneState::Play:
      {
        // Renderer for Main Viewport
        m_viewportRenderer->BeginRenderPass();
        Renderer::Clear({0.26f, 0.26f, 0.29f, 1.0f});

        m_runtimeScene->OnUpdateRuntime(ts);
        m_runtimeScene->OnRenderRuntime(ts, m_viewportRenderer);

        RenderFixTexts();
 
        m_viewportRenderer->EndRenderPass();
        break;
      }
      case SceneState::Pause:
      {
        break;
      }
        
      default:
        break;
    }
  }
  
  void RendererLayer::OnEvent(Event& event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::OnKeyPressedEvent));
    dispatcher.Dispatch<MouseButtonPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::OnMouseButtonPressed));
    
    m_panels.OnEvent(event);
    
    if (m_viewport.panelMouseHover)
    {
      m_editorCamera.OnEvent(event);
    }
    AssetEditorManager::OnEvent(event);
  }
  
  bool RendererLayer::OnKeyPressedEvent(KeyPressedEvent& e)
  {
    bool leftCmd = Input::IsKeyPressed(Key::LeftSuper);
    bool rightCmd = Input::IsKeyPressed(Key::RightSuper);
    bool leftShift = Input::IsKeyPressed(Key::LeftShift);
    bool rightShift = Input::IsKeyPressed(Key::RightShift);
    
    // Guizmo -----------------------------------------------------------
    if (m_viewport.panelMouseHover and !Input::IsMouseButtonPressed(MouseButton::Right) and m_currentScene != m_runtimeScene)
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
          m_editorCamera.Focus(selectedEntity.Transform().Position());
          break;
        }
          
        default:
          break;
      }
    }
    
    // Scene -----------------------------------------------------------
    if (leftCmd and !Input::IsMouseButtonPressed(MouseButton::Right))
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
      
      if (leftShift or rightShift)
      {
        switch (e.GetKeyCode())
        {
          case Key::S:
            SaveSceneAs();
            break;
          default:
            break;
        }
      }
    }
    
    // Stop Scene --------------------------------------------------------
    if (m_sceneState == SceneState::Play and e.GetKeyCode() == Key::Escape)
    {
      OnSceneStop();
    }
    
    // Project -----------------------------------------------------------
    if ((leftCmd or rightCmd) and (leftShift or rightShift))
    {
      switch (e.GetKeyCode())
      {
        case Key::N:
          m_showCreateNewProjectPopup = true;
          break;
        case Key::O:
          FolderExplorer::OpenPopup("Open Project", m_allProjectsPath);
          m_folderExplorerAction = FolderExplorerAction::OpenProject;
          break;
          
        default:
          break;
      }
    }
    
    return false;
  }
  
  void RendererLayer::UpdateViewportSize()
  {
    m_viewportRenderer->SetViewport(m_viewport.width, m_viewport.height);
    m_editorCamera.SetViewportSize(m_viewport.width, m_viewport.height);
    m_currentScene->SetViewportSize(m_viewport.width, m_viewport.height);
    
    FixedCamera::SetViewport(m_viewport.width, m_viewport.height);
  }
  
  void RendererLayer::UpdateHoveredEntity()
  {
    if (ImGuizmo::IsOver())
    {
      m_hoveredEntityID = (int32_t)m_currentScene->GetSelectedEntity();
      return;
    }
    
    m_viewportRenderer->GetEntityIdFromPixels(m_viewport.mousePosX, m_viewport.mousePosY, m_hoveredEntityID);
#if 0
    IK_CONSOLE_TRACE("", "{0}", m_hoveredEntityID);
#endif
  }
  
  void RendererLayer::RenderMiniViewport()
  {
    static constexpr glm::mat4 unitMat4 = glm::mat4(1.0f);
    static constexpr glm::vec3 size = {1.0f/4.0f, -1.0f/4.0f, 1.0f};
    static constexpr glm::vec3 position = {0.88f, 0.88f, 0.0f};
    static constexpr glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    
    Renderer2D::BeginBatch(unitMat4, unitMat4);
    Renderer2D::DrawQuad(position, size, rotation, m_miniViewportRenderer->GetFinalImage());
    Renderer2D::EndBatch();
  }
    
  float RendererLayer::GetSnapValue()
  {
    switch (m_gizmoType)
    {
      case ImGuizmo::OPERATION::TRANSLATE: return 0.5f;
      case ImGuizmo::OPERATION::ROTATE: return 45.0f;
      case ImGuizmo::OPERATION::SCALE: return 0.5f;
    }
    return 0.0f;
  }
  
  std::pair<float, float> RendererLayer::GetMouseViewportSpace()
  {
    auto [mx, my] = ImGui::GetMousePos();
    const auto& viewportBounds = m_viewport.bounds;
    mx -= viewportBounds[0].x;
    my -= viewportBounds[0].y;
    auto viewportWidth = viewportBounds[1].x - viewportBounds[0].x;
    auto viewportHeight = viewportBounds[1].y - viewportBounds[0].y;
    
    return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
  }
  
  std::pair<glm::vec3, glm::vec3> RendererLayer::CastRay(const EditorCamera& camera, float mx, float my)
  {
    glm::vec4 mouseClipPos = { mx, my, -1.0f, 1.0f };
    
    auto inverseProj = glm::inverse(camera.GetProjectionMatrix());
    auto inverseView = glm::inverse(glm::mat3(camera.GetViewMatrix()));
    
    glm::vec4 ray = inverseProj * mouseClipPos;
    glm::vec3 rayPos = camera.GetPosition();
    glm::vec3 rayDir = inverseView * glm::vec3(ray);
    
    return { rayPos, rayDir };
  }
  
  bool RendererLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
  {
    if (Input::IsKeyPressed(IKan::Key::LeftControl) or ImGuizmo::IsOver() or m_hoveredGuizmotoolbar or !m_viewport.panelMouseHover)
    {
      return false;
    }
    
    {
      auto [mouseX, mouseY] = m_viewport.GetMousePos();
      if (mouseX <= 0 or mouseY <= 0 or mouseX >= m_viewport.width or mouseY >= m_viewport.height)
      {
        return false;
      }
      
      if (m_hoveredEntityID >= 0 and m_hoveredEntityID <= (int32_t)m_currentScene->GetMaxEntityId())
      {
        ClearSelectedEntity();
        Entity selectedEntity = m_currentScene->GetEntityWithEntityHandle(m_hoveredEntityID);
        
        SetSelectedEntity(selectedEntity);
        return false;
      }
    }
    
    auto [spaceMouseX, spaceMouseY] = GetMouseViewportSpace();
    if (spaceMouseX > -1.0f and spaceMouseX < 1.0f and spaceMouseY > -1.0f and spaceMouseY < 1.0f)
    {
      ClearSelectedEntity();
      
      const auto& camera = m_editorCamera;
      auto [origin, direction] = CastRay(camera, spaceMouseX, spaceMouseY);
      
      auto meshEntities = m_currentScene->GetAllEntitiesWith<StaticMeshComponent>();
      for (auto e : meshEntities)
      {
        Entity entity = { e, m_currentScene.get() };
        auto& mc = entity.GetComponent<StaticMeshComponent>();
        auto mesh = AssetManager::GetAsset<MeshSource>(mc.staticMesh);
        if (!mesh || mesh->IsFlagSet(AssetFlag::Missing))
        {
          continue;
        }
        
#ifdef WorldSpace
        glm::mat4 transform = m_currentScene->GetWorldSpaceTransformMatrix(entity);
#else
        glm::mat4 transform = entity.GetComponent<TransformComponent>().Transform();
#endif
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
                m_selectionContext.push_back({entity, distance});
                break;
              }
            } // For each triangle cache
          } // Bounding box intersect
        } // For each submesh
      } // For each Mesh Entity
      
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
  
  void RendererLayer::OnImGuiRender()
  {
    IK_PERFORMANCE("RendererLayer::OnImGuiRender");
    
    // Should be above all scene GUI
    UI_WelcomePopup();
    UI_NewProjectPopup();
    UI_FolderExplorer();
    
    RETRUN_IF_NO_PROJECT();
    
    // Dockings
    UI_StartMainWindowDocking();
    UI_Viewport();
    
    if (m_sceneState != SceneState::Play)
    {
      UI_StatisticsPanel();
      UI_EditorPanel();
      m_panels.OnImGuiRender();
    }
    
    UI_EndMainWindowDocking();
    
    // Popups
    UI_NewScene();
    UI_AboutPopup();
    
    AssetEditorManager::OnImGuiRender();
  }
  
  void RendererLayer::UpdateWindowTitle(const std::string& sceneName)
  {
    const auto& caps = Renderer::Capabilities::Get();
    const std::string title = fmt::format("{0} ({1}) - Kreator - {2} ({3}) Renderer: {4}",
                                          sceneName, Project::GetActive()->GetConfig().name,
                                          caps.vendor, caps.version, caps.renderer);
    Application::Get().GetWindow().SetTitle(title);
  }
  
  void RendererLayer::RenderDebug()
  {
    Renderer2D::BeginBatch(m_editorCamera.GetUnReversedViewProjection(), m_editorCamera.GetViewMatrix());
    
    if (m_showIcons)
    {
      ShowIcons();
    }
    
    if (m_showColliders)
    {
      ShowColliders({0, 1, 0, 1});
    }
    
    if (m_showGrid)
    {
      ShowGrid({0.6, 0.6, 0.6, 0.3f});
    }
    
    Renderer2D::EndBatch();

    // Joints are in 3D Renderer
    if (m_showColliders)
    {
      ShowJoints({0, 0, 1, 1});
    }
  }
  
  void RendererLayer::ShowIcons()
  {
    if (m_sceneState != SceneState::Edit)
    {
      return;
    }
    
    auto cameraEntities = m_currentScene->GetAllEntitiesWith<CameraComponent>();
    for (auto e : cameraEntities)
    {
      Entity entity = { e, m_currentScene.get() };
#ifdef WorldSpace
      Renderer2D::DrawFixedViewQuad(m_currentScene->GetWorldSpaceTransform(entity).Transform(), m_cameraIcon);
#else
      const auto& tc = entity.GetComponent<TransformComponent>();
      Renderer2D::DrawFixedViewQuad(tc.Position(), tc.Scale(), m_cameraIcon, glm::vec4(1.0f), 1, (uint32_t)entity);
#endif
    }
  }
  
  void RendererLayer::ShowColliders(const glm::vec4& color)
  {
    if(m_sceneState != SceneState::Simulate)
    {
      return;
    }
    
    auto debugRenderer = m_currentScene->GetPhysicsDebugRenderer();
    auto triangle = debugRenderer.getTriangles();
    
    for (auto i = 0; i < debugRenderer.getNbTriangles(); i++)
    {
      Renderer2D::DrawLine({triangle[i].point1.x, triangle[i].point1.y, triangle[i].point1.z},
                           {triangle[i].point2.x, triangle[i].point2.y, triangle[i].point2.z}, color);
      Renderer2D::DrawLine({triangle[i].point2.x, triangle[i].point2.y, triangle[i].point2.z},
                           {triangle[i].point3.x, triangle[i].point3.y, triangle[i].point3.z}, color);
      Renderer2D::DrawLine({triangle[i].point3.x, triangle[i].point3.y, triangle[i].point3.z},
                           {triangle[i].point1.x, triangle[i].point1.y, triangle[i].point1.z}, color);
    }
  }
  
  void RendererLayer::ShowJoints(const glm::vec4 &color)
  {
    Renderer::EnableStencilPass();
    m_viewportRenderer->BeginScene({m_editorCamera.GetUnReversedViewProjection(), m_editorCamera.GetDistance()});
    
    static constexpr glm::vec3 unitScale = {1, 1, 1};
    static constexpr glm::mat4 unitMat(1.0f);

    auto jointEntities = m_currentScene->GetAllEntitiesWith<JointComponent>();
    for (auto e : jointEntities)
    {
      Entity entity = { e, m_currentScene.get() };
      if (!IsEntitySelected(entity))
      {
        continue;
      }
      
      const auto& fjc = entity.GetComponent<JointComponent>();
      auto connectedEntity = m_currentScene->TryGetEntityWithUUID(fjc.connectedEntity);
      if (!connectedEntity)
      {
        continue;
      }
      
      if (fjc.isWorldSpace)
      {
        m_viewportRenderer->SubmitMeshSource(DefaultMesh::GetMesh(DefaultMesh::Type::Sphere),
                                             glm::scale(glm::translate(unitMat, fjc.worldAnchorPoint), {0.2, 0.2, 0.2}),
                                             m_jointShader);
      }
      else
      {
        auto drawJoint = [&color, this](const Entity& entity, const glm::vec3& localAnchorPoint) {
          
          const auto& tc = entity.GetComponent<TransformComponent>();
          const glm::mat4 posRotTransform = Utils::Math::GetTransformMatrix(tc.Position(), tc.Rotation(), unitScale);
          const glm::vec3 position = posRotTransform * glm::vec4(localAnchorPoint, 1.0f);

          m_viewportRenderer->SubmitMeshSource(DefaultMesh::GetMesh(DefaultMesh::Type::Sphere),
                                               glm::scale(glm::translate(unitMat, position), {0.2, 0.2, 0.2}),
                                               m_jointShader);
        };
        drawJoint(entity, fjc.localAnchorPoint1);
        drawJoint(m_currentScene->GetEntityWithUUID(fjc.connectedEntity), fjc.localAnchorPoint2);
      }
    }
    m_viewportRenderer->EndScene();
    Renderer::DisableStencilPass();
  }
  
  void RendererLayer::ShowGrid(const glm::vec4 &color)
  {
    for (int32_t i = -CamFarView; i < CamFarView; i++)
    {
      if (i == 0)
      {
        Renderer2D::DrawLine({i, 0, -CamFarView}, {i, 0, CamFarView}, {0, 1, 0, 0.6});
        Renderer2D::DrawLine({-CamFarView, 0, i}, {CamFarView, 0, i}, {1, 0, 0, 0.6});
      }
      else
      {
        Renderer2D::DrawLine({i, 0, -CamFarView}, {i, 0, CamFarView}, color);
        Renderer2D::DrawLine({-CamFarView, 0, i}, {CamFarView, 0, i}, color);
      }
    }
  }
  
  void RendererLayer::RenderFixTexts()
  {
    static constexpr glm::vec3 position = { 5.0f, 5.0f, 0.3f };
    static constexpr glm::vec2 size = {0.3f, 0.3f};
    static constexpr glm::vec4 color = { 0.23f, 0.33f, 0.22f, 1.0f};
    
    TextRenderer::BeginBatch(FixedCamera::s_projection);
    TextRenderer::RenderFixedViewText("(c) IKAN", Font::GetDefaultFont(), { m_viewport.width - 80, 5.0f, 0.3f },
                                      size, color);
    TextRenderer::RenderFixedViewText(std::to_string((uint32_t)(ImGui::GetIO().Framerate)), Font::GetDefaultFont(),
                                      position, size, color);
    TextRenderer::EndBatch();
  }

  void RendererLayer::CreateProject(const std::filesystem::path &projectDir)
  {
    IK_LOG_TRACE("Kreator Layer", "Creating Project at {0} ", projectDir.string().c_str());

    // Close the current Project
    if (Project::GetActive())
    {
      CloseProject();
    }

    // Copy the template files
    Utils::FileSystem::Copy(m_templateProjectDir, projectDir);

    // Open Template Project file
    std::ifstream stream(projectDir / "TemplateProject.ikproj");
    std::stringstream ss;
    ss << stream.rdbuf();
    stream.close();
    
    // Rename the Project name in file
    std::string str = ss.str();
    KreatorUtils::ReplaceProjectName(str, m_projectNameBuffer);
    
    // Open Projecy file again
    std::ofstream ostream(projectDir / "TemplateProject.ikproj");
    ostream << str;
    ostream.close();
    
    // Rename the file name
    std::string newProjectFileName = std::string(m_projectNameBuffer) + ProjectExtension;
    Utils::FileSystem::Rename(projectDir / "TemplateProject.ikproj", projectDir / newProjectFileName);
    
    // Create Required Direcotries
    Utils::FileSystem::CreateDirectory(projectDir / "Assets" / "Textures");
    Utils::FileSystem::CreateDirectory(projectDir / "Assets" / "Fonts");
    Utils::FileSystem::CreateDirectory(projectDir / "Assets" / "Scenes");
    Utils::FileSystem::CreateDirectory(projectDir / "Assets" / "Meshes" / "Source");
    
    auto projectFile = projectDir.string() + "/" + std::string(m_projectNameBuffer) + ProjectExtension;
    OpenProject(projectFile);
  }
  
  void RendererLayer::OpenProject(const std::string &filepath)
  {
    IK_LOG_TRACE("Kreator Layer", "Opening Project {0}", filepath.c_str());
    
    // Check if File exists
    if (!Utils::FileSystem::Exists(filepath))
    {
      IK_LOG_ERROR("Kreator Layer", "Tried to open a project that doesn't exist. Project path: {0}", filepath);
      
      // TODO: Remove Assert after testing. may be this will never hit just precaution check
      IK_ASSERT(false);
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
    
    m_panels.OnProjectChanged(project);
    
    // Reset cameras
    m_editorCamera = EditorCamera(45.0f, 1280.0f, 720.0f, 0.1f, CamFarView);
    
    if (!project->GetConfig().startScene.empty())
    {
      OpenScene((Project::GetAssetDirectory() / project->GetConfig().startScene).string());
    }
    else
    {
      NewScene();
    }

    PushProjectToRecentProjects(filepath);
    
    // Initialize Default mesh
    DefaultMesh::Initialize();
  }
  
  void RendererLayer::PushProjectToRecentProjects(std::filesystem::path projectFile)
  {
    RecentProject projectEntry;
    projectEntry.name = Utils::String::RemoveExtension(projectFile.filename().string());
    projectEntry.filePath = projectFile;
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
  
  void RendererLayer::OpenProject()
  {
    if (m_openProjectPath == "")
    {
      return;
    }
    
    // stash the filepath away.  Actual opening of project is deferred until it is "safe" to do so.
    m_projectFilePathBuffer.StrCpy(m_openProjectPath.string());
    
    PushProjectToRecentProjects(m_openProjectPath);
    OpenProject(m_openProjectPath);
  }
  
  void RendererLayer::CloseProject(bool unloadProject)
  {
    SaveProject();
    
    // Destroy Default mesh
    DefaultMesh::Shutdown();
  }
  
  void RendererLayer::SaveProject()
  {
    if (!Project::GetActive())
    {
      // FIXME: (Kreator) Fix Later
      IK_ASSERT(false);
    }
    
    Ref<Project> project = Project::GetActive();
    ProjectSerializer serializer(project);
    serializer.Serialize(project->GetConfig().projectDirectory + "/" + project->GetConfig().projectFileName);
  }
  
  void RendererLayer::NewScene(const std::string& name)
  {
    m_editorScene =  Scene::Create(name);
    m_sceneFilePath = std::string();
    
    m_editorCamera = EditorCamera(45.0f, 1280.0f, 720.0f, 0.1f, CamFarView);
    m_currentScene = m_editorScene;
    
    m_panels.SetSceneContext(m_editorScene);
    UpdateWindowTitle(name);
  }
  
  void RendererLayer::OpenScene(const std::string& filepath)
  {
    if (!Utils::FileSystem::Exists(filepath))
    {
      IK_LOG_ERROR("Kreator Layer" ,"Tried loading a non-existing scene: {0}", filepath);
      NewScene(Utils::String::GetFileNameFromPath(filepath));
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
    
    std::filesystem::path path = filepath;
    UpdateWindowTitle(path.filename().string());
    
    m_panels.SetSceneContext(m_currentScene);
  }
  
  void RendererLayer::OpenScene()
  {
    FolderExplorer::OpenPopup("Open Scene", Project::GetActive()->GetAssetDirectory() / "Scenes");
    m_folderExplorerAction = FolderExplorerAction::OpenScene;
  }
  
  void RendererLayer::SaveSceneAs()
  {
    FolderExplorer::SavePopup("Save Scene", Project::GetActive()->GetAssetDirectory() / "Scenes");
    m_folderExplorerAction = FolderExplorerAction::SaveScene;
  }
  
  void RendererLayer::SaveScene()
  {
    if (!m_sceneFilePath.empty())
    {
      SceneSerializer serializer(m_editorScene);
      serializer.Serialize(m_sceneFilePath);
      
      m_timeSinceLastSave = 0.0f;
    }
    else
    {
      SaveSceneAs();
    }
  }
  void RendererLayer::SaveSceneAuto()
  {
    if (!m_sceneFilePath.empty())
    {
      SceneSerializer serializer(m_editorScene);
      serializer.Serialize(m_sceneFilePath + ".auto");
      m_timeSinceLastSave = 0.0f;
    }
  }

  void RendererLayer::OnScenePlay()
  {
    ClearSelectedEntity();
    
    m_sceneState = SceneState::Play;
    
    m_panels.GetPanel<EditorConsolePanel>(CONSOLE_PANEL_ID)->OnScenePlay();
    
    m_runtimeScene = CreateRef<Scene>();
    m_editorScene->CopyTo(m_runtimeScene);
    
    m_runtimeScene->OnRuntimeStart();
    m_currentScene = m_runtimeScene;
    m_panels.SetSceneContext(m_runtimeScene);
    
#ifdef DEBUG
    m_panels.GetPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSceneContext(m_runtimeScene);
#endif
  }
  
  void RendererLayer::OnSceneStop()
  {
    ClearSelectedEntity();
    
    m_runtimeScene->OnRuntimeStop();
    m_sceneState = SceneState::Edit;
    
    // Unload runtime scene
    m_runtimeScene = nullptr;
    
    m_currentScene = m_editorScene;
    m_panels.SetSceneContext(m_editorScene);
    m_panels.GetPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSceneContext(m_editorScene);
  }
  
  void RendererLayer::OnScenePause()
  {
    
  }
  
  void RendererLayer::OnSceneResume()
  {
    
  }
  
  void RendererLayer::OnSceneStartSimulation()
  {
    ClearSelectedEntity();
    
    m_sceneState = SceneState::Simulate;
    
    m_simulationScene = CreateRef<Scene>();
    m_editorScene->CopyTo(m_simulationScene);
    
    m_simulationScene->OnSimulationStart();
    m_currentScene = m_simulationScene;
    m_panels.SetSceneContext(m_simulationScene);
    m_panels.GetPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSceneContext(m_simulationScene);
  }
  
  void RendererLayer::OnSceneStopSimulation()
  {
    ClearSelectedEntity();
    
    m_simulationScene->OnSimulationStop();
    m_sceneState = SceneState::Edit;
    
    m_simulationScene = nullptr;
    
    m_currentScene = m_editorScene;
    m_panels.SetSceneContext(m_editorScene);
    m_panels.GetPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSceneContext(m_editorScene);
  }
  
  bool RendererLayer::IsEntitySelected(Entity entity) const
  {
    for (const auto& selectedContext : m_selectionContext)
    {
      if (selectedContext.entity == entity)
      {
        return true;
      }
    }
    return false;
  }

  void RendererLayer::OnEntitySelected(SelectionContext entities)
  {
    if (entities.Size() == 0)
    {
      return;
    }
    
    m_selectionContext.clear();
    for (const auto& entity : entities)
    {
      SelectedSubmesh selection;
      selection.entity = entity;
      
      m_selectionContext.push_back(selection);
    }
    
    if (m_currentScene != m_runtimeScene)
    {
      m_currentScene->SetSelectedEntity(entities.At(0));
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
      m_currentScene->SetSelectedEntity(entt::null);
    }
    
    m_selectionContext.clear();
  }
  
  void RendererLayer::SetSelectedEntity(Entity entity)
  {
    m_panels.GetPanel<SceneHierarchyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSelectedEntity(entity);
  }
  
  // UI APIS ---------------------------------------------------------------------------------------------------------
  void RendererLayer::UI_StartMainWindowDocking()
  {
    static bool optFullscreenPersistant = true;
    static ImGuiDockNodeFlags optFlags = ImGuiDockNodeFlags_None;
    bool optFullscreen = optFullscreenPersistant;
    
    // ImGui Input Output Fonts
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    
    io.ConfigWindowsResizeFromEdges = io.BackendFlags & ImGuiBackendFlags_HasMouseCursors;
    
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags windowFlags = /* ImGuiWindowFlags_MenuBar |*/ ImGuiWindowFlags_NoDocking;
    if (optFullscreen)
    {
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
      windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    
#if 0
    // When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle
    // the pass-thru hole, so we ask Begin() to not render a background.
    if (optFlags & ImGuiDockNodeFlags_PassthruCentralNode)
    {
      windowFlags |= ImGuiWindowFlags_NoBackground;
    }
#endif
    
    const Window& window = Application::Get().GetWindow();
    bool isMaximized = window.IsMaximized();
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(3.0f, 3.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
    ImGui::Begin("DockSpace Demo", nullptr, windowFlags);
    ImGui::PopStyleColor(); // MenuBarBg
    ImGui::PopStyleVar(2);
    
    if (optFullscreen)
    {
      ImGui::PopStyleVar(2);
    }
    
    if (m_sceneState != SceneState::Play)
    {
      // Render the title if original title bar is hidden
      if (Application::Get().GetSpecification().windowSpecification.hideTitleBar)
      {
        float titlebarHeight = UI_DrawTitlebar();
        UI::SetCursorPosY(titlebarHeight + ImGui::GetCurrentWindow()->WindowPadding.y);
      }
    }
    
    // Dockspace
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 250.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
      ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), optFlags);
    }
    style.WindowMinSize.x = minWinSizeX;
  }
  
  void RendererLayer::UI_EndMainWindowDocking()
  {
    ImGui::End();
  }
  
  void RendererLayer::UI_Viewport()
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
    
    m_viewport.panelMouseHover = ImGui::IsWindowHovered();
    m_viewport.panelFocused = ImGui::IsWindowFocused();
    
    auto viewportOffset = ImGui::GetCursorPos(); // includes tab bar
    auto viewportSize = ImGui::GetContentRegionAvail();
    
    // Updating the View port size
    m_viewport.width = viewportSize.x;
    m_viewport.height = viewportSize.y;
    
    // Set Viewport of Kreator Data
    UpdateViewportSize();
    
    // Render viewport image
    UI::Image(m_viewportRenderer->GetFinalImage(), viewportSize);
    
    if (m_sceneState != SceneState::Play)
    {
      UI_SceneToolbar();
      UI_GuizmoToolbar();
      UI_UpdateGuizmo();
    }
    
    auto windowSize = ImGui::GetWindowSize();
    ImVec2 minBound = ImGui::GetWindowPos();
    minBound.x += viewportOffset.x;
    minBound.y += viewportOffset.y;
    
    ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
    m_viewport.bounds[0] = { minBound.x, minBound.y };
    m_viewport.bounds[1] = { maxBound.x, maxBound.y };
    
    m_allowViewportCameraEvents = ImGui::IsMouseHoveringRect(minBound, maxBound);
    
    ImGui::End();
    ImGui::PopStyleVar();
  }
  
  float RendererLayer::UI_DrawTitlebar()
  {
    float titlebarHeight = 40.0f;
    
    const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;
    
    // Draw the title Bar rectangle ---------------------------------------------------
    const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
    const ImVec2 titlebarMax =
    {
      ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth(),
      ImGui::GetCursorScreenPos().y + titlebarHeight
    };
    
    UI::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y));
    auto* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(titlebarMin, titlebarMax, UI::Theme::Color::Titlebar);
    
    // Drag and Control the window with user title bar ---------------------------------
    UI::SetCursorPos(windowPadding);
    UI_TitlebarDragArea(titlebarHeight);
    
    // Draw Kreator Logo ---------------------------------------------------------------
    const int32_t logoWidth = titlebarHeight - 10;
    const int32_t logoHeight = titlebarHeight - 10;
    const ImVec2 logoOffset(windowPadding.x, windowPadding.y);
    const ImVec2 logoRectStart =
    {
      ImGui::GetItemRectMin().x + logoOffset.x,
      ImGui::GetItemRectMin().y + logoOffset.y
    };
    const ImVec2 logoRectMax =
    {
      logoRectStart.x + logoWidth,
      logoRectStart.y + logoHeight
    };
    drawList->AddImage(UI::GetTextureID(m_applicationIcon), logoRectStart, logoRectMax);
    
    // Draw the Menu Tab in Title bar --------------------------------------------------
    ImGui::SetItemAllowOverlap();
    
    const float logoOffsetX = 6.0f * 2.0f + 41.0f + windowPadding.x;
    UI::SetCursorPos(ImVec2(logoOffsetX, 4.0f));
    
    UI_MenuBar();
    
    // Round Bar -------------------------------------------------------------------------
    UI::SetCursorPosX(ImGui::GetWindowWidth() / 4);
    UI::SetCursorPosY(titlebarHeight / 3);
    
    const ImVec2 roundBarMin = ImGui::GetCursorScreenPos();
    const ImVec2 roundBarMax =
    {
      ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() / 2,
      ImGui::GetCursorScreenPos().y + 23
    };
    drawList->AddRectFilled(roundBarMin, roundBarMax, Kreator_UI::Color::TitleBarDark, 10);
    float roundBarRight = (3 * ImGui::GetWindowWidth()) / 4;
    UI::ShiftCursorY(5);

    const float underlineThickness = 1.0f;
    // Project name --------------------------------------------------------------------
    {
      UI::ScopedColor textColor(ImGuiCol_Text, Kreator_UI::Color::TextDarker);
      UI::ShiftCursorX(10);
      const std::string title = Project::GetActive()->GetConfig().name;
      {
        UI::ScopedFont boldFont(Kreator_UI::GetBoldFont());
        ImGui::Text(title.c_str());
      }
      UI::SetTooltip("Current project (" + Project::GetActive()->GetConfig().projectFileName + ")");
      
      // Get the Project Name rectangle (Expanded by 10 to have good space)
      ImRect itemRect = UI::RectExpanded(UI::GetItemRect(), 10.0f, 2.0f);
      // Make the Max + thickness of rectange as min of verticle line
      itemRect.Min.x = itemRect.Max.x + underlineThickness;
      // Make the new Min + Thickness of rectange as new Max of verticle line
      itemRect.Max.x = itemRect.Min.x + underlineThickness;
      
      drawList->AddRectFilled(itemRect.Min, itemRect.Max, UI::Theme::Color::Muted, 2.0f);
    }

    // Current Scene name ---------------------------------------------------------------
    {
      UI::ScopedColor textColor(ImGuiCol_Text, UI::Theme::Color::Text);
      UI::SameLine();
      
      const std::string sceneName = m_currentScene->GetName();
      UI::SetCursorPosX(roundBarRight - ImGui::CalcTextSize(sceneName.c_str()).x - 20);
      {
        if (m_sceneFilePath == "")
        {
          ImGui::PushStyleColor(ImGuiCol_Text, Kreator_UI::Color::Warning);
        }
        UI::ScopedFont boldFont(Kreator_UI::GetBoldFont());
        ImGui::Text(sceneName.c_str());
        if (m_sceneFilePath == "")
        {
          ImGui::PopStyleColor();
        }
      }
      if (m_sceneFilePath == "")
      {
        UI::SetTooltip("Unsaved Scene");
      }
      else
      {
        UI::SetTooltip("Current scene (" + m_sceneFilePath + ")");
      }
      
      // Get the Project Name rectangle (Expanded by 10 to have good space)
      ImRect itemRect = UI::RectExpanded(UI::GetItemRect(), 10.0f, 2.0f);
      
      // Make the new Min + Thickness of rectange as new Max of verticle line
      itemRect.Max.x = itemRect.Min.x + underlineThickness;
      
      drawList->AddRectFilled(itemRect.Min, itemRect.Max, UI::Theme::Color::Muted, 2.0f);
    }
    
    // Render the Window Buttons -------------------------------------------------------
    UI::SetCursorPosX(ImGui::GetWindowWidth() - 78);
    UI::SetCursorPosY(20.0f);
    UI_WindowButtons();
    
    return titlebarHeight;
  }
  
  void RendererLayer::UI_MenuBar()
  {
    // Menu Bar Rectactangle Size
    const ImRect menuBarRect =
    {
      ImGui::GetCursorPos(), // Min Rect Coord
      {ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeightWithSpacing()} // Max Rect Coord
    };
    
    ImGui::BeginGroup();
    if (UI::BeginMenuBar(menuBarRect))
    {
      bool menuOpen = ImGui::IsPopupOpen("##menubar", ImGuiPopupFlags_AnyPopupId);
      
      // Push the Colors if Menu is active
      if (menuOpen)
      {
        const ImU32 colActive = UI::ColorWithSaturation(Kreator_UI::Color::Accent, 0.5f);
        ImGui::PushStyleColor(ImGuiCol_Header, colActive);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, colActive);
      }
      
      // Function to Pop the highlight Color
      auto popItemHighlight = [&menuOpen]
      {
        if (menuOpen)
        {
          ImGui::PopStyleColor(2);
          menuOpen = false;
        }
      };
      
      // Menu Items
      UI_Utils::AddMenu("File", popItemHighlight, [this]() {
        if (ImGui::MenuItem("Create Project...", "Cmd + Shift + N"))
        {
          m_showCreateNewProjectPopup = true;
        }
        if (ImGui::MenuItem("Open Project...", "Cmd + Shift + O"))
        {
          FolderExplorer::OpenPopup("Open Project", m_allProjectsPath);
          m_folderExplorerAction = FolderExplorerAction::OpenProject;
        }
        if (ImGui::BeginMenu("Open Recent"))
        {
          size_t i = 0;
          for (auto it = m_userPreferences->recentProjects.begin(); it != m_userPreferences->recentProjects.end(); it++)
          {
            if (i > 10)
            {
              break;
            }
            
            if (Utils::FileSystem::Exists(it->second.filePath))
            {
              if (ImGui::MenuItem(it->second.name.c_str()))
              {
                // stash filepath away and defer actual opening of project until it is "safe" to do so
                m_projectFilePathBuffer.StrCpy(it->second.filePath.data());
                
                RecentProject projectEntry;
                projectEntry.name = it->second.name;
                projectEntry.filePath = it->second.filePath;
                projectEntry.lastOpened = time(NULL);
                
                it = m_userPreferences->recentProjects.erase(it);
                
                m_userPreferences->recentProjects[projectEntry.lastOpened] = projectEntry;
                
                UserPreferencesSerializer preferencesSerializer(m_userPreferences);
                preferencesSerializer.Serialize(m_userPreferences->filePath);
                
                OpenProject(projectEntry.filePath);
                break;
              }
            }
            else
            {
              m_userPreferences->recentProjects.erase(it);
              UserPreferencesSerializer serializer(m_userPreferences);
              serializer.Serialize(m_userPreferences->filePath);
              break;
            }
            
            i++;
          }
          ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::MenuItem("New Scene", "Cmd+N"))
        {
          m_showNewScenePopup = true;
        }
        if (ImGui::MenuItem("Open Scene", "Cmd+O"))
        {
          OpenScene();
        }
        if (ImGui::MenuItem("Save Scene", "Cmd+S"))
        {
          SaveScene();
        }
        if (ImGui::MenuItem("Save Scene As", "Cmd+Shift+S"))
        {
          SaveSceneAs();
        }
        
        ImGui::Separator();
        if (ImGui::MenuItem("Exit", "Cmd + Q"))
        {
          Application::Get().Close();
        }
      });
      
      UI_Utils::AddMenu("Edit", popItemHighlight, [this]() {
        
      });
      
      UI_Utils::AddMenu("View", popItemHighlight, [this]() {
        for (auto& [id, panelData] : m_panels.GetPanels())
        {
          ImGui::MenuItem(panelData.name, nullptr, &panelData.isOpen);
        }
        ImGui::Separator();
        ImGui::MenuItem("Editor Panel", nullptr, &m_showEditorPanel);
        ImGui::MenuItem("Statistic Panel", nullptr, &m_showStatisticsPanel);
      });
      
      UI_Utils::AddMenu("Debug", popItemHighlight, [this]() {
        if (ImGui::MenuItem("Show Wlecome Screen", nullptr, m_userPreferences->showWelcomeScreen))
        {
          m_userPreferences->showWelcomeScreen = m_userPreferences->showWelcomeScreen ? false : true;
          UserPreferencesSerializer serializer(m_userPreferences);
          serializer.Serialize(m_userPreferences->filePath);
        }
      });
      
      UI_Utils::AddMenu("Help", popItemHighlight, [this]() {
        if (ImGui::MenuItem("About"))
        {
          m_showAboutPopup = true;
        }
      });
      
      if (menuOpen)
      {
        ImGui::PopStyleColor(2);
      }
    }
    UI::EndMenuBar();
    ImGui::EndGroup();
  }
  
  void RendererLayer::UI_WindowButtons()
  {
    // Window buttons
    const ImU32 buttonColN = UI::ColorWithMultipliedValue(UI::Theme::Color::Text, 0.9f);
    const ImU32 buttonColH = UI::ColorWithMultipliedValue(UI::Theme::Color::Text, 1.2f);
    const ImU32 buttonColP = Kreator_UI::Color::TextDarker;
    const float buttonWidth = 14.0f;
    const float buttonHeight = 14.0f;
    const Window& window = Application::Get().GetWindow();
    
    // Minimize Button
    {
      const int iconHeight = m_iconMinimize->GetHeight();
      const float padY = (buttonHeight - (float)iconHeight) / 2.0f;
      
      if (ImGui::InvisibleButton("Minimize", ImVec2(buttonWidth, buttonHeight), ImGuiButtonFlags_AllowItemOverlap))
      {
        window.Iconify();
      }
      UI::DrawButtonImage(m_iconMinimize, buttonColN, buttonColH, buttonColP, UI::RectExpanded(UI::GetItemRect(), 0.0f, -padY));
    }
    
    UI::SameLine();
    // Maximize Button
    {
      bool isMaximized = window.IsMaximized();
      if (ImGui::InvisibleButton("Maximize/Restore", ImVec2(buttonWidth, buttonHeight), ImGuiButtonFlags_AllowItemOverlap))
      {
        (isMaximized) ? window.Restore() : window.Maximize();
      }
      UI::DrawButtonImage(isMaximized ? m_iconRestore : m_iconMaximize, buttonColN, buttonColH, buttonColP);
    }
    
    UI::SameLine();
    // Close Button
    {
      if (ImGui::InvisibleButton("Close", ImVec2(buttonWidth, buttonHeight), ImGuiButtonFlags_AllowItemOverlap))
      {
        Application::Get().Close();
      }
      UI::DrawButtonImage(m_iconClose, UI::Theme::Color::Text, UI::ColorWithMultipliedValue(UI::Theme::Color::Text, 1.4f), buttonColP);
    }
  }
  void RendererLayer::UI_TitlebarDragArea(float titlebarHeight)
  {
    static float moveOffsetX;
    static float moveOffsetY;
    const float titleBarWidth = ImGui::GetContentRegionAvail().x;
    
    auto* rootWindow = ImGui::GetCurrentWindow()->RootWindow;
    const float windowWidth = (int32_t)rootWindow->RootWindow->Size.x;
    
    if (ImGui::InvisibleButton("##titleBarDragZone", ImVec2(titleBarWidth, titlebarHeight * 2),
                               ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_AllowItemOverlap))
    {
      ImVec2 point = ImGui::GetMousePos();
      ImRect rect = rootWindow->Rect();
      
      // Calculate the difference between the cursor pos and window pos
      moveOffsetX = point.x - rect.Min.x;
      moveOffsetY = point.y - rect.Min.y;
    }
    
    const Window& window = Application::Get().GetWindow();
    bool maximized = window.IsMaximized();
    
    // Maximize or restore on doublt click
    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) and ImGui::IsItemHovered())
    {
      (maximized) ? window.Restore() : window.Maximize();
    }
    else if (ImGui::IsItemActive())
    {
      if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
      {
        if (maximized)
        {
          window.Restore();
          
          // Get the original size of window
          [[maybe_unused]] int32_t newWidth = window.GetWidth();
          [[maybe_unused]] int32_t newHeight = window.GetHeight();
          
          // Offset position proportionally to mouse position on titlebar
          // This ensures we dragging window relatively to cursor position on titlebar
          // correctly when window size changes
          if (windowWidth - (float)newWidth > 0.0f)
          {
            moveOffsetX *= (float)newWidth / windowWidth;
          }
        }
        
        // Update the new position of window
        ImVec2 point = ImGui::GetMousePos();
        window.SetPosition({point.x - moveOffsetX, point.y - moveOffsetY});
      }
    }
  }
  
  void RendererLayer::UI_NewScene()
  {
    static char s_newSceneName[128];
    if (m_showNewScenePopup)
    {
      ImGui::OpenPopup("New Scene");
      memset(s_newSceneName, 0, 128);
      m_showNewScenePopup = false;
    }
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2{ 400,0 });
    if (ImGui::BeginPopupModal("New Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
      ImGui::BeginVertical("NewSceneVertical");
      ImGui::SetNextItemWidth(-1);
      ImGui::InputTextWithHint("##scene_name_input", "Scene Name", s_newSceneName, 128);
      
      ImGui::BeginHorizontal("NewSceneHorizontal");
      
      UI::ShiftCursorX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("Create").x * 2);
      if (UI::DrawRoundButton("Create", Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 10) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter))
      {
        if ((s_newSceneName[0] != '\0'))
        {
          NewScene(s_newSceneName);
          ImGui::CloseCurrentPopup();
        }
      }
      
      if (UI::DrawRoundButton("Cancel", Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 10) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape))
      {
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndHorizontal();
      ImGui::EndVertical();
      ImGui::EndPopup();
    }
  }
  
  void RendererLayer::UI_SceneToolbar()
  {
    UI::ScopedStyle disableSpacing(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    UI::ScopedStyle disableWindowBorder(ImGuiStyleVar_WindowBorderSize, 0.0f);
    UI::ScopedStyle windowRounding(ImGuiStyleVar_WindowRounding, 4.0f);
    UI::ScopedStyle disablePadding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    auto viewportStart = ImGui::GetItemRectMin();
    
    const float buttonSize = 18.0f;
    const float edgeOffset = 4.0f;
    const float windowHeight = 32.0f; // annoying limitation of ImGui, window can't be smaller than 32 pixels
    const float numberOfButtons = 4.0f;
    const float windowWidth = edgeOffset * 6.0f + buttonSize * numberOfButtons + edgeOffset * (numberOfButtons - 1.0f) * 2.0f;
    
    ImGui::SetNextWindowPos(ImVec2(viewportStart.x + m_viewport.width / 2, viewportStart.y + edgeOffset));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("##scene_tool", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking);
    
    // A hack to make icon panel appear smaller than minimum allowed by ImGui size
    // Filling the background for the desired 26px height
    const float desiredHeight = 26.0f;
    ImRect background = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), 0.0f, -(windowHeight - desiredHeight) / 2.0f);
    ImGui::GetWindowDrawList()->AddRectFilled(background.Min, background.Max, IM_COL32(15, 15, 15, 127), 4.0f);
    
    ImGui::BeginVertical("##scene_tool_V", ImGui::GetContentRegionAvail());
    ImGui::Spring();
    ImGui::BeginHorizontal("##scene_tool_H", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });
    ImGui::Spring();
    {
      UI::ScopedStyle enableSpacing(ImGuiStyleVar_ItemSpacing, ImVec2(edgeOffset * 2.0f, 0));
      
      auto playbackButton = [](const Ref<Image>& icon, const ImColor& tint)
      {
        const float size = std::min((float)icon->GetHeight(), ImGui::GetWindowHeight() - 4.0f);
        const float iconPadding = 0.0f;
        const bool clicked = ImGui::InvisibleButton(UI::GenerateID(), ImVec2(size, size));
        UI::DrawButtonImage(icon, tint,
                            UI::ColorWithMultipliedValue(tint, 1.3f),
                            UI::ColorWithMultipliedValue(tint, 0.8f),
                            UI::RectExpanded(UI::GetItemRect(), -iconPadding, -iconPadding));
        
        return clicked;
      };
      
      // white buttons
      const ImColor buttonTint = IM_COL32(192, 192, 192, 255);
            
      if (playbackButton(m_playButtonTex, buttonTint))
      {
        OnScenePlay();
      }
      
      const ImColor tint = m_sceneState == SceneState::Simulate ? ImColor(1.0f, 0.75f, 0.75f, 1.0f) : buttonTint;
      if (playbackButton(m_simulateButtonTex, tint))
      {
        if (m_sceneState == SceneState::Edit)
        {
          OnSceneStartSimulation();
        }
        else if (m_sceneState == SceneState::Simulate)
        {
          OnSceneStopSimulation();
        }
      }
      
      if (playbackButton(m_pauseButtonTex, buttonTint))
      {
        if (m_sceneState == SceneState::Simulate)
        {
          OnScenePause();
        }
        else if (m_sceneState == SceneState::Pause)
        {
          OnSceneResume();
        }
      }
    }
    ImGui::Spring();
    ImGui::EndHorizontal();
    ImGui::Spring();
    ImGui::EndVertical();
    
    ImGui::End();
  }
  
  void RendererLayer::UI_GuizmoToolbar()
  {
    UI::ScopedStyle disableSpacing(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    UI::ScopedStyle disableWindowBorder(ImGuiStyleVar_WindowBorderSize, 0.0f);
    UI::ScopedStyle windowRounding(ImGuiStyleVar_WindowRounding, 4.0f);
    UI::ScopedStyle disablePadding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    auto viewportStart = ImGui::GetItemRectMin();
    
    const float buttonSize = 18.0f;
    const float edgeOffset = 4.0f;
    const float windowHeight = 32.0f; // annoying limitation of ImGui, window can't be smaller than 32 pixels
    const float numberOfButtons = 4.0f;
    const float windowWidth = edgeOffset * 6.0f + buttonSize * numberOfButtons + edgeOffset * (numberOfButtons - 1.0f) * 2.0f;
    
    ImGui::SetNextWindowPos(ImVec2(viewportStart.x + 14, viewportStart.y + edgeOffset));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
    ImGui::SetNextWindowBgAlpha(0.0f);
    
    ImGui::Begin("##viewport_tools", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking);
    
    // A hack to make icon panel appear smaller than minimum allowed by ImGui size
    // Filling the background for the desired 26px height
    const float desiredHeight = 26.0f;
    ImRect background = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), 0.0f, -(windowHeight - desiredHeight) / 2.0f);
    ImGui::GetWindowDrawList()->AddRectFilled(background.Min, background.Max, IM_COL32(15, 15, 15, 127), 4.0f);
    
    ImGui::BeginVertical("##gizmosV", ImGui::GetContentRegionAvail());
    ImGui::Spring();
    ImGui::BeginHorizontal("##gizmosH", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });
    ImGui::Spring();
    
    {
      UI::ScopedStyle enableSpacing(ImGuiStyleVar_ItemSpacing, ImVec2(edgeOffset * 2.0f, 0));
      
      const ImColor c_SelectedGizmoButtonColor = Kreator_UI::Color::Accent;
      const ImColor c_UnselectedGizmoButtonColor = UI::Theme::Color::TextBrighter;
      
      auto gizmoButton = [&c_SelectedGizmoButtonColor, buttonSize, this](const Ref<Image>& icon,
                                                                   const ImColor& tint, float paddingY = 0.0f)
      {
        const float height = std::min((float)icon->GetHeight(), buttonSize) - paddingY * 2.0f;
        const float width = (float)icon->GetWidth() / (float)icon->GetHeight() * height;
        const bool clicked = ImGui::InvisibleButton(UI::GenerateID(), ImVec2(width, height));
        UI::DrawButtonImage(icon, tint, tint, tint, UI::RectOffset(UI::GetItemRect(), 0.0f, paddingY));
        m_hoveredGuizmotoolbar = ImGui::IsItemHovered();
        return clicked;
      };
      
      ImColor buttonTint = m_gizmoType == -1 ? c_SelectedGizmoButtonColor : c_UnselectedGizmoButtonColor;
      if (gizmoButton(m_selectToolTex, buttonTint, m_gizmoType != -1))
      {
        m_gizmoType = -1;
      }
      
      buttonTint = m_gizmoType == ImGuizmo::OPERATION::TRANSLATE ?
      c_SelectedGizmoButtonColor :
      c_UnselectedGizmoButtonColor;
      if (gizmoButton(m_moveToolTex, buttonTint))
      {
        m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
      }
      
      buttonTint = m_gizmoType == ImGuizmo::OPERATION::ROTATE ?
      c_SelectedGizmoButtonColor :
      c_UnselectedGizmoButtonColor;
      if (gizmoButton(m_rotateToolTex, buttonTint))
      {
        m_gizmoType = ImGuizmo::OPERATION::ROTATE;
      }
      
      buttonTint = m_gizmoType == ImGuizmo::OPERATION::SCALE ?
      c_SelectedGizmoButtonColor :
      c_UnselectedGizmoButtonColor;
      if (gizmoButton(m_scaleToolTex, buttonTint))
      {
        m_gizmoType = ImGuizmo::OPERATION::SCALE;
      }
    }
    
    ImGui::Spring();
    ImGui::EndHorizontal();
    ImGui::Spring();
    ImGui::EndVertical();
       
    ImGui::End();
  }
  
  void RendererLayer::UI_UpdateGuizmo()
  {
    if (Input::IsKeyPressed(IKan::Key::LeftControl) or m_selectionContext.size() > 1)
    {
      return;
    }
    if (m_gizmoType != -1 and m_selectionContext.size() and m_currentScene != m_runtimeScene)
    {
      float rw = (float)ImGui::GetWindowWidth();
      float rh = (float)ImGui::GetWindowHeight();
      ImGuizmo::SetOrthographic(false);
      ImGuizmo::SetDrawlist();
      ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);
      
      bool snap = Input::IsKeyPressed(Key::LeftControl);
      float snapValue = GetSnapValue();
      float snapValues[3] = { snapValue, snapValue, snapValue };
      
      auto& selection = m_selectionContext[0];
      TransformComponent& entityTransform = selection.entity.Transform();
#ifdef WorldSpace
      glm::mat4 transform = m_currentScene->GetWorldSpaceTransformMatrix(selection.entity);
#else
      glm::mat4 transform = entityTransform.Transform();
#endif
      if (Input::IsKeyPressed(Key::LeftAlt))
      {
        m_gizmoMode = 1;
      }
      else
      {
        m_gizmoMode = 0;
      }
      ImGuizmo::Manipulate(glm::value_ptr(m_editorCamera.GetViewMatrix()),
                           glm::value_ptr(m_editorCamera.GetUnReversedProjectionMatrix()),
                           (ImGuizmo::OPERATION)m_gizmoType,
                           (ImGuizmo::MODE)m_gizmoMode,
                           glm::value_ptr(transform),
                           nullptr,
                           snap ? snapValues : nullptr);
      
      if (ImGuizmo::IsUsing())
      {
#ifdef WorldSpace
        Entity parent = m_currentScene->TryGetEntityWithUUID(selection.entity.GetParentUUID());
        if (parent)
        {
          glm::mat4 parentTransform = m_currentScene->GetWorldSpaceTransformMatrix(parent);
          transform = glm::inverse(parentTransform) * transform;
          
          glm::vec3 translation, rotation, scale;
          Utils::Math::DecomposeTransform(transform, translation, rotation, scale);
          
          glm::vec3 deltaRotation = rotation - entityTransform.Rotation();
          entityTransform.UpdatePosition(translation);
          entityTransform.UpdateRotation(entityTransform.Rotation() + deltaRotation);
          entityTransform.UpdateScale(scale);
        }
        else
#endif
        {
          glm::vec3 translation, rotation, scale;
          Utils::Math::DecomposeTransform(transform, translation, rotation, scale);
          
          glm::vec3 deltaRotation = rotation - entityTransform.Rotation();
          entityTransform.UpdatePosition(translation);
          entityTransform.UpdateRotation(entityTransform.Rotation() + deltaRotation);
          entityTransform.UpdateScale(scale);
        }
      }
    }
  }
  
  void RendererLayer::UI_AboutPopup()
  {
    auto boldFont = Kreator_UI::GetSemiHeaderFont();
    auto largeFont = Kreator_UI::GetHugeHeaderFont();
    
    if (m_showAboutPopup)
    {
      ImGui::OpenPopup("About##AboutPopup");
      m_showAboutPopup = false;
    }
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2{ 600,0 });
    if (ImGui::BeginPopupModal("About##AboutPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
    {
      ImGui::PushFont(largeFont);
      ImGui::Text("IKan-Kreate");
      ImGui::PopFont();
      
      ImGui::Separator();
      ImGui::TextWrapped("IKan-Kreate is a revolutionary game engine designed to empower game developers on the Mac OS");
      
      auto cap = Renderer::Capabilities::Get();
      ImGui::TextWrapped("Vendor %s", cap.vendor.c_str());
      ImGui::TextWrapped("Renderer %s", cap.renderer.c_str());
      ImGui::TextWrapped("Version %s", cap.version.c_str());
      
      ImGui::Separator();
      ImGui::PushFont(boldFont);
      ImGui::Text("IKan Core Team");
      ImGui::PopFont();
      ImGui::Text("Ashish");
      ImGui::Separator();
      
      if ((ImGui::Button("OK")) or (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter)))
      {
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }
  }
  
  void RendererLayer::UI_StatisticsPanel()
  {
    UI::ScopedFont sameWidth(Kreator_UI::GetFixedWidthFont());
    if (!m_showStatisticsPanel)
    {
      return;
    }
    
    if (ImGui::Begin("Statistics"))
    {
      UI::ScopedColor windowBg (ImGuiCol_WindowBg, UI::Theme::Color::BackgroundDark);
      UI::ScopedColor childBg (ImGuiCol_ChildBg, UI::Theme::Color::BackgroundDark);
      
      ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
      if (ImGui::BeginTabBar("StatisticsTabs", tabBarFlags))
      {
        Application& app = Application::Get();
        
        if (ImGui::BeginTabItem("Renderer"))
        {
          auto& caps = Renderer::Capabilities::Get();
          ImGui::Text("Vendor     : %s", caps.vendor.c_str());
          ImGui::Text("Renderer   : %s", caps.renderer.c_str());
          ImGui::Text("Version    : %s", caps.version.c_str());
          
          ImGui::Separator();
          ImGui::Text("Frame Time : %.2fms\n", app.GetTimestep().MilliSeconds());
          ImGui::Text("FPS        : %.2fms\n", ImGui::GetIO().Framerate);
          
          ImGui::Separator();
          ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Performance"))
        {
          ImGui::Text("Frame Time : %.2fms\n", app.GetTimestep().MilliSeconds());
          ImGui::Text("FPS        : %.2fms\n", app.GetTimestep().FPS());
          ImGui::Separator();
          
          const auto& perFrameData = PerformanceProfiler::Get()->GetPerFrameData();
          for (auto&& [name, time] : perFrameData)
          {
            ImGui::Text("%s: %.3fms\n", name, time);
          }
          ImGui::Separator();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Memory"))
        {
#if IK_TRACK_MEMORY
          const auto& allocStats = Memory::GetAllocationStats();
          const auto& allocStatsMap = Allocator::GetAllocationStats();
          
          {
            std::string totalAllocatedStr = StringUtils::BytesToString(allocStats.TotalAllocated);
            std::string totalFreedStr = StringUtils::BytesToString(allocStats.TotalFreed);
            std::string totalUsedStr = StringUtils::BytesToString(allocStats.TotalAllocated - allocStats.TotalFreed);
            
            ImGui::Text("Total allocated    : %s", totalAllocatedStr.c_str());
            ImGui::Text("Total freed        : %s", totalFreedStr.c_str());
            ImGui::Text("Current usage      : %s", totalUsedStr.c_str());
          }
          
          ImGui::Separator();
          
          static std::string searchedString;
          ImGui::SetNextItemWidth(-1);
          UI::Widgets::SearchWidget(searchedString.data());
          
          struct MemoryRefEntry
          {
            const char* Category;
            size_t Size;
          };
          std::vector<MemoryRefEntry> sortedEntries;
          sortedEntries.reserve(allocStatsMap.size());
          for (auto& [category, stats] : allocStatsMap)
          {
            if (!UI::IsMatchingSearch(category, searchedString))
            {
              continue;
            }
            
            sortedEntries.push_back({ category, stats.TotalAllocated - stats.TotalFreed });
          }
          
          std::sort(sortedEntries.begin(), sortedEntries.end(), [](auto& a, auto& b) { return a.Size > b.Size; });
          
          for (const auto& entry : sortedEntries)
          {
            std::string usageStr = StringUtils::BytesToString(entry.Size);
            
            if (const char* slash = strstr(entry.Category, "/"))
            {
              std::string tag = slash;
              auto lastSlash = tag.find_last_of("/");
              if (lastSlash != std::string::npos)
              {
                tag = tag.substr(lastSlash + 1, tag.size() - lastSlash);
              }
              ImGui::TextColored(ImVec4(0.3f, 0.4f, 0.9f, 1.0f), "%s: %s", tag.c_str(), usageStr.c_str());
            }
            else
            {
              const char* category = entry.Category;
              if (category = strstr(entry.Category, "class"); category)
              {
                category += 6;
              }
              ImGui::Text("%s: %s", category, usageStr.c_str());
              
            }
          }
#else
          ImGui::TextColored(ImVec4(0.9f, 0.35f, 0.3f, 1.0f), "Memory is not being tracked because IK_TRACK_MEMORY is not defined!");
#endif
          ImGui::Separator();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Renderer Stats"))
        {
          const auto& stats = RendererStatistics::Get();
          
          {
            UI::ScopedColor header(ImGuiCol_Text, ImVec4(0.6f, 0.25f, 0.3f, 1.0f));
            UI::ScopedFont boldFont(Kreator_UI::GetBoldFont());
            ImGui::Text("Renderer General Stats");
          }
          ImGui::Text("Draw Calls            : %d", stats.drawCalls);
          ImGui::Text("Vertex Count          : %d", stats.vertexCount);
          ImGui::Text("Vertex Buffer Size    : %d B", stats.vertexBufferSize);
          ImGui::Text("Index Count           : %d B", stats.indexCount);
          ImGui::Text("Index Buffer Size     : %d B", stats.indexBufferSize);
          ImGui::Text("Texture Buffer Size   : %d B", stats.textureBufferSize);
          
          ImGui::Separator();
          {
            UI::ScopedColor header(ImGuiCol_Text, ImVec4(0.6f, 0.25f, 0.3f, 1.0f));
            UI::ScopedFont boldFont(Kreator_UI::GetBoldFont());
            ImGui::Text("Renderer 2D Stats");
          }
          ImGui::Text("Quads in this batch   : %d", stats._2d.quads);
          ImGui::Text("Max Quad Per Batch    : %d", stats._2d.maxQuads);
          ImGui::Text("Circles in this batch : %d", stats._2d.circles);
          ImGui::Text("Max Circles Per Batch : %d", stats._2d.maxCircles);
          ImGui::Text("Lines in this batch   : %d", stats._2d.lines);
          ImGui::Text("Max Lines Per Batch   : %d", stats._2d.maxLines);
          ImGui::Text("Chars in this batch   : %d", stats._2d.chars);
          ImGui::Text("Max Char Per Batch    : %d", 16);
          
          ImGui::Separator();
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }
    }
    ImGui::End();
  }
  void RendererLayer::UI_WelcomePopup()
  {
    if (m_showWelcomePopup)
    {
      ImGui::OpenPopup("Welcome Screen");
      m_showWelcomePopup = false;
    }
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2{ 1000, 500 });

    UI::ScopedColor bgCol(ImGuiCol_ChildBg, IKan::UI::Theme::Color::BackgroundPopup);
    UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 15.0f));
    UI::ScopedStyle rounding(ImGuiStyleVar_FrameRounding, 10);

    if (ImGui::BeginPopupModal("Welcome Screen", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar
                               | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground))
    {
      ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV ;
      UI::PushID();

      if (ImGui::BeginTable(UI::GenerateID(), 2 /* Num Columns */, tableFlags, ImVec2(0.0f, 0.0f)))
      {
        const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

        ImGui::TableSetupColumn("##About/New_Project", 0, 650.0f);
        ImGui::TableSetupColumn("##Recent_Projects", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow();

        // About/New_Project -----------------------------------------------------
        ImGui::TableSetColumnIndex(0);
        ImGui::BeginChild("##About/New_Project");
        {
          const int32_t logoSize = 200;

          // Draw Kreator Logo -----
          {
            const ImVec2 logoOffset(windowPadding.x, windowPadding.y);
            const ImVec2 logoRectStart =
            {
              ImGui::GetItemRectMin().x + (ImGui::GetColumnWidth() / 2 - logoSize / 2),
              ImGui::GetItemRectMin().y
            };
            const ImVec2 logoRectMax =
            {
              logoRectStart.x + logoSize,
              logoRectStart.y + logoSize
            };
            auto* drawList = ImGui::GetWindowDrawList();
            drawList->AddImage(UI::GetTextureID(m_welcomeIcon), logoRectStart, logoRectMax, {0, 0}, {1, 1},
                               IM_COL32(255, 255, 255, 255));
          }

          // Welcome Header -------
          {
            UI::SetCursorPosY(logoSize + ImGui::GetCurrentWindow()->WindowPadding.y + 10);
            {
              UI::ScopedFont header(Kreator_UI::GetHugeHeaderFont());
              static std::string welcomeText = "Welcome to IKan-Kreate";
              UI::SetCursorPosX(ImGui::GetColumnWidth() / 2 - ImGui::CalcTextSize(welcomeText.c_str()).x / 2);
              ImGui::Text("%s", welcomeText.c_str());
            }
            
            {
              UI::ScopedFont version(Kreator_UI::GetBoldFont());
              static std::string versionText = "Version " + IKanVersion;
              UI::SetCursorPosX(ImGui::GetColumnWidth() / 2 - ImGui::CalcTextSize(versionText.c_str()).x / 2);
              ImGui::Text("%s", versionText.c_str());
            }
          }
          ImGui::Separator();

          // Buttons
          {
            auto button = [this](const char* title, Ref<Image> icon, const std::string& buttonHelper) {
              auto textSize = ImGui::CalcTextSize(buttonHelper.c_str()).x;;
              
              const float buttonHeight = 40.0f;
              const float buttonWidth = textSize + buttonHeight + 80;
              
              // Minimize Button
              UI::ShiftCursorX(80);
              if (ImGui::InvisibleButton(title, ImVec2(buttonWidth, buttonHeight), ImGuiButtonFlags_AllowItemOverlap))
              {
                return true;
              }
              
              const ImVec2 logoRectStart = ImGui::GetItemRectMin();
              const ImVec2 logoRectMax =
              {
                logoRectStart.x + buttonHeight,
                logoRectStart.y + buttonHeight
              };
              
              const ImU32 buttonColN = UI::ColorWithMultipliedValue(UI::Theme::Color::Text, 0.9f);
              const ImU32 buttonColH = UI::ColorWithMultipliedValue(UI::Theme::Color::Text, 1.2f);
              const ImU32 buttonColP = Kreator_UI::Color::TextDarker;
              UI::DrawButtonImage(icon, buttonColN, buttonColH, buttonColP, ImRect{logoRectStart, logoRectMax});
              
              ImGui::SameLine(140);
              UI::ShiftCursorY(10);
              {
                UI::ScopedFont text(Kreator_UI::GetSemiHeaderFont());
                UI::ScopedColor color(ImGuiCol_Text, IM_COL32(184, 184, 184, 255));
                
                ImGui::Text("%s", buttonHelper.c_str());
              }
              
              return false;
            };
            
            if (button("New Project", m_newProject, "Create New Kreator Project"))
            {
              m_showCreateNewProjectPopup = true;
              ImGui::CloseCurrentPopup();
            }
            
            if (button("Open Project", m_folder, "Open an exisiting Kreator Project"))
            {
              ImGui::CloseCurrentPopup();
              FolderExplorer::OpenPopup("Open Project", m_allProjectsPath, &m_showWelcomePopup);
              m_folderExplorerAction = FolderExplorerAction::OpenProject;
            }
          }
          
          UI::ShiftCursorY(35);
          UI::ShiftCursorX(10);
          bool showAgain = m_userPreferences->showWelcomeScreen;
          if (ImGui::Checkbox("##showAgain", &showAgain))
          {
            m_userPreferences->showWelcomeScreen = showAgain;
            UserPreferencesSerializer serializer(m_userPreferences);
            serializer.Serialize(m_userPreferences->filePath);
          }
          
          ImGui::SameLine();
          ImGui::TextUnformatted("Show this window again when Kreator Launches");

          ImGui::SameLine();
          
          // Draw side shadow-----------------------------------------------------------------
          ImRect windowRect = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), 0.0f, 0.0f);
          ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
          UI::DrawShadowInner(m_shadowTexture, 12.0f, windowRect, 1.0f, windowRect.GetHeight() / 4.0f,
                              false, true, false, false);
          ImGui::PopClipRect();
        }
        ImGui::EndChild(); // About/New_Project

        // Recent Project
        ImGui::TableSetColumnIndex(1);
        ImGui::BeginChild("##Recent_Projects");
        {
          // Title of Popup --------------------------------------------------
          {
            UI::ScopedColor muted(ImGuiCol_Text, IM_COL32(155, 155, 155, 255));
            
            static const std::string title = "Recent Projects";
            static const auto titleSize = ImGui::CalcTextSize(title.c_str());
            
            UI::ScopedFont version(Kreator_UI::GetSemiHeaderFont());
            UI::SetCursorPosY(10);
            UI::SetCursorPosX(ImGui::GetColumnWidth() / 2 - titleSize.x / 1.4);
            ImGui::Text(title.c_str());
            ImGui::Separator();
          }

          // Recent Project --------------------------------------------------
          {
            UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
            UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 20.0f));
            UI::ScopedFont semiHeader(Kreator_UI::GetSemiHeaderFont());
            
            m_openProjectPath = "";
            for (auto it = m_userPreferences->recentProjects.begin(); it != m_userPreferences->recentProjects.end(); it++)
            {
              if (!Utils::FileSystem::Exists(it->second.filePath))
              {
                m_userPreferences->recentProjects.erase(it);
                break;
              }
              
              ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_FramePadding;
              bool open = UI::TreeNode("##Recent_Projects", it->second.name, flags, m_projectIcon);
              if (ImGui::IsItemClicked())
              {
                m_openProjectPath = it->second.filePath;
              }
              
              if(open)
              {
                ImGui::TreePop();
              }
            }
            
            if(m_openProjectPath != "")
            {
              OpenProject();
              ImGui::CloseCurrentPopup();
            }
          }
          
          // Theme Selection --------------------------------------------------
          {
            UI::SetCursorPosY(ImGui::GetWindowHeight() - 30);
            int32_t theme = (int32_t)m_userPreferences->theme;
            if (Kreator_UI::PropertyDropdownNoLabel("Theme", {"Grey", "Blue"}, 2, &theme))
            {
              m_userPreferences->theme = static_cast<UserPreferences::Theme>(theme);
              UserPreferencesSerializer userPreferencesSerializer(m_userPreferences);
              userPreferencesSerializer.Serialize(m_userPreferences->filePath);
              
              Kreator_UI::SetTheme(m_userPreferences->theme);
            }
          }
        }
        ImGui::EndChild(); // Recent_Projects
        ImGui::EndTable();
      }
      
      UI::PopID();
      ImGui::EndPopup();
    }
  }
  
  void RendererLayer::UI_NewProjectPopup()
  {
    UI::ScopedStyle rounding(ImGuiStyleVar_FrameRounding, 0);

    if (m_showCreateNewProjectPopup)
    {
      ImGui::OpenPopup("New Project");
      m_projectNameBuffer.Memset(0);
      m_showCreateNewProjectPopup = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2{ 700, 0 });
    UI::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
    UI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2(4, 10));

    if (ImGui::BeginPopupModal("New Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove
                               | ImGuiWindowFlags_NoTitleBar))
    {
      // Title of Popup --------------------------------------------------
      {
        UI::ScopedColor muted(ImGuiCol_Text, IM_COL32(155, 155, 155, 255));

        static const std::string title = "New Project";
        static const auto titleSize = ImGui::CalcTextSize(title.c_str());

        UI::ScopedFont version(Kreator_UI::GetSemiHeaderFont());
        UI::SetCursorPosY(10);
        UI::SetCursorPosX(ImGui::GetColumnWidth() / 2 -  titleSize.x / 2);
        ImGui::Text(title.c_str());
      }
      
      UI::ShiftCursorY(-20);
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 200 / 8);
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
      
      ImGui::SetNextItemWidth(-1);
      ImGui::InputTextWithHint("##new_project_name", "Project Name", m_projectNameBuffer.Data(), m_projectNameBuffer.Size());
      
      ImVec2 labelSize = ImGui::CalcTextSize("...", NULL, true);
      auto& style = ImGui::GetStyle();
      ImVec2 buttonSize = ImGui::CalcItemSize(ImVec2(0, 0),
                                              labelSize.x + style.FramePadding.x * 2.0f,
                                              labelSize.y + style.FramePadding.y * 2.0f);

      ImGui::SetNextItemWidth(680 - style.FramePadding.x * 2.0f - style.ItemInnerSpacing.x - 1 - buttonSize.x);
      {
        UI::ScopedColor muted(ImGuiCol_Text, UI::Theme::Color::Muted);
        ImGui::InputTextWithHint("##new_project_location", "Project Location",
                                 m_projectFilePathBuffer.Data(), m_projectFilePathBuffer.Size(), ImGuiInputTextFlags_ReadOnly);
      }
      ImGui::SameLine();
      if (UI::DrawRoundButton("...", Kreator_UI::ColorVec3FromU32(UI::Theme::Color::BackgroundDark), 0))
      {
        ImGui::CloseCurrentPopup();
        FolderExplorer::SelectPopup("Select Project Directory", m_allProjectsPath, &m_showCreateNewProjectPopup);
        m_folderExplorerAction = FolderExplorerAction::NewPreoject;
      }
      
      // Buttons
      {
        auto boldFont = Kreator_UI::GetBoldFont();
        ImGui::PushFont(boldFont);
        
        UI::ShiftCursorX(ImGui::GetWindowWidth() / 2 - 90);
        if ((UI::DrawRoundButton("Create", Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 20)) or
            (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter)))
        {
          std::string fullProjectPath = "";
          if (strlen(m_projectFilePathBuffer.Data()) > 0)
          {
            fullProjectPath = std::string(m_projectFilePathBuffer) + "/" + std::string(m_projectNameBuffer);
          }
          
          CreateProject(fullProjectPath);
          ImGui::CloseCurrentPopup();
        }
        
        ImGui::SameLine();
        if ((UI::DrawRoundButton("Cancel", Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 20)) or
            (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape)))
        {
          if (!Project::GetActive())
          {
            m_showWelcomePopup = true;
          }
          ImGui::CloseCurrentPopup();
        }
        ImGui::PopFont();
      }

      ImGui::PopStyleVar();
      ImGui::EndPopup();
    }
  }

  void RendererLayer::UI_FolderExplorer()
  {
    // Folder explorer if clicked ... Button
    auto explorerOutput = FolderExplorer::Explore();
    if (explorerOutput != "")
    {
      switch (m_folderExplorerAction)
      {
        case FolderExplorerAction::NewPreoject:
        {
          break;
        }
        case FolderExplorerAction::OpenProject:
        {
          if (Utils::String::GetExtensionFromPath(explorerOutput) == ProjectExtension)
          {
            m_openProjectPath = explorerOutput;
            OpenProject();
          }
          else
          {
            FolderExplorer::OpenPopup("Open Project", explorerOutput.parent_path());
            m_folderExplorerAction = FolderExplorerAction::OpenProject;
          }
          break;
        }
        case FolderExplorerAction::SaveScene:
        {
          m_sceneFilePath = explorerOutput;
          m_sceneFilePath += SceneExtension;
          
          SaveScene();
          UpdateWindowTitle(m_sceneFilePath);
          
          break;
        }
        case FolderExplorerAction::OpenScene:
        {
          if (Utils::String::GetExtensionFromPath(explorerOutput) == SceneExtension)
          {
            OpenScene(explorerOutput);
          }
          else
          {
            FolderExplorer::OpenPopup("Open Scene", explorerOutput.parent_path());
            m_folderExplorerAction = FolderExplorerAction::OpenScene;
          }
          break;
        }
          
        case FolderExplorerAction::None:
        default:
          IK_ASSERT(false);
          break;
      }
    }
  }
  
  void RendererLayer::UI_EditorPanel()
  {
    if (!m_showEditorPanel)
    {
      return;
    }
    
    ImGui::Begin("Settings", &m_showEditorPanel);
    {
      Kreator_UI::BeginPropertyGrid();
      ImGui::AlignTextToFramePadding();

      float camSpeed = m_editorCamera.GetNormalSpeed();
      if (Kreator_UI::PropertySlider("Camera Speed", camSpeed, 0.0005f, 2.f))
      {
        m_editorCamera.SetNormalSpeed(camSpeed);
      }
      
      // Other Settings
      {
        Kreator_UI::Property("Show Icons", m_showIcons);
        Kreator_UI::Property("Show Grids", m_showGrid);
        Kreator_UI::Property("Show Mini Viewport", m_showMiniViewport);
      }
      
      ImGui::Separator();
      // Physics
      {
        Kreator_UI::Property("Show Colliders", m_showColliders);
        Kreator_UI::Property("Gravity", m_currentScene->GetPhysicsSettings().gravity);
      }
      Kreator_UI::EndPropertyGrid();
    }
    ImGui::End();
  }
  
  std::filesystem::path RendererLayer::GetClientResorucePath() const
  {
    return m_clientResourcePath;
  }
} // namespace Kreator
