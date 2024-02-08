//
//  Shader.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "Shader.hpp"

namespace IKan
{
  Ref<Shader> ShaderLibrary::GetShader(const std::filesystem::path& shaderFilePath)
  {
    IK_PROFILE();
    if (s_shaders.find(shaderFilePath) == s_shaders.end() or !s_shaders.at(shaderFilePath))
    {
      s_shaders[shaderFilePath] = ShaderFactory::Create(shaderFilePath);
      IK_LOG_DEBUG(LogModule::Shader, "Adding Shader '{0}' to Shader Library. (Total Shaders {1})", static_cast<std::string>(shaderFilePath.filename()), s_shaders.size());
    }
    else
    {
      IK_LOG_DEBUG(LogModule::Shader, "Returning Pre loaded Shader '{0}' from Shader Library", static_cast<std::string>(shaderFilePath.filename()));
    }
    return s_shaders.at(shaderFilePath);
  }
  void ShaderLibrary::Reset()
  {
    IK_PROFILE();
    for (auto it = s_shaders.begin(); it != s_shaders.end(); it++)
    {
      it->second.reset();
      IK_LOG_DEBUG(LogModule::Shader, "Removing Shader '{0}' from Shdaer Library", static_cast<std::string>(static_cast<std::filesystem::path>(it->first).filename()));
    }
  }
} // namespace IKan
