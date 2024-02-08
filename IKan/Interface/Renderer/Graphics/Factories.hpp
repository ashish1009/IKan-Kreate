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
  class RendererAPI;
  class Shader;
  class VertexBuffer;

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

} // namespace IKan
