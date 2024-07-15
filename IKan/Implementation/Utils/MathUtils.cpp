//
//  MathUtils.cpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#include "MathUtils.hpp"

namespace IKan::Utils::Math
{
  glm::mat4 GetTransformMatrix(const glm::vec3& position, const glm::vec3& rotaiton, const glm::vec3& scale)
  {
    IK_PERFORMANCE("Math::GetTransformMatrix");
    glm::mat4 rotation {glm::toMat4(glm::quat(rotaiton))};
    return glm::translate(glm::mat4(1.0f), position) * rotation * glm::scale(glm::mat4(1.0f), scale);
  }
} // namespace IKan::Utils::Math
