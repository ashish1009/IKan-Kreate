//
//  Renderer2D.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace IKan
{
  class Image;
  class Texture;

  /// This is the batch renderer for 2D Renderering
  class Renderer2D
  {
  public:
    /// This function initialzes the Batch renderer. Create All buffers needed to store Data (Both Renderer and CPU)
    static void Initialise();
    /// This functio dhutdown the batch renderer and destoy all the buffer reserved on initialize
    static void Shutdown();
    
    /// This function render the fullscreen quad
    /// - Parameters:
    ///   - texture: Texture to be loaded
    ///   - slot: Slot to be load the texture
    ///   - overrideShader: Override shader flag. if false then deafult shader is used
    /// - Note: If true then bind your own shader
    static void DrawFullscreenQuad(const Ref<Image>& texture = nullptr, uint32_t slot = 0, bool overrideShader = false);
    /// This function render the fullscreen quad
    /// - Parameters:
    ///   - texture: Texture to be loaded
    ///   - slot: Slot to be load the texture
    ///   - overrideShader: Override shader flag. if false then deafult shader is used
    /// - Note: If true then bind your own shader
    static void DrawFullscreenQuad(const Ref<Texture>& texture = nullptr, uint32_t slot = 0, bool overrideShader = false);
  };
} // namespace IKan
