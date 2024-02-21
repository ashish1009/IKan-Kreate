//
//  Scene.hpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#pragma once

#include <entt/entt.hpp>

#include "Asset/Asset.hpp"

namespace IKan
{
  static const std::string SceneExtension = ".ikscene";

  class Scene : public Asset
  {
  public:
    /// This is the default constructor of EnTT Scene
    /// - Parameters:
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    Scene(const std::string& name, uint32_t maxEntityCapacity);
    /// This is the default destructor of EnTT Scene
    ~Scene();
    
    /// This function closes the current scene
    void OnClose();

    // Getters -----------------------------------------------------------------------------------------------------
    /// This function returns the scene name
    const std::string& GetName() const;

    /// This function creates the instance of EnTT Scene
    /// - Parameters:
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    static Ref<Scene> Create(const std::string& name = "UntitledScene", uint32_t maxEntityCapacity = 200000);

    ASSET_TYPE(Scene);
    
    DELETE_COPY_MOVE_CONSTRUCTORS(Scene);

  private:
    // Member Variables ---------------------------------------------------------------------------------------------
    // Scene Utils ----------------------
    std::string m_name {};
    uint32_t m_viewportWidth {0}, m_viewportHeight {0};
    
    // EnTT -----------------------------
    entt::registry m_registry;
    uint32_t m_registryCapacity = 0;
    uint32_t m_numEntities = 0;
    int32_t m_maxEntityID = -1;
  };
} // namespace IKan
