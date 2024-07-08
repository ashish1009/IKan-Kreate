//
//  LayerStack.cpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#include "LayerStack.hpp"

namespace IKan
{
  LayerStack::LayerStack()
  : VectorIterator<Layer*>(m_layers)
  {
  }
  
  LayerStack::~LayerStack()
  {
    IK_PROFILE();
    if (m_layers.size())
    {
      IK_LOG_TRACE(LogModule::Layers, "Deleting all Layers from Layerstack");
    }
    
    // Delete and deteach all the layers from stack
    for (Layer* layer : m_layers)
    {
      IK_LOG_TRACE(LogModule::Layers, "  Removing the {0} Layer from the stack", layer->GetName().data());
      layer->OnDetach();
    }
    m_layers.erase(m_layers.cbegin(), m_layers.cend());
  }
  
  void LayerStack::PushLayer(Layer* layer)
  {
    IK_PROFILE();
    if (!layer or std::find(m_layers.begin(), m_layers.end(), layer) != m_layers.end())
    {
      return;
    }
    
    m_layers.emplace(m_layers.begin() + m_layerInsertIndex++, layer);
    m_totalLayers++;
    
    IK_LOG_TRACE(LogModule::Layers, "Pushing the {0} Layer in the stack at position {1}. Total Layers added {2}", 
                 layer->GetName().data(), m_layerInsertIndex, m_totalLayers);
    layer->OnAttach();
  }
  
  void LayerStack::PopLayer(Layer* layer)
  {
    IK_PROFILE();
    if (!layer)
    {
      return;
    }
    
    auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, layer);
    if (it != m_layers.begin() + m_layerInsertIndex)
    {
      layer->OnDetach();
      m_layers.erase(it);
      m_layerInsertIndex--;
      m_totalLayers--;
    }
    IK_LOG_TRACE(LogModule::Layers, "Poping the {0} Layer in the stack from the index {1}. Total Layers left {2}", 
                 layer->GetName().data(), m_layerInsertIndex, m_totalLayers);
  }
  
  void LayerStack::PushOverlay(Layer* layer)
  {
    IK_PROFILE();
    if (!layer or std::find(m_layers.begin(), m_layers.end(), layer) != m_layers.end())
    {
      return;
    }
    
    m_layers.emplace_back(layer);
    
    m_totalLayers++;
    IK_LOG_TRACE(LogModule::Layers, "Pushing the {0} Layer in the stack at the end. Total Layers added {1}", 
                 layer->GetName().data(), m_totalLayers);
    
    layer->OnAttach();
  }
  
  void LayerStack::PopOverlay(Layer* layer)
  {
    IK_PROFILE();
    if (!layer)
    {
      return;
    }
    
    // Search the layer from the stack the detach/delete it
    if (auto it = std::find(m_layers.begin(), m_layers.end(), layer); it != m_layers.end())
    {
      layer->OnDetach();
      m_layers.erase(it);
      m_totalLayers--;
    }
    
    IK_LOG_TRACE(LogModule::Layers, "Poping the {0} Layer in the stack from the end. Total Layers left {1}", 
                 layer->GetName().data(), --m_totalLayers);
  }
} // namespace IKan
