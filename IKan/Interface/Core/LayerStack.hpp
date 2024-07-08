//
//  LayerStack.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Core/Layer.hpp"

namespace IKan
{
  /// This class stores all the layer pointers in stack to execute them in application
  /// - Important: This class do not owns the pointer, so the Layer must not be destroyed withing game loop.
  /// - Note: If Layer need to be destroy then Pop the layer from the stack before destroying the object
  class LayerStack : public VectorIterator<Layer*>
  {
  public:
    // Constructor and Destructor ----------------------------------------------------------------------------------
    /// This constructor creates the instance of layer stack
    LayerStack();
    /// This destructor destroys the layer stack instance. Delete all the layers in the stack and detach them from application
    virtual ~LayerStack();
    
    // Fundamental APIs -------------------------------------------------------------------------------------------
    /// This function inserts the layer in the vector at the last inserted layer index (where last layer was pushed)
    /// - Note: Layer::OnAttach() will be called...
    /// - Parameter layer: Pointer of layer to be inserted
    void PushLayer(Layer* layer);
    /// This function deletes the layer from the vector from the last inserted layer index
    /// - Note: Layer::OnDetach() will be called...
    /// - Parameter layer: Pointer of layer to be deleted
    void PopLayer(Layer* layer);
    /// This function inserts the layer in the vector at last position (End)
    /// - Note: Layer::Attach() will be called...
    /// - Parameter layer: Pointer of layer to be inserted
    void PushOverlay(Layer* layer);
    /// This function deletes the layer from the vector from the last postion (End)
    /// - Note: Layer::OnDetach() will be called...
    /// - Parameter layer: Pointer of layer to be deleted
    void PopOverlay(Layer* layer);
    
  private:
    uint32_t m_layerInsertIndex {0};
    uint32_t m_totalLayers {0};
    std::vector<Layer*> m_layers;
  };
} // namespace IKan
