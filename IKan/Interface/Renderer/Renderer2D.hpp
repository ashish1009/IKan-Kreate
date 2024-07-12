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
  };
} // namespace IKan