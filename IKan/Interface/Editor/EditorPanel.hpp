//
//  EditorPanel.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Events/Events.h"
#include "Project/Project.hpp"
#include "Scene/Scene.hpp"

namespace IKan
{
  /// This class is the base for creating editor Panels
  class EditorPanel
  {
  public:
    /// Default Editor Panel Destructor
    virtual ~EditorPanel() = default;
    
    /// This function renders the Panel as GUI
    virtual void OnImGuiRender(bool& isOpen) = 0;
    /// This function hnadles all the events in panel
    virtual void OnEvent([[maybe_unused]] Event& e) {}
    /// This function handles the project change for editor panel
    /// - Parameter project: Project instance
    virtual void OnProjectChanged([[maybe_unused]] const Ref<Project>& project) {}
    /// This function handles the scene change for editor panel
    /// - Parameter context: scene context
    virtual void SetSceneContext([[maybe_unused]] const Ref<Scene>& context){}
  };
} // namespace IKan
