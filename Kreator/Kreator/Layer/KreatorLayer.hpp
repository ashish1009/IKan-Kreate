//
//  KreatorLayer.hpp
//  Kreator
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Editor/UserPreferences.hpp"
#include "Editor/SelectionContext.hpp"

using namespace IKan;

namespace Kreator
{
  static const std::string KreatorVersion = "6.0";
  
  /// Stores the Viewport Data
  struct Viewport
  {
    struct MouseViewportSpace
    {
      float spaceMouseX, spaceMouseY;
    };

    bool panelMouseHover;
    bool panelFocused;
    uint32_t width, height;
    glm::vec2 bounds[2];
    MouseViewportSpace mouseViewportSpace;

    /// This function returns the space mouse position
    void GetMouseViewportSpace();
    /// This function check the mouse position inside viewport
    bool IsMouseSpaceInViewport();
  };

  class KreatorLayer : public Layer
  {
  public:
    /// This is Renderer Layer's Default Constructor
    /// - Parameters:
    ///   - clientResourcePath: Client resource path
    ///   - userPreferences: User preference
    ///   - systemUserPath: System user path
    KreatorLayer(const std::filesystem::path& clientResourcePath, const std::filesystem::path& systemUserPath, const std::filesystem::path& iKanKreatePath, Ref<UserPreferences> userPreferences);
    /// Default Destructor
    ~KreatorLayer();
    
    // Overriden APIs ------------------------------------------------------------------------------------------------
    /// This funciton initialises the layer, calls when we push layer in the application (To be called only once)
    void OnAttach() override;
    /// This funciton destroy the layer, calls when we pop layer from the application (To be called only once)
    void OnDetach() override;
    
    /// This function renders the GUI Window for this layer. To be called each frame from application.
    /// - Note: Core Application is taking care to call the RenderGui API for all Layers
    void OnImGuiRender() override;
    /// This function updates the layer data. To be called each frame from application.
    /// - Parameter ts: Time step between 2 Frames
    /// - Note Core Application is taking care to call the Update(ts) API for all Layers
    void OnUpdate(TimeStep ts) override;
    /// This function handles all the events triggered for the layer. To be whenever an event is triggered in the window.
    /// - Parameter event: Event (Base class) intance. Dispatch event from Event Dispatcher
    /// - Note Core Application is taking care to call the EventHandler(event) API for all Layers
    void OnEvent(Event& event) override;
    
    // Scene Public APIs --------------------------------
    /// This function opens new scene with file
    /// - Parameter filepath: Scene filepath
    void OpenScene(const std::filesystem::path& filepath);
    
    // Getters -------------------------------------------------------------------------------------------------------
    /// This function returns the client file path
    const std::filesystem::path& GetClientResorucePath() const;
    /// This function returns the system user storage path
    const std::filesystem::path& GetSystemUserPath() const;
    /// This function returns the system IKan-Kreator path
    const std::filesystem::path& GetIKanKreatorPath() const;
    
    /// This function returns the single instance of the renderer layer
    static KreatorLayer& Get();
    
  private:
    // Member functions ----------------------------------------------------------------------------------------------
    /// This function handles the key press event
    /// - Parameter e: key events
    bool OnKeyPressedEvent(KeyPressedEvent& e);
    /// This function handles the mouse events
    /// - Parameter e: Mouse Event
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    /// This function Updates the viewports of all Kreator Data
    void UpdateViewportSize();
    
    /// This function cast ray from camera
    /// - Parameter camera: Camera
    Ray CastRay(const EditorCamera& camera);
    
    /// This function renders the debug icons, colliders and so on....
    void DebugRenderer();
    /// This function renders the system info
    void RenderSystemInfo();
    /// This function renders the relationship connection
    void RenderRelationshipConnection();
    /// This function renders the camera A
    void RenderCameraAxis();
    /// This function renders the debug Colliders
    void ShowColliders();

    /// This function returns the Imguizmo Snap value
    float GetSnapValue();
    
    // Project API ---------------------------------------
    /// This function Creates new project
    /// - Parameter projectDir: Project Direcotry
    void CreateProject(const std::filesystem::path& projectDir);
    /// This function Closed the current active project
    void CloseProject();
    /// This function opens the Project from file path
    /// - Parameter filepath: project file path
    void OpenProject(const std::filesystem::path& filepath);
    /// This function Opens the project from popup
    void OpenProject();
    /// This funciton push the current project to recent project
    /// - Parameter projectPath: Project to be pushed
    void PushProjectToRecentProjects(const std::filesystem::path& projectPath);
    
    // Scene APIs ----------------------------------------
    /// This function creates new scene
    /// - Parameter name: Scene name
    void NewScene(const std::string& name = "UntitledScene");
    /// This function closes the scene
    void CloseCurrentScene();
    /// This function Saves the scene at new location
    void SaveSceneAs();
    /// This function Saves the scene
    void SaveScene();
    /// This function Saves the scene automatically
    void SaveSceneAuto();
    /// This function Opens the scene from popup
    void OpenScene();
    /// This function Opens the scene from metadata
    /// - Parameter assetMetadata: metadata
    void OpenScene(const AssetMetadata& assetMetadata);
    
    /// This function handle scene play
    void OnScenePlay();
    /// This function handle scene Stop
    void OnSceneStop();
    /// This function handle scene Pause
    void OnScenePause();
    /// This function handle scene Resume
    void OnSceneResume();
    /// This function handle scene simuation start
    void OnSceneStartSimulation();
    /// This function handle scene simuation ends
    void OnSceneStopSimulation();
    
    // Scene Panel Manager Callbacks ----------------------
    /// This function handles the entity selection
    /// - Parameter entities: entities
    void OnEntitySelected(const SelectionContext& entities);
    /// This function handles the entity deletion
    /// - Parameter entities: entities
    void OnEntityDeleted(const SelectionContext& entities);
    /// This function clear the selected Entity
    /// - Note: To be called inside Render Pass Begin end End
    void ClearSelectedEntity();
    /// This function set the selected entity
    /// - Parameter entity: Selected entity
    void SetSelectedEntity(const Entity& entity);
    /// This function checks is entity is selected
    /// - Parameter e: Entity to be checked
    bool IsEntitySelected(Entity e) const;
    
    // UI APIs -------------------------------------------
    /// This function shows the Welcome screen Popup
    void UI_WelcomePopup();
    /// This function shows new project popup
    void UI_NewProjectPopup();
    /// This function update the folder explorer output
    void UI_FolderExplorer();
    /// This function Creates main ImGui Window with Docking
    void UI_StartMainWindowDocking();
    /// This function Ends Main ImGui window with Docking
    void UI_EndMainWindowDocking();
    /// This function renders our title bar
    float UI_DrawTitlebar();
    /// This function handle the drag and Click on title bar
    /// - Parameter titlebarHeight: Title bar height
    void UI_TitlebarDragArea(float titlebarHeight);
    /// This function renders the Menu bar for Title bar
    void UI_MenuBar();
    /// This function renders window buttons
    void UI_WindowButtons();
    /// This function shows the Statistics Panel
    void UI_StatisticsPanel();
    /// This functions shows primary Viewport
    void UI_Viewport();
    /// This function popup new scene widget
    void UI_NewScenePopup();
    /// This funciton shows the scene settings
    void UI_SceneSettings();
    /// This function Update the Guizmo Renderer
    void UI_UpdateGuizmo();
    /// This function renders the Guizmo toolbar
    void UI_GuizmoToolbar();
    /// This function shows the Applicaton toolbar
    void UI_SceneToolbar();
    /// This function renders the Camera toolbar
    void UI_CameraToolbar();
    /// This function handles the Drop content in viewport
    void UI_DropViewport();
    /// This function Popup invalid asset open
    void UI_InvalidAssetMetadataPopup();
    
    // Member variables ----------------------------------------------------------------------------------------------
    // Popups/Panel --------------------------------------
    bool m_showWelcomePopup {false};
    bool m_showCreateNewProjectPopup {false};
    bool m_showStatisticsPanel {true};
    bool m_showNewScenePopup {false};
    bool m_showSceneSettings {true};
    bool m_showInvalidAssetMetadataPopup {false};
    
    // Debug Renderer ------------------------------------
    bool m_renderSystemInfo {true};
    bool m_showRelationshipConnection {true};
    bool m_showCameraAxis {true};
    bool m_showColliders {true};

    // Debug renderer colors -----------------------------
    glm::vec4 m_relationshipColor = {0.4, 0.4, 0.4, 0.4};
    glm::vec4 m_colliderColor = {0.1, 0.6, 0.3, 0.7};

    // Application Data ----------------------------------
    std::filesystem::path m_clientResourcePath {};
    std::filesystem::path m_systemUserPath {};
    std::filesystem::path m_iKanKreatePath {};
    Ref<UserPreferences> m_userPreferences;
    
    // Applicaiton Textures ------------------------------
    Ref<Texture> m_shadowTexture;
    Ref<Texture> m_applicationIcon, m_welcomeIcon;
    Ref<Texture> m_newProjectIcon, m_folderIcon;
    Ref<Texture> m_projectIcon;
    Ref<Texture> m_iconMinimize, m_iconMaximize, m_iconRestore, m_iconClose;
    Ref<Texture> m_gridTexture;

    // Project Data --------------------------------------
    std::filesystem::path m_openProjectPath = "";
    GUI_InputBuffer<255> m_projectNameBuffer;
    GUI_InputBuffer<512> m_projectFilePathBuffer;
    
    // Editor Data ---------------------------------------
    bool m_allowViewportCameraEvents = false;
    PanelManager m_panels;
    EditorCamera m_editorCamera;
    
    Ref<Texture> m_XIcon, m_YIcon, m_ZIcon;
    enum class FolderExplorerAction
    {
      None, NewPreoject, OpenProject, SaveScene, OpenScene
    };
    FolderExplorerAction m_folderExplorerAction {FolderExplorerAction::None};
    struct InvalidAssetMetadataPopupData
    {
      AssetMetadata metadata;
    } m_invalidAssetMetadataPopupData;

    // Scene Data ----------------------------------------
    float m_timeSinceLastSave = 0.0f;
    std::filesystem::path m_sceneFilePath{};
    enum class SceneState
    {
      Edit = 0, Play = 1, Pause = 2, Simulate = 3
    };
    SceneState m_sceneState {SceneState::Edit};
    Ref<Scene> m_currentScene, m_editorScene, m_runtimeScene, m_simulationScene;
    struct SelectedEntity
    {
      Entity entity;
      float distance = 0.0f;
    };
    std::vector<SelectedEntity> m_selectionContext;
    Ref<Texture> m_stopButtonTex, m_playButtonTex, m_simulateButtonTex, m_pauseButtonTex;

    // View port Data ------------------------------------
    Viewport m_viewport;
    SceneRenderer m_viewportRenderer, m_miniViewportRenderer;
    
    // Guizmo Data ---------------------------------------
    bool m_hoveredGuizmoToolbar = false;
    int32_t m_gizmoType = -1; // -1 = no gizmo
    uint32_t m_gizmoMode = 1; //  0 = local, 1 = World
    Ref<Texture> m_selectToolTex, m_moveToolTex, m_rotateToolTex, m_scaleToolTex, m_gizmoModeTex;

    // Single Instance -----------------------------------
    static KreatorLayer* s_instance;
  };
} // namespace Kreator
