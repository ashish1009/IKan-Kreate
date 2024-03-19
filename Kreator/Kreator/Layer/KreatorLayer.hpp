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
    KreatorLayer(const std::filesystem::path& clientResourcePath, const std::filesystem::path& systemUserPath, 
                 const std::filesystem::path& iKanKreatePath, Ref<UserPreferences> userPreferences);
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
    
    /// This function returns the single instance of the renderer layer
    static KreatorLayer& Get();

  private:
    // Member functions ----------------------------------------------------------------------------------------------
    /// This function renders the debug icons, colliders and so on....
    void DebugRenderer();

    // Member variables ----------------------------------------------------------------------------------------------
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
    Ref<Texture> m_selectToolTex, m_moveToolTex, m_rotateToolTex, m_scaleToolTex, m_gizmoModeTex;
    Ref<Texture> m_stopButtonTex, m_playButtonTex, m_simulateButtonTex, m_pauseButtonTex;
    Ref<Texture> m_XIcon, m_YIcon, m_ZIcon, m_cameraIcon;

    // Editor Data ---------------------------------------
    bool m_allowViewportCameraEvents = false;
    EditorCamera m_editorCamera;

    // View port Data ------------------------------------
    Viewport m_viewport;
    SceneRenderer m_viewportRenderer, m_miniViewportRenderer;

    // Single Instance -----------------------------------
    static KreatorLayer* s_instance;
  };
} // namespace Kreator
