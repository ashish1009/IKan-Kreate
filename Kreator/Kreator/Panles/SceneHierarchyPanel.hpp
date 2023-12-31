//
//  SceneHierarchyPanel.hpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#pragma once

#include "EntitySelectionContext.hpp"

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
    virtual void OnProjectChanged(const Ref<Project>& project) override;
    /// @see Editor Panel
    virtual void OnImGuiRender(bool& isOpen) override;
    
    /// This funciton initialise the scene hierarchy panel
    static void Initialize();
    /// This funciton shutdown the scene hierarchy panel
    static void Shutdown();
    
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
    void SetSelectedEntity(Entity entity, bool multipleSelection = false);

  private:
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This function render the hierachy of scene
    void RenderHierarchy();
    /// This function draw selected component
    /// - Parameter entity: entity handle
    void DrawComponents(Entity entity);
    /// This funciton renders the Entity Hierarchy
    /// - Parameters:
    ///   - entity entity handle
    ///   - searchFilter swearch filter
    void DrawEntityNode(Entity entity, const std::string& searchFilter);
    /// This function create menue for entiyt
    void DrawCreateEntityMenu(Entity parent = {});
    /// This function shows the add component popup
    void AddComponentPopup();

    /// This function search the entity string recursivly for childs too
    /// - Parameters:
    ///   - entity: entity handle
    ///   - searchFilter: search filter
    ///   - maxSearchDepth: max depth
    bool TagSearchRecursive(Entity entity, std::string_view searchFilter, uint32_t maxSearchDepth, uint32_t currentDepth = 1);

    /// This function is the callback for external entity destroy
    /// - Parameter entity entity handle
    void OnExternalEntityDestroyed(Entity entity);
    /// This function is the callback for external entity destroy
    /// - Parameter entity entity handle
    void OnEntityDestroyed(Entity entity);
    
    /// This function wraps to create mesh asset
    /// - Parameter filePath: file path
    AssetHandle CreateMeshAsset(const std::string& filePath);
    
    // Member Variable ---------------------------------------------------------------------------------------------
    Ref<Scene> m_context;
    bool m_isWindow;
    
    SelectionContext m_selectionContext;    
    std::function<void(SelectionContext)> m_selectionChangedCallback, m_entityDeletedCallback;
    
    inline static Ref<MaterialAsset> s_currentOpenedMaterialAsset;

    inline static Ref<Image> s_pencilIcon;
    inline static Ref<Image> s_plusIcon;
    inline static Ref<Image> s_gearIcon;
  };
} // namespace Kreator
