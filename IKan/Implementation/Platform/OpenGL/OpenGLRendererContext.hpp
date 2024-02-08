//
//  OpenGLRendererContext.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Graphics/RendererContext.hpp"

namespace IKan
{
  /// This is the implementation class for Renderer Context
  class OpenGLRendererContext : public RendererContext
  {
  public:
    /// This is the default constructor for creating Open GL Renderer context
    /// - Parameter windowPtr: GLFW Window pointer for which context need to be created
    OpenGLRendererContext(GLFWwindow* windowPtr);
    /// This is the default destructore to destroy the Open GL Rendere Context
    ~OpenGLRendererContext();
    
    /// This function swaps the renderer buffers, Should be called each frame
    void SwapBuffers() override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLRendererContext);
    
  private:
    GLFWwindow* m_window;
  };
} // namespace IKan
