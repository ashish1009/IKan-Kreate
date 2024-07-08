//
//  Renderer.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

namespace IKan
{
  /// Data type of Renderer IDs
  using RendererID = uint32_t;
  
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

    DELETE_ALL_CONSTRUCTORS(Renderer);
  };
} // namespace IKan
