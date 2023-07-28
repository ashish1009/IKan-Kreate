//
//  OpenGLRendererAPI.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include <glad/glad.h>
#include "OpenGLRendererAPI.hpp"
#include "Renderer/Renderer.hpp"

namespace IKan
{
  OpenGLRendererAPI::OpenGLRendererAPI()
  {
    IK_LOG_TRACE(LogModule::Renderer, "Creating Open GL Renderer API");
    IK_LOG_TRACE(LogModule::Renderer, "-----------------------------");
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    
    auto& caps = Renderer::Capabilities::Get();
    
    caps.vendor   = (const char*)glGetString(GL_VENDOR);
    caps.renderer = (const char*)glGetString(GL_RENDERER);
    caps.version  = (const char*)glGetString(GL_VERSION);
    
    IK_LOG_TRACE(LogModule::Renderer, "  Multi Sample | Enabled ");
    IK_LOG_TRACE(LogModule::Renderer, "  Blending     | Enabled ");
    IK_LOG_TRACE(LogModule::Renderer, "  Depth        | Enabled (One Minus Alpha)");
    
    IK_LOG_TRACE(LogModule::Renderer, "  Vendor       | {0}", caps.vendor);
    IK_LOG_TRACE(LogModule::Renderer, "  Renderer     | {0}", caps.renderer);
    IK_LOG_TRACE(LogModule::Renderer, "  Version      | {0}", caps.version);
  }
  
  OpenGLRendererAPI::~OpenGLRendererAPI()
  {
    IK_LOG_WARN(LogModule::Renderer, "Destroying Open GL Renderer API");
  }
  
  void OpenGLRendererAPI::Blend(bool state) const
  {
    if (state)
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
      glDisable(GL_BLEND);
    }
  }
  void OpenGLRendererAPI::Depth(bool state) const
  {
    (state) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
  }
  
  void OpenGLRendererAPI::MultiSample(bool state) const
  {
    (state) ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
  }
  void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const
  {
    glClearColor(color.r, color.g, color.b, color.a);
  }
  void OpenGLRendererAPI::ClearBits() const
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }
  void OpenGLRendererAPI::ClearDepthBit() const
  {
    glClear(GL_DEPTH_BUFFER_BIT);
  }
  void OpenGLRendererAPI::ClearStencilBit() const
  {
    glClear(GL_STENCIL_BUFFER_BIT);
  }
  void OpenGLRendererAPI::ClearColorBit() const
  {
    glClear(GL_COLOR_BUFFER_BIT);
  }
  
  void OpenGLRendererAPI::SetViewport(uint32_t width, uint32_t height) const
  {
    glViewport(0, 0, width, height);
  }
  void OpenGLRendererAPI::GetEntityIdFromPixels(int32_t mx, int32_t my, uint32_t pixelIDIndex, int32_t& pixelData) const
  {
    glReadBuffer(GL_COLOR_ATTACHMENT0 + pixelIDIndex);
    glReadPixels(mx, my, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
  }
} // namespace IKan
