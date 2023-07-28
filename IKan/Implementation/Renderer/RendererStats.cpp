//
//  RendererStats.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "RendererStats.hpp"

namespace IKan
{
  void RendererStatistics::ResetEachFrame()
  {
    drawCalls = 0;
    indexCount = 0;
    vertexCount = 0;
  }
  
  void RendererStatistics::ResetAll()
  {
    ResetEachFrame();
    
    vertexBufferSize = 0;
    indexBufferSize = 0;
    textureBufferSize = 0;
  }
  
  RendererStatistics& RendererStatistics::Get()
  {
    static RendererStatistics instance;
    return instance;
  }
} // namespace IKan
