//
//  MathUtils.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "MathUtils.hpp"

namespace IKan::Utils::Math
{
  glm::mat4 GetTransformMatrix(const glm::vec3& position, const glm::vec3& rotaiton, const glm::vec3& scale)
  {
    glm::mat4 rotation = glm::toMat4(glm::quat(rotaiton));
    return glm::translate(glm::mat4(1.0f), position) * rotation * glm::scale(glm::mat4(1.0f), scale);
  }
  void DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale)
  {
    // From glm::decompose in matrix_decompose.inl
    using namespace glm;
    using T = float;
    
    mat4 LocalMatrix(transform);
    
    // Normalize the matrix.
    RETURN_IF (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
    
    // First, isolate perspective.  This is the messiest.
    if ( epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) or
        epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) or
        epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
    {
      // Clear the perspective partition
      LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
      LocalMatrix[3][3] = static_cast<T>(1);
    }
    
    // Next take care of translation (easy).
    outTranslation = vec3(LocalMatrix[3]);
    LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);
    
    vec3 Row[3];
    
    // Now get scale and shear.
    for (length_t i = 0; i < 3; ++i)
    {
      for (length_t j = 0; j < 3; ++j)
      {
        Row[i][j] = LocalMatrix[i][j];
      }
    }
    
    // Compute X scale factor and normalize first row.
    outScale.x = length(Row[0]);
    Row[0] = detail::scale(Row[0], static_cast<T>(1));
    outScale.y = length(Row[1]);
    Row[1] = detail::scale(Row[1], static_cast<T>(1));
    outScale.z = length(Row[2]);
    Row[2] = detail::scale(Row[2], static_cast<T>(1));
    
    outRotation.y = asin(-Row[0][2]);
    if (cos(outRotation.y) != 0)
    {
      outRotation.x = atan2(Row[1][2], Row[2][2]);
      outRotation.z = atan2(Row[0][1], Row[0][0]);
    }
    else
    {
      outRotation.x = atan2(-Row[2][0], Row[1][1]);
      outRotation.z = 0;
    }
  }
  
  void Print(const std::string& title, const glm::mat4& mat)
  {
#if IK_ENABLE_LOG
    // Max Tag String space to be reserved in log
    static constexpr uint32_t MaxValueLength = 10;

    std::string result = title;

    for (int i = 0; i < 4; i++)
    {
      result += "\n";
      result += std::string(static_cast<size_t>(56), ' ');;
      for (int j = 0; j < 4; j++)
      {
        std::string valString = std::to_string(mat[i][j]);
        valString += std::string(static_cast<size_t>(MaxValueLength - valString.size()), ' ');
        result += valString;
      }
    }
    IK_LOG_TRACE("[Math Debug]", "{0}", result);

#endif
  }
  void Print(const glm::vec3& mat)
  {
#if IK_ENABLE_LOG
#endif
  }
} // namespace IKan::Utils::Math
