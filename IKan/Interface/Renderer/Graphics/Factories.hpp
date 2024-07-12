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
  class IndexBuffer;
  class Shader;
  class Pipeline;
  
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
    /// This function creates the vertex buffer based on the renderer API supported by the Engine. Buffer data is passed
    /// and stored as static in GPU
    /// - Parameters:
    ///   - data: Data of vertex Buffer
    ///   - size: Size of vertex Buffer
    [[nodiscard]] static Ref<VertexBuffer> Create(void* data, uint32_t size);
    /// This function creates the vertex buffer based on the renderer API supported by the Engine. Buffer size is passed
    /// to reserve the memory in GPU. Data to passed later dynamically
    /// - Parameter size: Size of vertex buffer
    [[nodiscard]] static Ref<VertexBuffer> Create(uint32_t size);
  };
  
  /// This structure stores the API to create the index buffer instance based on the renderer API supported by the Engine
  struct IndexBufferFactory
  {
    /// This function create the Index Buffer with Indices data and its size based on the renderer API supported by the Engine
    /// - Parameters:
    ///   - data: data to be filled in index buffer
    ///   - size: size of data in index buffer
    [[nodiscard]] static Ref<IndexBuffer> CreateWithSize(void* data, uint32_t size);
    /// This function create the Index Buffer with Indices data and number of indices based on the renderer API supported by the Engine
    /// - Parameters:
    ///   - data: data to be filled in index buffer
    ///   - count: number of indices in index buffer
    /// - Note: Size of single indices is taken as size of uint32_t (4)
    [[nodiscard]] static Ref<IndexBuffer> CreateWithCount(void* data, uint32_t count);
  };

  /// This structure stores the API to create the shader instance based on the renderer API supported by the Engine
  struct ShaderFactory
  {
    /// This function creates the shader instance with file path based on the renderer API supported by the Engine
    /// - Parameter shaderFilePath: Shader Code file path
    [[nodiscard]] static Ref<Shader> Create(const std::filesystem::path& shaderFilePath);
  };
  
  /// This structure stores the API to create the pipeline instance based on the renderer API supported by the Engine
  struct PipelineFactory
  {
    /// This function creates the shader instance with specification data based on the renderer API supported by the Engine
    /// - Parameter spec: Pipeline specification data
    [[nodiscard]] static Ref<Pipeline> Create(const PipelineSpecification& spec);
  };
} // namespace IKan
