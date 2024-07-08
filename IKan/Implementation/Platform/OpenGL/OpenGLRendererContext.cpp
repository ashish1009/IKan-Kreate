//
//  OpenGLRendererContext.cpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#include "OpenGLRendererContext.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace IKan
{
  OpenGLRendererContext::OpenGLRendererContext(GLFWwindow* windowPtr)
  : m_window(windowPtr)
  {
    IK_PROFILE();
    
    IK_ASSERT(m_window, "Window pointer is NULL !");
    IK_LOG_TRACE(LogModule::Renderer, "Creating Open GL Renderer Context.");
    
    // Create Renderer Context for GLFW Window
    // NOTE: This function makes the OpenGL or OpenGL ES context of the specified window on the calling calling thread.
    //       A context must only be made current on a single thread at a time and each thread can have only a single
    //       context at a time.
    glfwMakeContextCurrent(m_window);
    
    // We pass GLAD the function to load the address of the OpenGL function pointers which is OS-specific. GLFW gives
    // us glfwGetProcAddress that defines the correct function based on which OS we're compiling for
    [[maybe_unused]] bool loadedGlad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    IK_ASSERT(loadedGlad, "Can not initialize the Glad");
  }
  
  OpenGLRendererContext::~OpenGLRendererContext()
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Renderer, "Destroying Open GL Renderer Context.");
  }
  
  void OpenGLRendererContext::SwapBuffers()
  {
    IK_PERFORMANCE("OpenGLRendererContext::SwapBuffers")
    glfwSwapBuffers(m_window);
  }
} // namespace IKan
