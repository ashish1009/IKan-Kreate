//
//  RendererContext.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

#include <GLFW/glfw3.h>

namespace IKan
{
  class RendererContext
  {
  public:
    /// Default Virtual Destructor for Rendere Context
    virtual ~RendererContext() = default;
    /// This function swaps the graphics buffer. This should be called each frame
    virtual void SwapBuffers() = 0;
    
    /// This Function creates the renderer Context based on the renderer API supported by the Engine
    /// - Parameter window_ptr: GLFW Window pointer
    [[nodiscard]] static Scope<RendererContext> Create(GLFWwindow* window_ptr);
  };
} // namespace IKan
