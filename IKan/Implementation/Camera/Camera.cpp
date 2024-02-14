//
//  Camera.cpp
//  IKan
//
//  Created by Ashish . on 14/02/24.
//

#include "Camera.hpp"

namespace IKan
{
  Camera::Camera(const glm::mat4& projection, const glm::mat4& unReversedProjection)
  : m_projectionMatrix(projection), m_unReversedProjectionMatrix(unReversedProjection)
  {
    IK_PROFILE();
  }
  
  Camera::Camera(const float degFov, const float width, const float height, const float nearP, const float farP)
  : m_projectionMatrix(glm::perspectiveFov(glm::radians(degFov), width, height, farP, nearP)),
  m_unReversedProjectionMatrix(glm::perspectiveFov(glm::radians(degFov), width, height, nearP, farP))
  {
    IK_PROFILE();
  }
  
  Camera::Camera(const float width, const float height, const float nearP, const float farP)
  : m_projectionMatrix(glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, farP, nearP)),
  m_unReversedProjectionMatrix(glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, nearP, farP))
  {
    IK_PROFILE();
  }
  
  void Camera::SetProjectionMatrix(const glm::mat4 projection, const glm::mat4 unReversedProjection)
  {
    IK_PROFILE();
    m_projectionMatrix = projection;
    m_unReversedProjectionMatrix = unReversedProjection;
  }
  
  void Camera::SetPerspectiveProjectionMatrix(const float radFov, const float width, const float height, const float nearP, const float farP)
  {
    IK_PROFILE();
    m_projectionMatrix = glm::perspectiveFov(radFov, width, height, farP, nearP);
    m_unReversedProjectionMatrix = glm::perspectiveFov(radFov, width, height, nearP, farP);
  }
  
  void Camera::SetOrthoProjectionMatrix(const float width, const float height, const float nearP, const float farP)
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
