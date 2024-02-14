//
//  Renderer3D.hpp
//  IKan
//
//  Created by Ashish . on 14/02/24.
//

#pragma once

namespace IKan
{
  class Renderer3D
  {
  public:
    /// This funcion initialize the 3D Renderer
    static void Initialize();
    /// This funcion shutsdown the 3D Renderer
    static void Shutdown();
    /// This function renders full screen cube
    static void DrawFullscreenCube();
    
    DELETE_ALL_CONSTRUCTORS(Renderer3D);
  };
} // namespace IKan
