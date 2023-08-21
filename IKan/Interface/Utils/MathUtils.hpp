//
//  MathUtils.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace IKan::Utils
{
  /// Wrapper GLM Match Class
  class Math
  {
  public:
    /// This funciton decomposes the Transform matrix as position rotation and scale
    /// - Parameters:
    ///   - transform: Transfrom matrix to be decomposed:
    ///   - outTranslation: Position output
    ///   - outRotation: Rotation output
    ///   - outScale: Scale output
    ///  - Important: This function cost too much in CPU
    static void DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
    
    /// This function generates and returns the transform matrix from position scale and rotation
    /// - Parameters:
    ///   - position: Position of the component:
    ///   - rotation: Rotation of the component. Default glm vec3(0.0f):
    ///   - scale: Size of the component. Default glm vec3(1.0f):
    ///  - Important: Return value should not be discarded.
    ///  - Important: This function cost too much in CPU
    [[nodiscard]] static glm::mat4 GetTransformMatrix(const glm::vec3& position, const glm::vec3& rotation = {0, 0, 0},
                                                      const glm::vec3& scale = {1, 1, 1});
    
    /// This function conerts the Cartisian point to Isometric
    /// - Parameter cartisian: cartisian point
    [[nodiscard]] static glm::vec2 GetIsometricFromCartesian(const glm::vec2& cartisian);
    
    MAKE_PURE_STATIC(Math);
  };
} // namespace IKan::Utils
