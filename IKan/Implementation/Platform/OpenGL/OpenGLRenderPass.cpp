//
//  OpenGLRenderPass.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "OpenGLRenderPass.hpp"

namespace IKan
{
  OpenGLRenderPass::OpenGLRenderPass(const Specification& spec)
  : m_specification(spec)
  {
    IK_LOG_DEBUG(LogModule::RenderPass, "Creating Open GL Render Pass '{0}'", m_specification.debugName);
  }
  
  OpenGLRenderPass::~OpenGLRenderPass() {
    IK_LOG_DEBUG(LogModule::RenderPass, "Destroying Open GL Render Pass '{0}'", m_specification.debugName);
  }
  
  void OpenGLRenderPass::Begin()
  {
    m_specification.targetFramebuffer->Bind();
  }
  
  void OpenGLRenderPass::End()
  {
    m_specification.targetFramebuffer->Unbind();
  }
  
  void OpenGLRenderPass::Resize(uint32_t width, uint32_t height)
  {
    m_specification.targetFramebuffer->Resize(width, height);
  }
  
  RenderPass::Specification& OpenGLRenderPass::GetSpecification()
  {
    return m_specification;
  }
  
  const RenderPass::Specification& OpenGLRenderPass::GetSpecification() const
  {
    return m_specification;
  }
} // namespace IKan
