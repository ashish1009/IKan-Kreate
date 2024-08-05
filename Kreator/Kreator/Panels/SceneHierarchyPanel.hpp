//
//  SceneHierarchyPanel.hpp
//  Kreator
//
//  Created by Ashish . on 02/08/24.
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
    /// - Parameters:
    ///   - scene: Scene context
    ///   - isWindow: Is panel primary widget
    SceneHierarchyPanel(const Ref<Scene>& scene, bool isWindow = true);
    
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
    /// This funciton renders the Entity Hierarchy
    /// - Parameters:
    ///   - entity entity handle
    ///   - searchFilter swearch filter
    void DrawEntityNode(Entity entity, const std::string& searchFilter);
    /// This function draw selected component
    /// - Parameter entity: entity handle
    void DrawComponents(Entity entity);

    /// This function search the entity string recursivly for childs too
    /// - Parameters:
    ///   - entity: entity handle
    ///   - searchFilter: search filter
    ///   - maxSearchDepth: max depth
    bool SearchEntityRecursive(Entity entity, const std::string_view& searchFilter, const uint32_t maxSearchDepth, uint32_t currentDepth = 1);
    /// This funcion recusively checks if any of the child is selected
    /// - Parameter entity: entity
    bool IsChildSelected(Entity entity);

    /// This function is the callback for external entity destroy
    /// - Parameter entity entity handle
    void OnExternalEntityDestroyed(const Entity& entity);
    /// This function is the callback for external entity destroy
    /// - Parameter entity entity handle
    void OnEntityDestroyed(Entity entity);

    /// This function draw entity selector
    /// - Parameters:
    ///   - title: title of property
    ///   - entityID: Entity ID
    ///   - outID: output ID
    void DrawEntitySelector(const char* title, UUID entityID, UUID& outID, const UI::PropertyAssetReferenceSettings& settings = {});

    /// This function shows the add component popup
    void AddComponentPopup();

    // Member Variable ---------------------------------------------------------------------------------------------
    Ref<Scene> m_context;
    bool m_isWindow;

    // Entity Data
    SelectionContext m_selectionContext;
    std::function<void(SelectionContext)> m_selectionChangedCallback, m_entityDeletedCallback;

    // Material editor
    bool m_showMaterialEditor = false;
    Ref<MaterialAsset> m_selectedMeshMaterialAsset;

    inline static Ref<Image> s_pencilIcon, s_plusIcon, s_gearIcon, s_reloadIcon, s_closeEyeIcon, s_EyeIcon;
  };
} // namespace Kreator
