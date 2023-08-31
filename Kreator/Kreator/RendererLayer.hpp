//
//  RendererLayer.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "UserPreference.hpp"

namespace Kreator
{
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
    
    // Getters -------------------------------------------------------------------------------------------------------
    /// This function returns the client file path
    std::filesystem::path GetClientResorucePath() const;

    /// This function returns the single instance of the renderer layer
    static RendererLayer& Get();
    
  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    // UI API --------------------------------------------
    /// This function shows the Welcome screen
    void UI_WelcomePopup();
    /// This function shows new project popup
    void UI_NewProjectPopup();

    // Member Variables ----------------------------------------------------------------------------------------------
    // Popups --------------------------------------------
    bool m_showWelcomePopup = false;

    // Applicaiton Icons ---------------------------------
    Ref<Image> m_shadowTexture;
    Ref<Image> m_newProject, m_folder;
    Ref<Image> m_applicationIcon, m_welcomeIcon;
    Ref<Image> m_iconMinimize, m_iconMaximize, m_iconRestore, m_iconClose;

    // Project Data --------------------------------------
    bool m_showCreateNewProjectPopup = false;
    Ref<UserPreferences> m_userPreferences;
    std::filesystem::path m_allProjectsPath;
    std::filesystem::path m_templateProjectDir;
    GUI_InputBuffer<255> m_projectNameBuffer;
    GUI_InputBuffer<512> m_projectFilePathBuffer;

    // Client Data ---------------------------------------
    std::filesystem::path m_clientResourcePath;

    // Single Instance -----------------------------------
    static RendererLayer* s_instance;
  };
} // namespace Kreator
