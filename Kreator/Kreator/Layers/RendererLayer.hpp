//
//  RendererLayer.hpp
//  Kreator
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

using namespace IKan;

namespace Kreator
{
  /// This class is the Renderer layer for Kreator
  class RendererLayer : public Layer
  {
  public:
    /// This Constructor instantiate the Renderer Layer
    RendererLayer();
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
    // Memeber variables ----------------------------------------------------------------------------------------------
    // Editor Data ----------------
    EditorCamera m_editorCamera;
    
    // Single Instance -----------------------------------
    static RendererLayer* s_instance;
  };
} // namespace Kreator
