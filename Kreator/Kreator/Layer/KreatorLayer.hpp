//
//  KreatorLayer.hpp
//  Kreator
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Editor/UserPreferences.hpp"

using namespace IKan;

namespace Kreator
{
  static const std::string KreatorVersion = "4.4";
  
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
    /// This function Updates the viewports of all Kreator Data
    void UpdateViewportSize();

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

    // Member variables ----------------------------------------------------------------------------------------------
    // Popups --------------------------------------------
    bool m_showWelcomePopup {false};
    bool m_showCreateNewProjectPopup {false};
    bool m_showStatisticsPanel {true};

    // Application Data ----------------------------------
    std::filesystem::path m_clientResourcePath {};
    std::filesystem::path m_systemUserPath {};
    std::filesystem::path m_iKanKreatePath {};
    Ref<UserPreferences> m_userPreferences;
    
    // Applicaiton Icons ---------------------------------
    Ref<Texture> m_shadowTexture;
    Ref<Texture> m_applicationIcon, m_welcomeIcon;
    Ref<Texture> m_newProjectIcon, m_folderIcon;
    Ref<Texture> m_projectIcon;
    Ref<Texture> m_iconMinimize, m_iconMaximize, m_iconRestore, m_iconClose;

    // Project Data --------------------------------------
    std::filesystem::path m_openProjectPath = "";
    GUI_InputBuffer<255> m_projectNameBuffer;
    GUI_InputBuffer<512> m_projectFilePathBuffer;
    
    // Editor Data ---------------------------------------
    bool m_allowViewportCameraEvents = false;
    PanelManager m_panels;
    enum class FolderExplorerAction
    {
      None, NewPreoject, OpenProject
    };
    FolderExplorerAction m_folderExplorerAction {FolderExplorerAction::None};
    
    // Scene Data ----------------------------------------
    std::string m_sceneFilePath{};
    enum class SceneState
    {
      Edit = 0, Play = 1, Pause = 2, Simulate = 3
    };
    SceneState m_sceneState {SceneState::Edit};

    // View port Data ------------------------------------
    Viewport m_viewport;
    SceneRenderer m_viewportRenderer, m_miniViewportRenderer;

    // Single Instance -----------------------------------
    static KreatorLayer* s_instance;
  };
} // namespace Kreator
