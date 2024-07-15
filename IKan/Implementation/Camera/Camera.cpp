//
//  Camera.cpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#include "Camera.hpp"

namespace IKan
{
  Camera::Camera(const glm::mat4& projection, const glm::mat4& unReversedProjection)
  : m_projectionMatrix(projection), m_unReversedProjectionMatrix(unReversedProjection)
  {
    IK_PROFILE();
  }
  
  void Camera::SetProjectionMatrix(const glm::mat4& projection, const glm::mat4& unReversedProjection)
  {
    IK_PROFILE();
    m_projectionMatrix = projection;
    m_unReversedProjectionMatrix = unReversedProjection;
  }
  
  void Camera::SetPerspectiveProjectionMatrix(float radFov, float width, float height, float nearP, float farP)
  {
    IK_PROFILE();
    m_projectionMatrix = glm::perspectiveFov(radFov, width, height, farP, nearP);
    m_unReversedProjectionMatrix = glm::perspectiveFov(radFov, width, height, nearP, farP);
  }
  
  void Camera::SetOrthoProjectionMatrix(float width, float height, float nearP, float farP)
  {
    IK_PROFILE();
    m_projectionMatrix = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, farP, nearP);
    m_unReversedProjectionMatrix = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, nearP, farP);
  }
  
  const glm::mat4& Camera::GetProjectionMatrix() const
  {
    return m_projectionMatrix;
  }
  const glm::mat4& Camera::GetUnReversedProjectionMatrix() const
  {
    return m_unReversedProjectionMatrix;
  }
} // namespace IKan
