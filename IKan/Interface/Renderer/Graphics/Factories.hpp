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
  class Texture;
  class VertexBuffer;

  /// This structure stores the API to create the renderer context instance based on the renderer API supported by the Engine
  struct RendererContextFactory
  {
    /// This Function creates the renderer context instance based on the renderer API supported by the Engine
    /// - Parameter windowPtr: GLFW Window pointer
    [[nodiscard]] static Scope<RendererContext> Create(GLFWwindow* windowPtr);
  };
  
  /// This structure stores the API to creates the renderer API instance based on the renderer API supported by the Engine
  struct RendererAPIFactory
  {
    /// This function creates the Renderer API instance based on the Supported APIs
    [[nodiscard]] static Scope<RendererAPI> Create();
  };
  
  /// This structure stores the API to create the texture instance based on the renderer API supported by the Engine
  struct TextureFactory
  {
    /// This static function creates the texture instance with specification based on the current Supported API
    /// - Parameter spec: Texture specification
    [[nodiscard]] static Ref<Texture> Create(const Texture2DSpecification& spec);
    /// This API creates the Texture instance with data based on the current Supported API
    /// - Parameter data : White data
    [[nodiscard]] static Ref<Texture> Create(uint32_t data = 0xffffffff);
    /// This API creates the Texture instance with white data based on the current Supported API
    [[nodiscard]] static Ref<Texture> CreateWhiteTexture();
  };
  
  /// This structure stores the API to create the vertex buffer instance based on the renderer API supported by the Engine
  struct VertexBufferFactory
  {
    /// This function creates the vertex buffer. Buffer data is passed and stored as static in GPU
    /// - Parameters:
    ///   - data: Data of vertex Buffer
    ///   - size: Size of vertex Buffer
    [[nodiscard]] static Ref<VertexBuffer> Create(void* data, uint32_t size);
    /// This function creates the vertex buffer. Buffer size is passed to reserve the memory in GPU. Data to passed
    /// later dynamically
    /// - Parameter size: Size of vertex buffer
    [[nodiscard]] static Ref<VertexBuffer> Create(uint32_t size);
  };

} // namespace IKan
