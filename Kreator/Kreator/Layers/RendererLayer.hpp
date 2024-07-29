//
//  RendererLayer.hpp
//  Kreator
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Config.hpp"
#include "Editor/Viewport.hpp"
#include "Editor/UserPreference.hpp"

using namespace IKan;

namespace Kreator
{
  /// This class is the Renderer layer for Kreator
  class RendererLayer : public Layer
  {
  public:
    /// This is Renderer Layer's Default Constructor
    /// - Parameters:
    ///   - directories: client directories
    ///   - userPreferences: User preference
    RendererLayer(const KreatorDirectories& directories, const Ref<UserPreferences> userPreferences);
    /// This Constructor destroys the Renderer Layer
    ~RendererLayer();
    
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
    
    // Static APIs ----------------------------------------------------------------------------------------------------
    /// This function returns the single instance of the renderer layer
    static RendererLayer& Get();

  private:
    // Memeber functions ----------------------------------------------------------------------------------------------
    /// This function updates the viewports of all kreator data
    void UpdateViewportSize();
    
    // Project APIs --------------------------------------
    /// This function creates a new project for kreator
    /// - Parameter newProjectFilePath: New project file path
    void CreateProject(const std::filesystem::path& newProjectFilePath);
    /// This function opens the Project from file path
    /// - Parameter projectFilePath: project file path
    void OpenProject(const std::filesystem::path& projectFilePath);
    /// This function closes the current active project
    void CloseProject();
    /// This funciton push the current project to recent project
    /// - Parameter projectPath: Project to be pushed
    void PushProjectToRecentProjects(const std::filesystem::path& projectPath);

    // UI APIs -------------------------------------------
    /// This function shows the Welcome screen Popup
    void UI_WelcomePopup();
    /// This function shows new project popup
    void UI_NewProjectPopup();

    /// This function starts main ImGui Window with Docking
    void UI_StartMainWindowDocking();
    /// This function ends Main ImGui window with Docking
    void UI_EndMainWindowDocking();
    /// This functions shows primary Viewport
    void UI_Viewport();

    // Memeber variables ----------------------------------------------------------------------------------------------
    // Popups/Panel --------------------------------------
    UI::Popup m_welcomePopup {"Welcome Screen"};
    UI::Popup m_createNewProjectPopup {"Create New Project"};

    // Applicaiton Textures ------------------------------
    Ref<Image> m_welcomeIcon;
    Ref<Image> m_applicationIcon;
    Ref<Image> m_newProjectIcon, m_folderIcon;
    Ref<Image> m_shadowTexture;
    Ref<Image> m_2DIcon, m_3DIcon;

    // Application data ----------------------------------
    KreatorDirectories m_directories;
    Ref<UserPreferences> m_userPreferences;
    float m_windowOriginalWidth {0}, m_windowOriginalHeight {0};
    enum class SelectedProject : uint8_t
    {
      None, _2D, _3D
    };
    SelectedProject m_selectedProject {SelectedProject::None};

    // Viewport Data -------------------------------------
    Viewport m_primaryViewport, m_secondaryViewport;
    SceneRenderer m_viewportRenderer, m_miniViewportRenderer, m_secondaryViewportRenderer;

    // Editor Data ---------------------------------------
    EditorCamera m_editorCamera;
    PanelManager m_panels;

    // Single Instance -----------------------------------
    static RendererLayer* s_instance;
  };
} // namespace Kreator
