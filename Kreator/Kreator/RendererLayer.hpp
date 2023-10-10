//
//  RendererLayer.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "UserPreference.hpp"
#include "EntitySelectionContext.hpp"

namespace Kreator
{
  /// Stores the Viewport Data
  struct Viewport
  {
    bool panelMouseHover;
    bool panelFocused;
    uint32_t width, height;
    glm::vec2 bounds[2];
    int32_t mousePosX = -1;
    int32_t mousePosY = -1;
    
    void UpdateMousePos();
    std::pair<float, float> GetMouseSpace();
    std::pair<float, float> GetMousePos();
  };

  class RendererLayer : public Layer
  {
  public:
    /// This is Renderer Layer's Default Constructor
    /// - Parameters:
    ///   - userPreferences: User preference
    ///   - clientResourcePath: Client resource path
    RendererLayer(Ref<UserPreferences> userPreferences, const std::filesystem::path& clientResourcePath);
    /// Default Destructor
    virtual ~RendererLayer();
    
    // Overriden APIs ------------------------------------------------------------------------------------------------
    /// This funciton initialises the layer, calls when we push layer in the application (To be called only once)
    void OnAttach() override;
    /// This funciton destroy the layer, calls when we pop layer from the application (To be called only once)
    void OnDetach() override;
    
    /// This function renders the GUI Window for this layer. To be called each frame from application.
    /// - Note: Core Application is taking care to call the RenderGui API for all Layers
    void OnImGuiRender() override;
    /// This function updates the layer data. To be called each frame from application.
    /// - Note Core Application is taking care to call the Update(ts) API for all Layers
    /// - Parameter ts: Time step between 2 Frames
    void OnUpdate(TimeStep ts) override;
    /// This function handles all the events triggered for the layer. To be whenever an event is triggered in the window.
    /// - Note Core Application is taking care to call the EventHandler(event) API for all Layers
    /// - Parameter event: Event (Base class) intance. Dispatch event from Event Dispatcher
    void OnEvent(Event& event) override;
    
    // Scene Public APIs --------------------------------
    /// This function opens new scene with file
    /// - Parameter filepath: Scene filepath
    void OpenScene(const std::string& filepath);
    
    // Getters -------------------------------------------------------------------------------------------------------
    /// This function returns the client file path
    std::filesystem::path GetClientResorucePath() const;
    
    /// This function returns the single instance of the renderer layer
    static RendererLayer& Get();
    
  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function handles the key press event
    /// - Parameter e: key events
    bool OnKeyPressedEvent(KeyPressedEvent& e);
    /// This function handles the mouse events
    /// - Parameter e: Mouse Event
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

    // Scene Panel Manager Callbacks ----------------------
    /// This function handles the entity selection
    /// - Parameter entities: entities
    void OnEntitySelected(SelectionContext entities);
    /// This function handles the entity deletion
    /// - Parameter entities: entities
    void OnEntityDeleted(SelectionContext entities);
    /// This function clear the selected Entity
    /// - Note: To be called inside Render Pass Begin end End
    void ClearSelectedEntity();
    /// This function set the selected entity
    /// - Parameter entity: Selected entity
    void SetSelectedEntity(Entity entity);

    // Scene APIs ----------------------------------------
    /// This function closes the scene
    void CloseCurrentScene();

    // Member Variables ----------------------------------------------------------------------------------------------
    // Flags ---------------------------------------------
    bool m_showIcons = true;
    bool m_showColliders = true;
    bool m_showMiniViewport = true;
    bool m_showGrid = true;
    
    // Popups --------------------------------------------
    bool m_showWelcomePopup = false;
    bool m_showAboutPopup = false;
    bool m_showStatisticsPanel = true;
    bool m_showEditorPanel = true;
    
    // Applicaiton Icons ---------------------------------
    Ref<Image> m_shadowTexture;
    Ref<Image> m_newProject, m_folder;
    Ref<Image> m_applicationIcon, m_welcomeIcon;
    Ref<Image> m_iconMinimize, m_iconMaximize, m_iconRestore, m_iconClose;
    Ref<Image> m_cameraIcon, m_lightIcon;
    Ref<Image> m_settingIcon;
    
    // Project Data --------------------------------------
    bool m_showCreateNewProjectPopup = false;
    Ref<UserPreferences> m_userPreferences;
    std::filesystem::path m_allProjectsPath = "";
    std::filesystem::path m_templateProjectDir = "";
    std::filesystem::path m_openProjectPath = "";
    GUI_InputBuffer<255> m_projectNameBuffer;
    GUI_InputBuffer<512> m_projectFilePathBuffer;
    Ref<Image> m_projectIcon;
    
    // Editor Data ---------------------------------------
    enum class FolderExplorerAction
    {
      None, NewPreoject, OpenProject, SaveScene, OpenScene
    };
    FolderExplorerAction m_folderExplorerAction;
    PanelManager m_panels;
    
    // Camera Data ---------------------------------------
    bool m_allowViewportCameraEvents = false;
    EditorCamera m_editorCamera;
    
    // View port Data ------------------------------------
    Viewport m_viewport;
    
    // Client Data ---------------------------------------
    std::filesystem::path m_clientResourcePath;
    
    // Renderers -----------------------------------------
    Ref<SceneRenderer> m_viewportRenderer;
    Ref<SceneRenderer> m_miniViewportRenderer;
    
    // Scene Data ----------------------------------------
    bool m_showNewScenePopup = false;
    float m_timeSinceLastSave = 0.0f;
    std::string m_sceneFilePath;
    Ref<Scene> m_editorScene, m_currentScene, m_runtimeScene, m_simulationScene;
    Ref<Image> m_stopButtonTex, m_playButtonTex, m_simulateButtonTex, m_pauseButtonTex;
    enum class SceneState
    {
      Edit = 0, Play = 1, Pause = 2, Simulate = 3
    };
    SceneState m_sceneState = SceneState::Edit;
    struct SelectedSubmesh
    {
      Entity entity;
      float distance = 0.0f;
    };
    std::vector<SelectedSubmesh> m_selectionContext;
    int32_t m_hoveredEntityID = -1;
    
    // Guizmo Data ---------------------------------------
    bool m_hoveredGuizmotoolbar = false;
    int32_t m_gizmoType = -1; // -1 = no gizmo
    uint32_t m_gizmoMode = 1;  //  0 = local, 1 = World
    Ref<Image> m_selectToolTex, m_moveToolTex, m_rotateToolTex, m_scaleToolTex, m_gizmoModeTex;
    
    // Editor Data ---------------------------------------
    Ref<Material> m_jointMaterial;
    Ref<Image> m_checkerboardTex;
    
    // Single Instance -----------------------------------
    static RendererLayer* s_instance;
  };
} // namespace Kreator
