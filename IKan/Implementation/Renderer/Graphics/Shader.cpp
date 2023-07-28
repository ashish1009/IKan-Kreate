//
//  Shader.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "Shader.hpp"

namespace IKan
{
  Ref<Shader> Shader::Create(const std::string &shaderFilePath)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl:
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API");
    }
  }
} // namespace IKan
