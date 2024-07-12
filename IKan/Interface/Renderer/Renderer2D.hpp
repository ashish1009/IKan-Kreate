//
//  Renderer2D.hpp
//  IKan
//
//  Created by Ashish . on 10/07/24.
//

#pragma once

namespace IKan
{
  /// This class stores the APIs to initialize the 2D batch renderer and draw the 2D shapes
  class Renderer2D
  {
  public:
    /// This function initializes the 2D batch renderer
    static void Initialize();
    /// This function shuts down the 2D batch renderer
    static void Shutdown();
    
    /// This funcition Adds the quad batch limit to prev allocated data
    /// - Parameter quads: quads to be renderered in single batch
    static void AddQuads(uint32_t quads);
    /// This funcition Adds the circle batch limit to prev allocated data
    /// - Parameter circles: circles to be renderered in single batch
    static void AddCircles(uint32_t circles);
    /// This funcition Adds the line batch limit to prev allocated data
    /// - Parameter lines: lines to be renderered in single batch
    static void AddLines(uint32_t lines);
  };
} // namespace IKan
