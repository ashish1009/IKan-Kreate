//
//  OpenGLTexture.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "OpenGLTexture.hpp"

namespace IKan
{
  Ref<Image> Image::Create(const std::string& filePath, bool linear)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl:
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API"); break;
    }
  }
} // namespace IKan
