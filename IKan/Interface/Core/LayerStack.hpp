//
//  LayerStack.hpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#pragma once

#include "Core/Layer.hpp"

namespace IKan
{
  class LayerStack : public VectorIterator<Ref<Layer>>
  {
  public:
    // Constructor and Destructor ----------------------------------------------------------------------------------
    /// This is the default Layer stack constructor
    LayerStack();
    /// This is the Layer stack destructor. Delete all the layers in the stack and detach them from application
    ~LayerStack();
    
    // Fundamental APIs -------------------------------------------------------------------------------------------
    /// This function inserts the layer in the vector at the last inserted layer index (where last layer was pushed)
    /// - Note: Layer::OnAttach() will be called...
    /// - Parameter layer: Layer to be inserted
    void PushLayer(const Ref<Layer>& layer);
    /// This function deletes the layer from the vector from the last inserted layer index
    /// - Note: Layer::OnDetach() will be called...
    /// - Parameter layer: Layer to be deleted
    void PopLayer(const Ref<Layer>& layer);
    /// This function inserts the layer in the vector at last position (End)
    /// - Note: Layer::Attach() will be called...
    /// - Parameter layer: Layer to be inserted
    void PushOverlay(const Ref<Layer>& layer);
    /// This function deletes the layer from the vector from the last postion (End)
    /// - Note: Layer::OnDetach() will be called...
    /// - Parameter layer: Layer to be deleted
    void PopOverlay(const Ref<Layer>& layer);
    
  private:
    uint32_t m_layerInsertIndex {0};
    uint32_t m_totalLayers {0};
    std::vector<Ref<Layer>> m_layers;
  };
} // namespace IKan
