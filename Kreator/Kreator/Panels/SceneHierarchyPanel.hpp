//
//  SceneHierarchyPanel.hpp
//  Kreator
//
//  Created by Ashish . on 02/08/24.
//

#pragma once

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
    
  private:
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This function render the hierachy of scene
    void RenderHierarchy();
    /// This funciton renders the Entity Hierarchy
    /// - Parameters:
    ///   - entity entity handle
    ///   - searchFilter swearch filter
    void DrawEntityNode(Entity entity, const std::string& searchFilter);

    // Member Variable ---------------------------------------------------------------------------------------------
    Ref<Scene> m_context;
    bool m_isWindow;
        
    inline static Ref<Texture> s_pencilIcon, s_plusIcon, s_gearIcon, s_reloadIcon, s_closeEyeIcon, s_EyeIcon;
  };
} // namespace Kreator
