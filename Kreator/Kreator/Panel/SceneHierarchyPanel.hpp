//
//  SceneHierarchyPanel.hpp
//  Kreator
//
//  Created by Ashish . on 22/02/24.
//

#pragma once

#include "Editor/SelectionContext.hpp"

namespace Kreator
{
  /// Scene Hierarchy panel for entities
  class SceneHierarchyPanel : public EditorPanel
  {
  public:
    /// This funciton initialise the scene hierarchy panel
    static void Initialize();
    /// This funciton shutdown the scene hierarchy panel
    static void Shutdown();
    
    /// Default Constructor
    SceneHierarchyPanel() = default;
    /// Constructor with scene instance
    /// - Parameter scene: Scene context
    SceneHierarchyPanel(const Ref<Scene>& scene);
    
    /// @see Editor Panel
    virtual void SetSceneContext(const Ref<Scene>& scene) override;
    /// @see Editor Panel
    virtual void OnProjectChanged(const Ref<Project>& project) override;
    /// @see Editor Panel
    virtual void OnImGuiRender(bool& isOpen) override;
    
    /// This function sets the entity set callback
    /// - Parameter func: function pointer
    void SetSelectionChangedCallback(const std::function<void(SelectionContext)>& func);
    /// This function sets the entity delete callback
    /// - Parameter func: function pointer
    void SetEntityDeletedCallback(const std::function<void(SelectionContext)>& func);
    /// This function update the selected entity
    /// - Parameters:
    ///   - entity: entity
    ///   - multipleSelection: multiple Selection falase
    void SetSelectedEntity(const Entity& entity, bool multipleSelection = false);

  private:
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This function render the hierachy of scene
    void RenderHierarchy();
    /// This function draw selected component
    /// - Parameter entity: entity handle
    void DrawComponents(Entity entity);

    /// This function is the callback for external entity destroy
    /// - Parameter entity entity handle
    void OnExternalEntityDestroyed(const Entity& entity);
    /// This function is the callback for external entity destroy
    /// - Parameter entity entity handle
    void OnEntityDestroyed(Entity entity);

    // Member Variable ---------------------------------------------------------------------------------------------
    Ref<Scene> m_context;
    
    SelectionContext m_selectionContext;
    Entity m_lastSelectedEntity;
    std::function<void(SelectionContext)> m_selectionChangedCallback, m_entityDeletedCallback;

    inline static Ref<Texture> s_pencilIcon, s_plusIcon, s_gearIcon, s_reloadIcon;
  };
} // namespace Kreator
