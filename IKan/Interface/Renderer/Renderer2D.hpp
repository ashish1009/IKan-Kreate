//
//  Renderer2D.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace IKan
{
  /// This is the batch renderer for 2D Renderering
  class Renderer2D
  {
  public:
    /// This function initialzes the Batch renderer. Create All buffers needed to store Data (Both Renderer and CPU)
    static void Initialise();
    /// This functio dhutdown the batch renderer and destoy all the buffer reserved on initialize
    static void Shutdown();
  };
} // namespace IKan
