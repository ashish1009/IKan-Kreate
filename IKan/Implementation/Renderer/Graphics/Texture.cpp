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
      case Renderer::Api::OpenGl:
      {
        if (s_textureLibrary.find(filePath) == s_textureLibrary.end() or !s_textureLibrary[filePath][linear])
        {
          s_textureLibrary[filePath][linear] = CreateRef<OpenGLImage>(filePath, linear);
          IK_LOG_DEBUG(LogModule::Texture, "Adding Texture '{0}' to Texture Library",
                       Utils::String::GetFileNameFromPath(filePath));
          IK_LOG_DEBUG(LogModule::Texture, "Number of Textures loaded {0}",
                       s_textureLibrary.size());
        }
        else
        {
          IK_LOG_DEBUG(LogModule::Texture, "Returning Pre loaded Texture '{0}' from Texture Library",
                       Utils::String::GetFileNameFromPath(filePath));
        }
        return s_textureLibrary.at(filePath)[linear];
      }
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
  
  void Image::ResetLibrary()
  {
    for (auto it = s_textureLibrary.begin(); it != s_textureLibrary.end(); it++)
    {
      IK_LOG_DEBUG(LogModule::Texture, "Removing Texture '{0}' from Texture Library",
                   Utils::String::GetFileNameFromPath(it->first));
      for (int i = 0; i < 2; i++)
      {
        it->second[i].reset();
      }
    }
  }

} // namespace IKan
