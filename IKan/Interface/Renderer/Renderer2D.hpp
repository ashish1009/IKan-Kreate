//
//  Renderer2D.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

namespace IKan
{
  class Texture;
  
  struct Renderer2DBatchLimit
  {
    uint32_t maxQuads {0};
    uint32_t maxCircles {0};
    uint32_t maxLines {0};
  };
  
  /// This is the batch renderer for 2D Renderering
  class Renderer2D
  {
  public:
    /// This function initialzes the 2D renderer.
    /// - Parameter data: Batch Rendere Limit data
    static void Initialize(const Renderer2DBatchLimit& data);
    /// This function dhutdown the 2D renderer.
    static void Shutdown();
    
    /// This funcition Adds the quads renderer data to prev allocaged data
    /// - Parameter maxQuads: max quad to be renderered in single batch
    static void AddQuadData(uint32_t maxQuads);
    /// This funcition Adds the circle renderer data to prev allocaged data
    /// - Parameter maxCirlces: max circle to be renderered in single batch
    static void AddCircleData(uint32_t maxCirlces);
    /// This funcition Adds the maxLines renderer data to prev allocaged data
    /// - Parameter maxLines: max maxLines to be renderered in single batch
    static void AddLineData(uint32_t maxLines);
  };
} // namespace IKan
