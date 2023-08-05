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
    static void Initialize();
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
    /// This function render the hierachy of scene
    void RenderHierarchy();
    /// This funciton renders the Entity Hierarchy
    /// - Parameters:
    ///   - entity entity handle
    ///   - searchFilter swearch filter
    void DrawEntityNode(Entity entity, const std::string& searchFilter);
    /// This function create menue for entiyt
    void DrawEntityCreateMenu(Entity parent = {});

    /// This function is the callback for external entity destroy
    /// - Parameter entity entity handle
    void OnEntityDestroyed(Entity entity);

    /// This function search the entity string recursivly for childs too
    /// - Parameters:
    ///   - entity: entity handle
    ///   - searchFilter: search filter
    ///   - maxSearchDepth: max depth
    bool TagSearchRecursive(Entity entity, std::string_view searchFilter, uint32_t maxSearchDepth, uint32_t currentDepth = 1);

    // Member Variable ---------------------------------------------------------------------------------------------
    Ref<Scene> m_context;
    Entity m_selectionContext;
    bool m_isWindow;
    
    std::function<void(Entity)> m_selectionChangedCallback, m_entityDeletedCallback;
    
    inline static Ref<Image> s_pencilIcon;
    inline static Ref<Image> s_plusIcon;
    inline static Ref<Image> s_gearIcon;
  };
} // namespace Kreator
