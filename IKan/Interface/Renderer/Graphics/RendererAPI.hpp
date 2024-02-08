//
//  RendererAPI.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Graphics/Factories.hpp"
#include "Renderer/Graphics/Specifications.h"

namespace IKan
{
  /// This class is the interface for all the renderer APIs
  class RendererAPI
  {
  public:
    /// This is the default virtual destrutctor
    virtual ~RendererAPI() = default;
  };
} // namespace IKan
