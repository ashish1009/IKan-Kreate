//
//  SceneHierarchyPanel.hpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#pragma once

namespace Kreator
{
  /// Scene Hierarchy panel for entities
  class SceneHierarchyPanel : public EditorPanel
  {
  public:
    /// Default Constructor
    SceneHierarchyPanel() = default;
    /// Constructor with scene instance
    /// - Parameter scene: Scene context
    SceneHierarchyPanel(const Ref<Scene>& scene, bool isWindow = true);

    /// @see Editor Panel
    virtual void SetSceneContext(const Ref<Scene>& scene) override;
    /// @see Editor Panel
    virtual void OnImGuiRender(bool& isOpen) override;

    /// This funciton initialise the scene hierarchy panel
    static void Init();
    /// This funciton shutdown the scene hierarchy panel
    static void Shutdown();
    
    /// This function sets the entity set callback
    /// - Parameter func: function pointer
    void SetSelectionChangedCallback(const std::function<void(Entity)>& func);
    /// This function sets the entity delete callback
    /// - Parameter func: function pointer
    void SetEntityDeletedCallback(const std::function<void(Entity)>& func);

    /// This function update the selected entity
    /// - Parameter entity: entity
    void SetSelectedEntity(Entity entity);

  private:
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This function is the callback for external entity destroy
    /// - Parameter entity entity handle
    void OnExternalEntityDestroyed(Entity entity);

    // Member Variable ---------------------------------------------------------------------------------------------
    Ref<Scene> m_context;
    Entity m_selectionContext;
    bool m_isWindow;
    
    std::function<void(Entity)> m_selectionChangedCallback, m_entityDeletedCallback;
  };
} // namespace Kreator
