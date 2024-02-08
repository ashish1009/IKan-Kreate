//
//  Shader.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Factories.hpp"
#include "Renderer/Graphics/Specifications.h"

namespace IKan
{
#ifdef __APPLE__
  /// Maximum Texture slot supported by Senderer Shader Current Open GL Version 4.1 supports only 16 Texture slot in Shader
  static constexpr uint32_t MaxTextureSlotsInShader = 16;
#else
  static constexpr uint32_t MaxTextureSlotsInShader = 0;
#endif
  
  /// Interface class for storing Shader
  class Shader
  {
  public:
    /// Default virtual destructor
    virtual ~Shader() = default;
  };
} // namespace IKan
