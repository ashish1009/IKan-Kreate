//
//  Renderer.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Renderer/RenderCommandQueue.hpp"

namespace IKan
{
  /// Data type of Renderer IDs
  using RendererID = uint32_t;
  
  // Forward Declaration
  class Pipeline;

  /// This enum stores the supported renderer APIs by IKan engine
  enum class RendererType : uint8_t
  {
    Invalid, OpenGL
  };
  
  /// Stores the capability of renderer
  struct RendererCapabilities
  {
    std::string_view vendor;
    std::string_view renderer;
    std::string_view version;
    
    static RendererCapabilities& Get();
  };

  /// This class is the singleton wrapper class to manage the Renderer APIs that deal with Graphics APIs directly
  /// - Note: Set the renderer API before using any other API related to rendering or window creation (Context)
  class Renderer
  {
  public:
    // Fundamentals ---------------------------------------------------------------------------------------------------
    /// This function initializes all the renderers including user defined renderers
    static void Initialize();
    /// This function shuts down all the renderers including user defined renderer. Also destroy the instance created
    /// for any Graphics API.
    static void Shutdown();

    // Renderer API --------------------------------------------------------------------------------------------------
    /// This function sets the current renderer API type
    /// - Parameter rendererType: current renderer type
    /// - Note: Should be set before creating renderer context or using any renderer API
    static void SetCurrentRendererAPI(RendererType rendererType);
    /// This function returns the current renderer API type
    static RendererType GetCurrentRendererAPI();

    // Renderer Controller --------------------------------------------------------------------------------------------
    /// This funciton Clears all the Renderer Bits and set the Clear color to new color
    /// - Parameter color: New color of renderer Clear color
    static void Clear(const glm::vec4& color);
    /// This funciton Clears all the Color Bits
    static void ClearColorBits();
    /// This funciton Clears all the Depth Bits
    static void ClearDepthBits();
    /// This funciton Clears all the Stencil Bits
    static void ClearStencilBits();
    /// This funciton Clears all the Bits
    static void ClearBits();
    
    /// This function update the renderer viewport size
    /// - Parameters:
    ///   - width: new width of renderer viewport
    ///   - height: new height of renderer viewport
    static void SetViewport(uint32_t width, uint32_t height);

    // Draw Calls ---------------------------------------------------------------------------------------------------
    /// This API draws a quad with pipeline and indexed count
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - count: number of Indices
    static void DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t count);
    /// This API draws Lines Vertex Array
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer layout
    ///   - vertexCount: number of vertices
    static void DrawLines(const Ref<Pipeline>& pipeline, uint32_t vertexCount);
    /// This API draws Indexed Vertex Array
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer attribute
    ///   - count: number of vertices
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

    // Render Command Queue -------------------------------------------------------------------------------------------
    /// This function submits the Renderer command in Queue
    /// - Parameter func: Function pointer lambda
    template<typename FuncT> static void Submit(FuncT&& func)
    {
#if RENDER_COMMAND_QUEUE_ENABLED
      static RenderCommandQueue::RenderCommandFn renderCmd = [](void* ptr)
      {
        FuncT* pFunc = (FuncT*)ptr;
        (*pFunc)();
        
        pFunc->~FuncT();
      };
      void* storageBuffer = GetRenderCommandQueue()->Allocate(renderCmd, sizeof(func));
      new (storageBuffer) FuncT(std::forward<FuncT>(func));
#else
      func();
#endif
    }
    
    /// This funcion executes all the renderer commands in queue
    static void WaitAndRender();

    DELETE_ALL_CONSTRUCTORS(Renderer);
    
  private:
    static RenderCommandQueue* GetRenderCommandQueue();
  };
} // namespace IKan
