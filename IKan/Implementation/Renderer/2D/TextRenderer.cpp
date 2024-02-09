//
//  TextRenderer.cpp
//  IKan
//
//  Created by Ashish . on 09/02/24.
//

#include "TextRenderer.hpp"

namespace IKan 
{
  void TextRenderer::Initialize()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::TextRenderer, "Initialising the Text Renderer ");
  }
  
  void TextRenderer::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::TextRenderer, "Shutting down the Text Renderer ");
  }
}
