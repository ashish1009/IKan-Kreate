//
//  OpenGLRendererAPI.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include <glad/glad.h>
#include "OpenGLRendererAPI.hpp"
#include "Renderer/Renderer.hpp"

namespace IKan
{
  OpenGLRendererAPI::OpenGLRendererAPI()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Renderer, "Creating Open GL Renderer API");
  }
  
  OpenGLRendererAPI::~OpenGLRendererAPI()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Renderer, "Destroying Open GL Renderer API");
  }
} // namespace IKan
