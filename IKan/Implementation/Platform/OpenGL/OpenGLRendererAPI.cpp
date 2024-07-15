//
//  OpenGLRendererAPI.cpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#include "OpenGLRendererAPI.hpp"

#include <glad/glad.h>

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Pipeline.hpp"
#include "Renderer/RendererStats.hpp"

namespace IKan
{
  OpenGLRendererAPI::OpenGLRendererAPI()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Renderer, "Creating Open GL Renderer API instance.");
    
    // Enable Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // For Text Rendering
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_MULTISAMPLE);
    
    // Enable
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    RendererCapabilities& caps = RendererCapabilities::Get();
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
    IK_LOG_WARN(LogModule::Renderer, "Destroying Open GL Renderer API instance.");
  }
  
  void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const
  {
    Renderer::Submit([color](){
      glClearColor(color.r, color.g, color.b, color.a);
    });
  }
  void OpenGLRendererAPI::ClearBits() const
  {
    Renderer::Submit([](){
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    });
  }
  void OpenGLRendererAPI::ClearColorBits() const
  {
    Renderer::Submit([](){
      glClear(GL_COLOR_BUFFER_BIT);
    });
  }
  void OpenGLRendererAPI::ClearDepthBits() const
  {
    Renderer::Submit([](){
      glClear(GL_DEPTH_BUFFER_BIT);
    });
  }
  void OpenGLRendererAPI::ClearStencilBits() const
  {
    Renderer::Submit([](){
      glClear(GL_STENCIL_BUFFER_BIT);
    });
  }
  
  
  void OpenGLRendererAPI::SetViewport(uint32_t width, uint32_t height) const
  {
    Renderer::Submit([width, height](){
      glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    });
  }
  
  void OpenGLRendererAPI::DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t count) const
  {
    IK_ASSERT(pipeline, "Pipeline is NULL!");
    
    pipeline->Bind();
    
    Renderer::Submit([count](){
      glDrawElements(GL_TRIANGLES, (GLsizei)count, GL_UNSIGNED_INT, nullptr);
      glBindTexture(GL_TEXTURE_2D, 0);
    });
    
    pipeline->Unbind();
    RendererStatistics::Get().drawCalls++;
  }

} // namespace IKan
