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
  
  /// Stores the capability of renderer
  struct RendererCapabilities
  {
    std::string vendor;
    std::string renderer;
    std::string version;
    
    static RendererCapabilities& Get();
  };

  /// This is the Wrapper class for all Renderer APIs (User and Graphics)
  /// Major responsibilities for Renderer is
  /// - Create Renderer API based on the renderer supported by the engine
  /// - Initialize and shutdown all the user defined renderers.
  /// - Manage Shader and Texture library to store the pre loaded shader and textures
  class Renderer
  {
  public:
    // Fundamentals --------------------------------------------------------------------------------------------------
    /// This function initializes all the renderers including user defined renderer and also setup the current Renderer
    /// API as 'api'.
    static void Initialize();
    /// This function shut down all the renderers including user defined renderer. Also destroy the instance created for
    /// any Graphics API.
    static void Shutdown();
    
    // Renderer Type -------------------------------------------------------------------------------------------------
    /// This function sets the current renderer API type
    /// - Parameter rendererType: current renderer type
    /// - Note: Should be set before Creating any Renderer Context
    static void SetCurrentRendererAPI(RendererType rendererType);
    /// This function return the current renderer API type
    static RendererType GetCurrentRendererAPI();

    DELETE_ALL_CONSTRUCTORS(Renderer);
  };
} // namespace IKan
