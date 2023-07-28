//
//  Renderer.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

namespace IKan
{
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

    MAKE_PURE_STATIC(Renderer);
  };
} // namespace IKan
