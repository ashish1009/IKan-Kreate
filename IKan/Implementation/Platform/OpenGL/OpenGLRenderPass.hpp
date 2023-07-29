//
//  OpenGLRenderPass.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Renderer/Graphics/RenderPass.hpp"

namespace IKan
{
  /// This is the Open GL implementation class for CreatinG Render Pass
  class OpenGLRenderPass : public RenderPass
  {
  public:
    /// This function creates instance of Render Pass based on the Renerer APU
    /// - Parameter spec: Render Pass Specification
    OpenGLRenderPass(const Specification& spec);
    /// Default virtual destrructor
    virtual ~OpenGLRenderPass();
    
    /// This function Begins the render pass
    void Begin() override;
    /// This function End the render pass
    void End() override;
    
    /// This function resizes the Framebuffer
    /// - parameters:
    ///   - width: New width of FB
    ///   - height: New height of FB
    void Resize(uint32_t width, uint32_t height) override;
    
    /// This function returns the reference of specificaion
    Specification& GetSpecification() override;
    /// This function returns the const specificaion
    const Specification& GetSpecification() const override;
    
  private:
    Specification m_specification;
  };
} // namespace IKan
