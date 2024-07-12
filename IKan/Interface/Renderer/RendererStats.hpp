//
//  RendererStats.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

namespace IKan
{
  /// This structure stores the statistics of renderer data
  struct RendererStatistics
  {
  public:
    // Member Variables ----------------------------------------------------------------------------------------------
    uint64_t vertexBufferSize {0}, indexBufferSize {0}, textureBufferSize {0};
    uint32_t vertexCount {0}, indexCount {0};
    uint32_t drawCalls {0};
    
    struct _2D
    {
      uint32_t maxQuads {0}, maxCircles {0}, maxLines {0};
      uint32_t quads {0}, circles {0}, lines {0}, chars {0};
    };
    
    // Stores the 2D Specific data
    _2D _2d;
    
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This function resets only those stats that need to be reset each frame
    void ResetEachFrame();
    /// This function resets all stats
    void ResetAll();
    
    /// This function returns the static singleton instance of RendererStatistics
    static RendererStatistics& Get();
  };
} // namespace IKan
