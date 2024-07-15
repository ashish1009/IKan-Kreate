//
//  FixedCamera.hpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#pragma once

namespace IKan
{
  /// This structure stores the fixed projection of camera
  /// - Note: Camera origin at bottm left corner. And Its orthographic size is too much (Objects will be smaller in size)
  struct FixedCamera
  {
    /// This function updates the fixed camera projection for scene
    /// - Parameters:
    ///   - width: width
    ///   - height: height
    static void SetViewport(const float width, const float height);
    
    inline static glm::mat4 s_projection;
    inline static uint32_t s_viewportWidth;
    inline static uint32_t s_viewportHeight;
  };
} // namespace IKan
