//
//  RenderPass.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Renderer/Graphics/FrameBuffer.hpp"

namespace IKan
{
  /// Interface class for storing Render Pass
  class RenderPass
  {
  public:
    struct Specification
    {
      std::string debugName;
      Ref<FrameBuffer> targetFramebuffer;
    };
    
    /// Default virtual destrructor
    virtual ~RenderPass() = default;
    
    /// This function Begins the render pass
    virtual void Begin() = 0;
    /// This function End the render pass
    virtual void End() = 0;
    /// This function resizes the Framebuffer
    /// - Parameters:
    ///   - width: New width of FB
    ///   - height: New height of FB
    virtual void Resize(uint32_t width, uint32_t height) = 0;
    
    /// This function returns the reference of specificaion
    virtual Specification& GetSpecification() = 0;
    /// This function returns the const specificaion
    virtual const Specification& GetSpecification() const = 0;
    
    /// This function creates instance of Render Pass based on the Renerer APU
    /// - Parameter spec: Render Pass Specification
    [[nodiscard]] static Ref<RenderPass> Create(const Specification& spec);
  };
} // namespace IKan
