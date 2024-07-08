//
//  OpenGLRendererContext.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Renderer/Graphics/RendererContext.hpp"

namespace IKan
{
  /// This class implements the APIs for creating Open GL Renderer Context.
  class OpenGLRendererContext : public RendererContext
  {
  public:
    /// This constructor creates the Open GL Renderer context.
    /// - Parameter windowPtr: GLFW Window pointer for which context need to be created.
    OpenGLRendererContext(GLFWwindow* windowPtr);
    /// This destructor destroys the Open GL Rendere Context.
    ~OpenGLRendererContext();
    
    /// This function swaps the renderer buffers, Should be called each frame.
    void SwapBuffers() override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLRendererContext);
    
  private:
    GLFWwindow* m_window;
  };
} // namespace IKan
