//
//  Renderer.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

namespace IKan
{
  // Forward Declaration
  class Shader;
  class Pipeline;

  /// All renderer ID type
  using RendererID = uint32_t;

  /// This is the wrapper class to interact with all renderer modules and Engine/Client
  /// Major responsibilities for Renderer is
  /// - Create Renderer API based on the renderer supported by the engine
  /// - Initialise and shutdown all the user defined renderers.
  /// - Manage Shader and Texture library to store the pre loaded shader an textures
  /// - Note: Apis to be called in the following orders
  ///     1. CreateRendererDate(Api)
  ///     <Create Window with Renderer Context>
  ///     2. Initialize()
  ///     <Run the Game Loop>
  ///     <Use Renderer Runtime APIs>
  ///     3. Shutdown();
  class Renderer
  {
  public:
    /// Supported APIs in IKan engine
    enum class Api : uint8_t
    {
      None, OpenGl
      // Add other supported API with time
    };
    
    /// Stores the capability of renderer
    struct Capabilities
    {
      std::string vendor;
      std::string renderer;
      std::string version;
      
      static Capabilities& Get();
    };
    
    // Fundamentals---------------------------------------------------------------------------------------------------
    /// This function Creates memory for rendere Data.
    /// - Note: To be called before initializing any Open GL or GLFW instances
    /// - Parameter api: current renderer API
    static void CreateRendererData(Api api);
    /// This function initializes all the renderers including user defined renderer and also setup the current Renderer
    /// API as 'api'.
    /// - Note: To be called after creating Open GL Rendere Context (should be created while creating GLFW window)
    static void Initialize();
    /// This function shut down all the renderers including user defined renderer. Also destroy the instance created for
    /// any Graphics API.
    static void Shutdown();

    // Getters ------------------------------------------------------------------------------------------------------
    /// This function returns the current API supported
    static Api GetApi();
    
    // Draw Calls ---------------------------------------------------------------------------------------------------
    /// This API draws a quad with pipeline and indexed count
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - count: number of Indices (if 0 then use index buffer of Vertex array)
    static void DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t count);
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

    MAKE_PURE_STATIC(Renderer);
  };
} // namespace IKan
