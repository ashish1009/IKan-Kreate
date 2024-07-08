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
    /// This Function creates the renderer Context based on the renderer API supported by the Engine
    /// - Parameter windowPtr: GLFW Window pointer
    [[nodiscard]] static Scope<RendererContext> Create(GLFWwindow* windowPtr);
  };
  
  /// This is the factory class to create Renderer API instance based on the supported API
  struct RendererAPIFactory
  {
    /// This function creates the Renderer API instance based on the Supported APIs
    [[nodiscard]] static Scope<RendererAPI> Create();
  };
} // namespace IKan
