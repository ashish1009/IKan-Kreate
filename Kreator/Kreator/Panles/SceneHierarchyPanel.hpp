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

    // Member Variable ---------------------------------------------------------------------------------------------
    Ref<Scene> m_context;
    bool m_isWindow;
    
    struct SelectionContext
    {
      std::vector<Entity> selections;
      bool Find(Entity entity)
      {
        for (const auto& e : selections)
        {
          if (e == entity)
          {
            return true;
          }
        }
        return false;
      }
      void Erase(Entity entity)
      {
        auto it = std::find(selections.begin(), selections.end(), entity);
        if (it != selections.end())
        {
          selections.erase(it);
        }
      }
      void PushBack(Entity entity)
      {
        selections.push_back(entity);
      }
      void Clear()
      {
        selections.clear();
      }
      Entity& operator[](size_t index)
      {
        return selections[index];
      }
      Entity& At(size_t index)
      {
        return selections.at(index);
      }
      size_t Size() const
      {
        return selections.size();
      }
    };
    SelectionContext m_selectionContext;
    
    std::function<void(Entity)> m_selectionChangedCallback, m_entityDeletedCallback;

    inline static Ref<Image> s_pencilIcon;
    inline static Ref<Image> s_plusIcon;
    inline static Ref<Image> s_gearIcon;
  };
} // namespace Kreator
