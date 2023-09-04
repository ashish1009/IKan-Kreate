//
//  Scene.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include <entt/entt.hpp>

#include "Core/TimeStep.hpp"
#include "Camera/EditorCamera.hpp"
#include "Scene/Component.hpp"
#include "Asset/Asset.hpp"

namespace IKan
{
  class SceneRenderer;  
  static const std::string SceneExtension = ".ikscene";

  class Scene : public Asset
  {
  public:
    /// This is the default constructor of EnTT Scene
    /// - Parameters:
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    Scene(const std::string& name = "UntitledScene", uint32_t maxEntityCapacity = 200000);
    /// This is the default destructor of EnTT Scene
    ~Scene();

    /// This function creates the instance of EnTT Scene
    /// - Parameters:
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    static Ref<Scene> Create(const std::string& name = "UntitledScene", uint32_t maxEntityCapacity = 200000);

    // Setters -----------------------------------------------------------------------------------------------------
    /// This function sets the scene name
    /// - Parameter name: scene name
    void SetName(const std::string& name);
    
    // Getters -----------------------------------------------------------------------------------------------------
    /// This function returns the scene name
    const std::string& GetName() const;

    ASSET_TYPE(Scene);
    
  private:
    // Member Variables ---------------------------------------------------------------------------------------------
    // Scene Utils ----------------------
    std::string m_name = "";
    uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
  };
} // namespace IKan
