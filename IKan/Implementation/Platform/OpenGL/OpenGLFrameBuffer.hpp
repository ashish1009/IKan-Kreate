//
//  OpenGLFrameBuffer.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Renderer/Graphics/FrameBuffer.hpp"
#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  /// Open GL Implementation class to store Framebuffer
  class OpenGLFrameBuffer : public FrameBuffer
  {
  public:
    /// This is the consturctor of frame buffer the create the buffer with the specification
    /// - Parameter spec: Framebuffer specificaion
    OpenGLFrameBuffer(const Specification& spec);
    /// This is the default frame buffer destructor that delete the buffer
    ~OpenGLFrameBuffer();
  };
} // namespace IKan
