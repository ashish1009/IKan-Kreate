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
  using namespace IKan;
  
  class RendererLayer : public Layer
  {
  public:
    /// This is Renderer Layer's Default Constructor
    /// - Parameters:
    ///   - userPreferences: User preference from Client
    RendererLayer(Ref<UserPreferences> userPreferences, const std::filesystem::path& clientDirPath);
    /// Default Destructor
    virtual ~RendererLayer();
    
    // Overriden APIs ------------------------------------------------------------------------------------------------
    /// This funciton initialises the layer, calls when we push layer in the application (To be called only once)
    void OnAttach() override;
    /// This funciton destroy the layer, calls when we pop layer from the application (To be called only once)
    void OnDetach() override;
    
    /// This function renders the GUI Window for this layer. To be called each frame from application.
    /// - Note: Core Application is taking care to call the RenderGui API for all Layers
    void OnImguiRender() override;
    /// This function updates the layer data. To be called each frame from application.
    /// - Note Core Application is taking care to call the Update(ts) API for all Layers
    /// - Parameter ts: Time step between 2 Frames
    void OnUpdate(TimeStep ts) override;
    /// This function handles all the events triggered for the layer. To be whenever an event is triggered in the window.
    /// - Note Core Application is taking care to call the EventHandler(event) API for all Layers
    /// - Parameter event: Event (Base class) intance. Dispatch event from Event Dispatcher
    void OnEvent(Event& event) override;
    
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

    // Project API --------------------------------------
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

    // Scene APIs ----------------------------------------
    /// This function creates new scene
    /// - Parameter name: Scene name
    void NewScene(const std::string& name = "UntitledScene");

    // Member Variables ----------------------------------------------------------------------------------------------
    std::filesystem::path m_clientDirPath;
    
    // Camera Data ---------------------------------------
    bool m_allowViewportCameraEvents = false;
    EditorCamera m_editorCamera;

    // Scene Data ----------------------------------------
    std::string m_sceneFilePath;
    Ref<Scene> m_editorScene, m_currentScene;
    
    // Project Data --------------------------------------
    std::filesystem::path m_templateProjectDir;
    Ref<UserPreferences> m_userPreferences;
    char* m_projectNameBuffer = nullptr;
    char* m_projectFilePathBuffer = nullptr;
  };
} // namespace Kreator
