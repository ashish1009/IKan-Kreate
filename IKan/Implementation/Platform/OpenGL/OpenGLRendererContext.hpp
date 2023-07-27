//
//  OpenGLRendererContext.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

#include "Renderer/Graphics/RendererContext.hpp"

namespace IKan
{
  class OpenGLRendererContext : public RendererContext
  {
  public:
    /// This is the default constructor for creating Open GL Renderer context
    /// - Parameter window_ptr: GLFW Window pointer for which context need to be created
    OpenGLRendererContext(GLFWwindow* window_ptr);
    /// This is the default destructore to destroy the Open GL Rendere Context
    ~OpenGLRendererContext();
    
    /// This function swaps the renderer buffers, Should be called each frame
    void SwapBuffers() override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLRendererContext);
    
  private:
    GLFWwindow* m_window;
  };
} // namespace IKan
