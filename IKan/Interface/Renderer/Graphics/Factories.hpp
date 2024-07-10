//
//  Factories.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Renderer/Graphics/Specifications.hpp"

// Forward Declarations
class GLFWwindow;

namespace IKan 
{
  // IKan Forward Declarations
  class RendererContext;
  class RendererAPI;

  /// This is the Renderer Context Factory
  struct RendererContextFactory
  {
    /// This Function creates the renderer context instance based on the renderer API supported by the Engine
    /// - Parameter windowPtr: GLFW Window pointer
    [[nodiscard]] static Scope<RendererContext> Create(GLFWwindow* windowPtr);
  };
  
  /// This Function creates the renderer API instance based on the renderer API supported by the Engine
  struct RendererAPIFactory
  {
    /// This function creates the Renderer API instance based on the Supported APIs
    [[nodiscard]] static Scope<RendererAPI> Create();
  };
} // namespace IKan
