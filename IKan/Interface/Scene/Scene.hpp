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
    ASSET_TYPE(Scene);
  };
} // namespace IKan
