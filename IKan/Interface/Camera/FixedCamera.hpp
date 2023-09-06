//
//  FixedCamera.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace IKan
{
  /// This structure stores the fixed projection of camera
  /// - Note: Camera origin at bottm left corner. And Its orthographic size is too much (Objects will be smaller in size)
  struct FixedCamera
  {
    /// This function update the fixed camera projection for scene
    /// - Parameters:
    ///   - width: width
    ///   - height: height
    static void SetViewport(float width, float height);
    
    inline static glm::mat4 s_projection;
    inline static uint32_t s_viewportWidth;
    inline static uint32_t s_viewportHeight;
  };
  
} // namespace IKan
