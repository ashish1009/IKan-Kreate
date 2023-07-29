//
//  Renderer2D.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Renderer2D.hpp"
#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Renderer/Graphics/Pipeline.hpp"

namespace IKan
{
  /// This structures stores the Full Screen quad
  struct FullScreenQuad
  {
    /// Vertices of Full Screen Quad
    static constexpr float vertices[] =
    {
      // positions        // texture Coords
      -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
      1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
      1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    
    /// Pipelne to render full screen quad
    Ref<Pipeline> pipeline;
    Ref<VertexBuffer> vertexBuffer;
    
    /// This function initilaise the full screen quad data
    FullScreenQuad()
    {

    }
  };
  static Scope<FullScreenQuad> s_fullscreenQuadData;
  
  void Renderer2D::Initialise()
  {
    s_fullscreenQuadData = CreateScope<FullScreenQuad>();
  }
  
  void Renderer2D::Shutdown()
  {
    s_fullscreenQuadData.reset();
  }
} // namespace IKan
