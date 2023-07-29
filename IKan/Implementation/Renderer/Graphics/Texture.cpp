//
//  Texture.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Texture.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"

namespace IKan
{
  Ref<Image> Image::Create(const std::string& filePath, bool linear)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl: return CreateRef<OpenGLImage>(filePath, linear);
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API"); break;
    }
  }
} // namespace IKan
