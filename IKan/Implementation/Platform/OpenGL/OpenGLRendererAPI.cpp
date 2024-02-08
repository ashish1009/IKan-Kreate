//
//  OpenGLRendererAPI.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include <glad/glad.h>
#include "OpenGLRendererAPI.hpp"
#include "Renderer/Renderer.hpp"

namespace IKan
{
  OpenGLRendererAPI::OpenGLRendererAPI()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Renderer, "Creating Open GL Renderer API");
    
    // Enable Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // For Text Rendering
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_MULTISAMPLE);
    
    // Enable
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    auto& caps = RendererCapabilities::Get();
    caps.vendor   = (const char*)glGetString(GL_VENDOR);
    caps.renderer = (const char*)glGetString(GL_RENDERER);
    caps.version  = (const char*)glGetString(GL_VERSION);
    
    IK_LOG_INFO(LogModule::Renderer, "  Vendor       | {0}", caps.vendor);
    IK_LOG_INFO(LogModule::Renderer, "  Renderer     | {0}", caps.renderer);
    IK_LOG_INFO(LogModule::Renderer, "  Version      | {0}", caps.version);
  }
  
  OpenGLRendererAPI::~OpenGLRendererAPI()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Renderer, "Destroying Open GL Renderer API");
  }
} // namespace IKan
