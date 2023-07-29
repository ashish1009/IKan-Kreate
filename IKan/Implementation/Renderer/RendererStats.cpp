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

    _2d.quads = 0;
    _2d.circles = 0;
    _2d.lines = 0;
    _2d.chars = 0;
  }
  
  void RendererStatistics::ResetAll()
  {
    ResetEachFrame();
    
    vertexBufferSize = 0;
    indexBufferSize = 0;
    textureBufferSize = 0;

    _2d.maxQuads = 0;
    _2d.maxCircles = 0;
    _2d.maxLines = 0;
  }
  
  RendererStatistics& RendererStatistics::Get()
  {
    static RendererStatistics instance;
    return instance;
  }
} // namespace IKan
