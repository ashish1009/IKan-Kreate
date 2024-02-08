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
  namespace RendererUtils
  {
    /// Convert Renderer GL Func to Open GL Func
    /// - Parameter func: Renderer Func enum
    static GLint FucToGlFunc(GlDepthFunc func)
    {
      switch (func)
      {
        case GlDepthFunc::Always : return GL_ALWAYS;
        case GlDepthFunc::Never : return GL_NEVER;
        case GlDepthFunc::Less : return GL_LESS;
        case GlDepthFunc::Equal : return GL_EQUAL;
        case GlDepthFunc::LEqual : return GL_LEQUAL;
        case GlDepthFunc::Greater : return GL_GREATER;
        case GlDepthFunc::LGreater : return GL_GEQUAL;
        case GlDepthFunc::NotEqual : return GL_NOTEQUAL;
        default:
          IK_ASSERT(false, "Invalid Type");
      }
    }
  }
  
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
  
  void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const
  {
    glClearColor(color.r, color.g, color.b, color.a);
  }
  void OpenGLRendererAPI::ClearBits() const
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }
  void OpenGLRendererAPI::ClearDepthBits() const
  {
    glClear(GL_DEPTH_BUFFER_BIT);
  }
  void OpenGLRendererAPI::ClearStencilBits() const
  {
    glClear(GL_STENCIL_BUFFER_BIT);
  }
  
  void OpenGLRendererAPI::EnableDepthPass() const
  {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
  }
  void OpenGLRendererAPI::DisableDepthPass() const
  {
    glDisable(GL_DEPTH_TEST);
  }
  void OpenGLRendererAPI::DepthFunc(GlDepthFunc func) const
  {
    glDepthFunc(RendererUtils::FucToGlFunc(func));
  }
  void OpenGLRendererAPI::EnableStencilPass() const
  {
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    
  }
  void OpenGLRendererAPI::DisableStencilPass() const
  {
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
  }
  void OpenGLRendererAPI::SetViewport(uint32_t width, uint32_t height) const
  {
    glViewport(0, 0, width, height);
  }
} // namespace IKan
