//
//  RendererStats.hpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#pragma once

namespace IKan
{
  /// Stores the statistics of renderer
  struct RendererStatistics
  {
  public:
    // Member Variables ----------------------------------------------------------------------------------------------
    uint32_t vertexBufferSize = 0, indexBufferSize = 0, textureBufferSize = 0;
    uint32_t vertexCount  = 0, indexCount = 0;
    uint32_t drawCalls = 0;
    
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This function resets only those stats that need to be reset each frame
    void ResetEachFrame();
    /// This function resets all stats
    void ResetAll();
    
    /// This function returns the static singleton instance of RendererStatistics
    static RendererStatistics& Get();
  };
} // namespace IKan
