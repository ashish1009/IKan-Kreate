//
//  EditorPanel.hpp
//  IKan
//
//  Created by Ashish . on 04/08/23.
//

#pragma once

#include "Event/Event.h"
#include "Project/Project.hpp"
#include "Scene/Scene.hpp"

namespace IKan
{
  /// Base Editor Panel class
  class EditorPanel
  {
  public:
    /// Default Editor Panel Destructor
    virtual ~EditorPanel() = default;
    
    /// This function render the Panel as GUI
    virtual void OnImguiRender(bool& is_open) = 0;
    /// This function hnadles all the events in panel
    virtual void OnEvent(Event& e) {}
    /// This function handles the project change for editor panel
    /// - Parameter project: Project instance
    virtual void OnProjectChanged(const Ref<Project>& project) {}
    /// This function handles the scene change for editor panel
    /// - Parameter context: scene context
    virtual void SetSceneContext(const Ref<Scene>& context){}
  };
} // namespace IKan
