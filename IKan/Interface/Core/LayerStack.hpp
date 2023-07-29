//
//  LayerStack.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Core/Layer.hpp"

namespace IKan
{
  class LayerStack
  {
  public:
    // Constructor and Destructor ----------------------------------------------------------------------------------
    /// This is the default Layer stack constructor
    LayerStack() = default;
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
    
    // Iterators ---------------------------------------------------------------------------------------------------
    std::vector<Ref<Layer>>::iterator begin() { return m_layers.begin(); }
    std::vector<Ref<Layer>>::iterator end() { return m_layers.end(); }
    std::vector<Ref<Layer>>::reverse_iterator rbegin() { return m_layers.rbegin(); }
    std::vector<Ref<Layer>>::reverse_iterator rend() { return m_layers.rend(); }
    
    std::vector<Ref<Layer>>::const_iterator begin() const { return m_layers.begin(); }
    std::vector<Ref<Layer>>::const_iterator end() const { return m_layers.end(); }
    std::vector<Ref<Layer>>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
    std::vector<Ref<Layer>>::const_reverse_iterator rend() const { return m_layers.rend(); }
    
    DELETE_COPY_MOVE_CONSTRUCTORS(LayerStack);
  private:
    uint32_t m_layerInsertIndex = 0;
    uint32_t m_totalLayers = 0;
    std::vector<Ref<Layer>> m_layers;
  };
} // namespace IKan
