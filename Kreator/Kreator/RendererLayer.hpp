//
//  RendererLayer.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace Kreator
{
  class RendererLayer : public Layer
  {
  public:
    /// This is Renderer Layer's Default Constructor
    RendererLayer();
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
    // Member Variables ----------------------------------------------------------------------------------------------

    // Client Data ---------------------------------------
    std::filesystem::path m_clientResourcePath;

    // Single Instance -----------------------------------
    static RendererLayer* s_instance;
  };
} // namespace Kreator
