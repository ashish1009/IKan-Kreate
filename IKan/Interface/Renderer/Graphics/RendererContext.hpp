//
//  RendererContext.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Renderer/Graphics/Factories.hpp"
#include "Renderer/Graphics/Specifications.hpp"

namespace IKan
{
  /// This class is the abstract class for creating Open GL Renderer Buffer
  class RendererContext
  {
  public:
    /// Default Virtual Destructor for Rendere Context
    virtual ~RendererContext() = default;
    /// This function swaps the graphics buffer. This should be called each frame
    virtual void SwapBuffers() = 0;
  };
} // namespace IKan
