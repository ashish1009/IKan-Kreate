//
//  Scene.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Assets/Asset.hpp"

namespace IKan
{
  /// This class stores the scene data (Entity) for renderer
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

    // Getters ------------------------------------------------------------------------------------------------------
    /// This function returns the scene name
    const std::string& GetName() const;

    // Static APIs --------------------------------------------------------------------------------------------------
    /// This function creates the instance of EnTT Scene
    /// - Parameters:
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    static Ref<Scene> Create(const std::string& name = "UntitledScene", uint32_t maxEntityCapacity = 200000);

    ASSET_TYPE(Scene);
    
  private:
    // Member Variables ---------------------------------------------------------------------------------------------
    // Scene Utils ----------------------
    std::string m_name {};
    uint32_t m_viewportWidth {0}, m_viewportHeight {0};

    // EnTT -----------------------------
    uint32_t m_registryCapacity {0};
  };
} // namespace IKan
