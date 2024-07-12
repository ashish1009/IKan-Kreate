//
//  Texture.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Factories.hpp"

namespace IKan
{
  /// This class is the interface for creating texture with specification data
  class Texture 
  {
  public:
    /// Default virtual destructor
    virtual ~Texture() = default;
  };
} //namespace IKan
