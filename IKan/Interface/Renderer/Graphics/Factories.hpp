//
//  Factories.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Graphics/Specifications.h"

class GLFWwindow;

namespace IKan
{
  class RendererContext;
  class RendererAPI;
  class Shader;
  class VertexBuffer;
  class IndexBuffer;
  class Pipeline;
  class Texture;

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
  
  /// This structure is the factory to create frame buffer based on the current API supported
  struct ShaderFactory
  {
    /// This static function creates the Shader instance based on the current Supported API
    /// - Parameter shaderFilePath: Shader Code file path
    [[nodiscard]] static Ref<Shader> Create(const std::filesystem::path& shaderFilePath);
  };
  
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
  
  struct IndexBufferFactory
  {
    /// This function create the Index Buffer with Indices data and its size
    /// - Parameters:
    ///   - data: data to be filled in index buffer
    ///   - size: size of data in index buffer
    [[nodiscard]] static Ref<IndexBuffer> CreateWithSize(void* data, uint32_t size);
    /// This function create the Index Buffer with Indices data and number of indices
    /// - Parameters:
    ///   - data: data to be filled in index buffer
    ///   - count: number of indices in index buffer
    /// - Note: Size of single indices is taken as size of uint32_t (4)
    [[nodiscard]] static Ref<IndexBuffer> CreateWithCount(void* data, uint32_t count);
  };
  
  /// This structure is the factory to create frame buffer based on the current API supported
  struct PipelineFactory
  {
    /// This static function creates the Pipeline instance based on the current Supported API
    /// - Parameter spec: Pipeline specificaion
    [[nodiscard]] static Ref<Pipeline> Create(const PipelineSpecification& spec);
  };

  /// This structure is the factory to create frame buffer based on the current API supported
  struct TextureFactory
  {
    /// This static function creates the Texture instance based on the current Supported API
    /// - Parameter spec: Texture specificaion
    [[nodiscard]] static Ref<Texture> Create(const Texture2DSpecification& spec);
    /// This API creates the Texture instance with white based on the Renderer API
    /// - Parameter data : White data
    [[nodiscard]] static Ref<Texture> Create(uint32_t data = 0xffffffff);
    /// This static functions creates the Texture from image file
    /// - Parameter filePath: path of texture file
    /// - Note: Use Renderer::GetTexture() to reuse already loaded texture (if following texture is already used
    ///         somewhere). This API will Load the texture again and new memory will be creted. So to avoid duplicate
    ///         memory for same texture use Library::GetTexture()
    [[nodiscard]] static Ref<Texture> Create(const std::filesystem::path& filePath);
    /// This static functions creates the Texture from image file
    /// - Parameter spec: Texture specificaion
    /// - Note: Use Renderer::GetTexture() to reuse already loaded texture (if following texture is already used
    ///         somewhere). This API will Load the texture again and new memory will be creted. So to avoid duplicate
    ///         memory for same texture use Library::GetTexture()
    [[nodiscard]] static Ref<Texture> Create(const ImageSpecificaion& spec);
  };

} // namespace IKan
