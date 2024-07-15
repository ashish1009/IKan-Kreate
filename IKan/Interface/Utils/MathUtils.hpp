//
//  MathUtils.hpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#pragma once

namespace IKan::Utils::Math
{
  static constexpr glm::vec2 ZeroVec2 {0.0f};
  static constexpr glm::vec3 ZeroVec3 {0.0f};
  static constexpr glm::vec4 ZeroVec4 {0.0f};
  static constexpr glm::mat2 ZeroMat2 {0.0f};
  static constexpr glm::mat3 ZeroMat3 {0.0f};
  static constexpr glm::mat4 ZeroMat4 {0.0f};
  
  static constexpr glm::vec2 UnitVec2 {1.0f};
  static constexpr glm::vec3 UnitVec3 {1.0f};
  static constexpr glm::vec4 UnitVec4 {1.0f};
  static constexpr glm::mat2 UnitMat2 {1.0f};
  static constexpr glm::mat3 UnitMat3 {1.0f};
  static constexpr glm::mat4 UnitMat4 {1.0f};
  
  /// This function generates and returns the transform matrix from position scale and rotation
  /// - Parameters:
  ///   - position: Position of the component:
  ///   - rotation: Rotation of the component. Default glm vec3(0.0f):
  ///   - scale: Size of the component. Default glm vec3(1.0f):
  ///  - Important: Return value should not be discarded.
  ///  - Important: This function cost in CPU cycle
  [[nodiscard]] glm::mat4 GetTransformMatrix(const glm::vec3& position, const glm::vec3& rotation = ZeroVec3, const glm::vec3& scale = UnitVec3);
} // namespace IKan::Utils::Math
