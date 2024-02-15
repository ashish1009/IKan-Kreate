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
  class KreatorLayer : public Layer
  {
  public:
    /// This is Renderer Layer's Default Constructor
    /// - Parameters:
    ///   - clientResourcePath: Client resource path
    ///   - userPreferences: User preference
    KreatorLayer(const std::filesystem::path& clientResourcePath, Ref<UserPreferences> userPreferences);
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
    
  private:
    // Member functions ----------------------------------------------------------------------------------------------
    // Project API ---------------------------------------
    /// This function Creates new project
    /// - Parameter projectDir: Project Direcotry
    void CreateProject(const std::filesystem::path& projectDir);
    
    // UI APIs -------------------------------------------
    /// This function shows the Welcome screen Popup
    void UI_WelcomePopup();
    
    // Member variables ----------------------------------------------------------------------------------------------
    // Popups --------------------------------------------
    bool m_showWelcomePopup {false};

    // Application Data ----------------------------------
    std::filesystem::path m_clientResourcePath {};
    Ref<UserPreferences> m_userPreferences;
  };
} // namespace Kreator
