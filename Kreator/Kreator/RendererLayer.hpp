//
//  RendererLayer.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace Kreator
{
  // Project Path Name Length
#define MAX_PROJECT_NAME_LENGTH 255
#define MAX_PROJECT_FILEPATH_LENGTH 512

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
    ///   - userPreferences: User preference from Client
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
    
    /// This function returns the client file path
    static std::filesystem::path GetClientResorucePath();
    
  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function handles the key press event
    /// - Parameter e: key events
    bool OnKeyPressedEvent(KeyPressedEvent& e);
    /// This function handles the mouse events
    /// - Parameter e: Mouse Event
    bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

    /// This function updates the name of window tile
    /// - Parameter sceneName: scene name
    void UpdateWindowTitle(const std::string& sceneName);
    /// This function Updates the viewports of all Kreator Data
    void UpdateViewportSize();

    // Project API ---------------------------------------
    /// This function Creates new project
    /// - Parameter projectPath: Project File path
    void CreateProject(const std::filesystem::path& projectPath);
    /// This function Closed the current active project
    /// - Parameter unloadProject: flag to unload the project
    void CloseProject(bool unloadProject = true);
    /// This function saves the Project
    void SaveProject();
    /// This function opens the Project from file path
    /// - Parameter filepath: project file path
    void OpenProject(const std::string& filepath);
    /// This function Opens the project from popup
    void OpenProject();
    /// This funciton push the current project to recent project
    /// - Parameter projectPath: Project to be pushed
    void PushProjectToRecentProjects(std::filesystem::path projectPath);
    
    // Scene APIs ----------------------------------------
    /// This function creates new scene
    /// - Parameter name: Scene name
    void NewScene(const std::string& name = "UntitledScene");
    /// This function opens new scene with file from popup
    void OpenScene();
    /// This function opens new scene with file
    /// - Parameter filepath: Scene filepath
    void OpenScene(const std::string& filepath);
    /// This function Saves the scene at new location
    void SaveSceneAs();
    /// This function Saves the scene
    void SaveScene();
    /// This function Saves the scene automatically
    void SaveSceneAuto();

    // UI API --------------------------------------------
    /// This functions shows primary Viewport
    void UI_Viewport();
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

    /// This function shows the Welcome screen
    void UI_WelcomePopup();
    /// This function shows the About Application popup
    void UI_AboutPopup();

    /// This function shows new project popup
    void UI_NewProjectPopup();
    /// This function update the folder explorer output
    void UI_FolderExplorer();
    
    /// This function popup new scene widget
    void UI_NewScene();
    
    // Member Variables ----------------------------------------------------------------------------------------------
    // Popups --------------------------------------------
    bool m_showWelcomePopup = false;
    bool m_showAboutPopup = false;
    bool m_showStatisticsPanel = true;

    // Applicaiton Icons ---------------------------------
    Ref<Image> m_shadowTexture;
    Ref<Image> m_newProject, m_folder;
    Ref<Image> m_applicationIcon, m_welcomeIcon;
    Ref<Image> m_iconMinimize, m_iconMaximize, m_iconRestore, m_iconClose;

    // Camera Data ---------------------------------------
    bool m_allowViewportCameraEvents = false;
    EditorCamera m_editorCamera;

    // Scene Data ----------------------------------------
    bool m_showNewScenePopup = false;
    float m_timeSinceLastSave = 0.0f;
    std::string m_sceneFilePath;
    Ref<Scene> m_editorScene, m_currentScene;
    
    // Project Data --------------------------------------
    bool m_showCreateNewProjectPopup = false;
    std::filesystem::path m_allProjectsPath;
    std::filesystem::path m_templateProjectDir;
    Ref<UserPreferences> m_userPreferences;
    char* m_projectNameBuffer = nullptr;
    char* m_projectFilePathBuffer = nullptr;
    std::filesystem::path m_openProjectPath = "";
    Ref<Image> m_projectIcon;

    // View port Data ------------------------------------
    Viewport m_viewport;
        
    // Editor Data ---------------------------------------
    enum class FolderExplorerAction
    {
      None, NewPreoject, OpenProject, SaveScene, OpenScene
    };
    FolderExplorerAction m_folderExplorerAction;
    PanelManager m_panels;
    
    // Client Data ---------------------------------------
    static std::filesystem::path s_clientResourcePath;
  };
} // namespace Kreator
