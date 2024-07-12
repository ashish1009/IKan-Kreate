//
//  Shader.cpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#include "Shader.hpp"

namespace IKan
{
  Ref<Shader> ShaderLibrary::Get(const std::filesystem::path& shaderFilePath)
  {
    IK_PROFILE();
    if (s_shaders.find(shaderFilePath) == s_shaders.end() or !s_shaders.at(shaderFilePath))
    {
      s_shaders[shaderFilePath] = ShaderFactory::Create(shaderFilePath);
      IK_LOG_TRACE(LogModule::Shader, "Shader '{0}' will be added to Shader Library. (Total Shaders {1})", shaderFilePath.filename().string(), s_shaders.size());
    }
    else
    {
      IK_LOG_TRACE(LogModule::Shader, "Returning Pre loaded Shader '{0}' from Shader Library", shaderFilePath.filename().string());
    }
    return s_shaders.at(shaderFilePath);
  }
  void ShaderLibrary::Clear()
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Shader, "Removing all shaders from Shdaer Library");
    s_shaders.clear();
  }
} // namespace IKan
