//
//  Renderer.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

namespace IKan
{
  /// Data type of Renderer ID
  using RendererID = uint32_t;
  
  /// This enum stores the renderer Type supported by Ehgine
  enum class RendererType : uint8_t
  {
    Invalid, OpenGL
  };
  
  enum class GlDepthFunc
  {
    Always, Never, Less, Equal, LEqual, Greater, LGreater, NotEqual
  };
  
  /// Stores the capability of renderer
  struct RendererCapabilities
  {
    std::string vendor;
    std::string renderer;
    std::string version;
    
    static RendererCapabilities& Get();
  };
  
  class Pipeline;

  /// This is the Wrapper class for all Renderer APIs (User and Graphics)
  /// Major responsibilities for Renderer is
  /// - Create Renderer API based on the renderer supported by the engine
  /// - Initialize and shutdown all the user defined renderers.
  /// - Manage Shader and Texture library to store the pre loaded shader and textures
  class Renderer
  {
  public:
    // Fundamentals ---------------------------------------------------------------------------------------------------
    /// This function initializes all the renderers including user defined renderer and also setup the current Renderer
    /// API as 'api'.
    static void Initialize();
    /// This function shut down all the renderers including user defined renderer. Also destroy the instance created for
    /// any Graphics API.
    static void Shutdown();
    
    // Renderer Type --------------------------------------------------------------------------------------------------
    /// This function sets the current renderer API type
    /// - Parameter rendererType: current renderer type
    /// - Note: Should be set before Creating any Renderer Context
    static void SetCurrentRendererAPI(RendererType rendererType);
    /// This function return the current renderer API type
    static RendererType GetCurrentRendererAPI();

    // Renderer Controller --------------------------------------------------------------------------------------------

    /// This funciton Clears all the Renderer Bits and set the Clear color to new color
    /// - Parameter color: New color of renderer Clear color
    static void Clear(const glm::vec4& color);
    /// This funciton Clears all the Depth Bits
    static void ClearDepthBits();
    /// This funciton Clears all the Stencil Bits
    static void ClearStencilBits();
    /// This funciton Clears all the Bits
    static void ClearBits();
    
    /// This funciton Enables Depth Field
    static void EnableDepthPass();
    /// This funciton Disables Depth Field
    static void DisableDepthPass();
    /// This function change depth function
    /// - Parameter func: depth function type
    static void DepthFunc(GlDepthFunc func);
    
    /// This function enables the stencil pass
    static void EnableStencilPass();
    /// This function disables the stencil pass
    static void DisableStencilPass();

    /// This function update the renderer viewport size
    /// - Parameters:
    ///   - width: new width of renderer viewport
    ///   - height: new height of renderer viewport
    static void SetViewport(uint32_t width, uint32_t height);
    
    // Draw Calls ---------------------------------------------------------------------------------------------------
    /// This API draws a quad with pipeline and indexed count
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - count: number of Indices (if 0 then use index buffer of Vertex array)
    static void DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t count);
    /// This API draws a quad with pipeline and indexed count
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - count: number of Indices (if 0 then use index buffer of Vertex array)
    static void DrawIndexedStrip(const Ref<Pipeline>& pipeline, uint32_t count);
    /// This API draws Lines Vertex Array
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - vertexCount: number of Indices
    static void DrawLines(const Ref<Pipeline>& pipeline, uint32_t vertexCount);
    /// This API draws Indexed Vertex Array
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - count: number of Indices (if 0 then use index buffer of Vertex array)
    static void DrawArrays(const Ref<Pipeline>& pipeline, uint32_t count);
    /// This function render the fullscreen quad
    /// - Parameter pipeline: pipeline having vertex buffer and index buffer
    static void DrawQuad(const Ref<Pipeline>& pipeline);
    /// This function draw the index based vertex
    /// - Parameters:
    ///   - indexCount: index count
    ///   - indicesData: indices data
    ///   - baseVertex: base vertex
    static void DrawIndexedBaseVertex(uint32_t indexCount, void* indicesData, uint32_t baseVertex);
    /// This function render the fullscreen cube
    /// - Parameter pipeline: pipeline
    static void DrawFullscreenCube(const Ref<Pipeline>& pipeline);

    DELETE_ALL_CONSTRUCTORS(Renderer);
  };
} // namespace IKan
