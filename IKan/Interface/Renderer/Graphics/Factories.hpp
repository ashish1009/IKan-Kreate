//
//  Factories.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

class GLFWwindow;

namespace IKan
{
  class RendererContext;

  /// This is the Renderer Context Factory
  struct RendererContextFactory
  {
    /// This Function creates the renderer Context based on the renderer API supported by the Engine
    /// - Parameter windowPtr: GLFW Window pointer
    [[nodiscard]] static Scope<RendererContext> Create(GLFWwindow* windowPtr);
  };
} // namespace IKan
