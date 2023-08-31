//
//  LayerStack.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "LayerStack.hpp"

namespace IKan
{
  LayerStack::~LayerStack()
  {
    IK_LOG_WARN(LogModule::LayerStack, "Destroying Layerstack. Deleting all Layers from Layerstack");
    
    // Delete and deteach all the layers from stack
    for (auto& layer : m_layers)
    {
      IK_LOG_WARN(LogModule::LayerStack, "Destroying the {0} Layer from the stack", layer->GetName().c_str());
      layer->OnDetach();
    }
    m_layers.erase(m_layers.cbegin(), m_layers.cend());
  }
  
  void LayerStack::PushLayer(const Ref<Layer>& layer)
  {
    if (std::find(m_layers.begin(), m_layers.end(), layer) != m_layers.end())
    {
      return;
    }
    m_layerInsertIndex++;
    IK_LOG_TRACE(LogModule::LayerStack, "Pushing the {0} Layer in the stack at position {1}. Total Layers added {2}",
                 layer->GetName().c_str(), m_layerInsertIndex, m_totalLayers);

    m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
    m_totalLayers++;
    layer->OnAttach();
  }
  
  void LayerStack::PopLayer(const Ref<Layer>& layer)
  {
    auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, layer);
    if (it != m_layers.begin() + m_layerInsertIndex)
    {
      layer->OnDetach();
      m_layers.erase(it);
      m_layerInsertIndex--;
      m_totalLayers--;
    }
    
    IK_LOG_WARN(LogModule::LayerStack, "Poping the {0} Layer in the stack from the index {1}. Total Layers left {2}",
                 layer->GetName().c_str(), m_layerInsertIndex, m_totalLayers);
  }
  
  void LayerStack::PushOverlay(const Ref<Layer>& layer)
  {
    if (std::find(m_layers.begin(), m_layers.end(), layer) != m_layers.end())
    {
      return;
    }
    m_totalLayers++;
    IK_LOG_TRACE(LogModule::LayerStack, "Pushing the {0} Layer in the stack at the end. Total Layers added {1}",
                 layer->GetName().c_str(), m_totalLayers);

    m_layers.emplace_back(layer);
    layer->OnAttach();    
  }
  
  void LayerStack::PopOverlay(const Ref<Layer>& layer)
  {
    // Search the layer from the stack the detach/delete it
    if (auto it = std::find(m_layers.begin(), m_layers.end(), layer); it != m_layers.end())
    {
      layer->OnDetach();
      m_layers.erase(it);
      m_totalLayers--;
    }
    
    IK_LOG_WARN(LogModule::LayerStack, "Poping the {0} Layer in the stack from the end. Total Layers left {1}",
                 layer->GetName().c_str(), --m_totalLayers);
  }
} // namespace IKan
