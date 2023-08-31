//
//  Shader.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "Shader.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"

namespace IKan
{
  Ref<Shader> Shader::Create(const std::string &filePath)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl:
      {
        if (s_shaders.find(filePath) == s_shaders.end() or !s_shaders.at(filePath))
        {
          s_shaders[filePath] = CreateRef<OpenGLShader>(filePath);
          IK_LOG_DEBUG(LogModule::Shader, "Adding Shader '{0}' to Shader Library", Utils::String::GetFileNameFromPath(filePath));
          IK_LOG_DEBUG(LogModule::Shader, "Number of Shaders loaded yet {0}", s_shaders.size());
        }
        else
        {
          IK_LOG_DEBUG(LogModule::Shader, "Returning Pre loaded Shader '{0}' from Shader Library", Utils::String::GetFileNameFromPath(filePath));
        }
        return s_shaders.at(filePath);
      }
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API");
    }
  }
  
  void Shader::ResetLibrary()
  {
    IK_PROFILE();
    for (auto it = s_shaders.begin(); it != s_shaders.end(); it++)
    {
      it->second.reset();
      IK_LOG_DEBUG(LogModule::Shader, "Removing Shader '{0}' from Shdaer Library", Utils::String::GetFileNameFromPath(it->first));
    }
  }

} // namespace IKan
