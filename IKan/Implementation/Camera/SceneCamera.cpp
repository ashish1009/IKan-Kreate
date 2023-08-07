//
//  SceneCamera.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "SceneCamera.hpp"

namespace IKan
{
  namespace CameraUtils
  {
    /// This function returns the Projection type in string
    /// - Parameter projType: Projection type
    std::string GetProjectionNameFromType(SceneCamera::ProjectionType projType)
    {
      switch (projType)
      {
        case SceneCamera::ProjectionType::Orthographic: return "Orthographic";
        case SceneCamera::ProjectionType::Perspective: return "Perspective";
        default: IK_ASSERT(false, "Invalid Projection Type");
      }
    }
  } // namespace CameraUtils
  
  SceneCamera::SceneCamera(ProjectionType projType)
  : m_projectionType(projType)
  {
    IK_LOG_TRACE(LogModule::SceneCamera, "Creating {0} Scene Camera ...", CameraUtils::GetProjectionNameFromType(m_projectionType));
  }
  
  void SceneCamera::SetPerspective(float degVerticalFOV, float nearClip, float farClip)
  {
    m_projectionType = ProjectionType::Perspective;
    m_degPerspectiveFOV = degVerticalFOV;
    m_perspectiveNear = nearClip;
    m_perspectiveFar = farClip;
    RecalculateProjection();
  }
  
  void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
  {
    m_projectionType = ProjectionType::Orthographic;
    m_orthographicSize = size;
    m_orthographicNear = nearClip;
    m_orthographicFar = farClip;
    RecalculateProjection();
  }
  
  void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
  {
    m_viewportWidth = width;
    m_viewportHeight = height;
    RecalculateProjection();
  }
  
  void SceneCamera::SetDegPerspectiveVerticalFOV(const float degVerticalFov)
  {
    m_degPerspectiveFOV = degVerticalFov;
    RecalculateProjection();
  }
  void SceneCamera::SetRadPerspectiveVerticalFOV(const float degVerticalFov)
  {
    m_degPerspectiveFOV = glm::degrees(degVerticalFov);
    RecalculateProjection();
  }
  void SceneCamera::SetPerspectiveNearClip(const float nearClip)
  {
    m_perspectiveNear = nearClip;
    RecalculateProjection();
  }
  void SceneCamera::SetOrthographicSize(const float size)
  {
    m_orthographicSize = size;
    RecalculateProjection();
  }
  void SceneCamera::SetOrthographicNearClip(const float nearClip)
  {
    m_orthographicNear = nearClip;
    RecalculateProjection();
  }
  void SceneCamera::SetOrthographicFarClip(const float farClip)
  {
    m_orthographicFar = farClip;
    RecalculateProjection();
  }
  void SceneCamera::SetPerspectiveFarClip(const float farClip)
  {
    m_perspectiveFar = farClip;
    RecalculateProjection();
  }
  void SceneCamera::SetProjectionType(ProjectionType type)
  {
    m_projectionType = type;
    RecalculateProjection();
  }
  
  float SceneCamera::GetPerspectiveNearClip() const
  {
    return m_perspectiveNear;
  }
  float SceneCamera::GetPerspectiveFarClip() const
  {
    return m_perspectiveFar;
  }
  float SceneCamera::GetOrthographicSize() const
  {
    return m_orthographicSize;
  }
  float SceneCamera::GetOrthographicNearClip() const
  {
    return m_orthographicNear;
  }
  float SceneCamera::GetOrthographicFarClip() const
  {
    return m_orthographicFar;
  }
  float SceneCamera::GetDegPerspectiveVerticalFOV() const
  {
    return m_degPerspectiveFOV;
  }
  float SceneCamera::GetRadPerspectiveVerticalFOV() const
  {
    return glm::radians(m_degPerspectiveFOV);
  }
  
  SceneCamera::ProjectionType SceneCamera::GetProjectionType() const
  {
    return m_projectionType;
  }
  
  void SceneCamera::RecalculateProjection() {
    switch (m_projectionType)
    {
      case ProjectionType::Perspective:
      {
        SetPerspectiveProjectionMatrix(glm::radians(m_degPerspectiveFOV), (float)m_viewportWidth,
                                       (float)m_viewportHeight, m_perspectiveNear, m_perspectiveFar);
        break;
      }
      case ProjectionType::Orthographic:
      {
        float aspect = (float)m_viewportWidth / (float)m_viewportHeight;
        float width = m_orthographicSize * aspect;
        float height = m_orthographicSize;
        SetOrthoProjectionMatrix(width, height, m_orthographicNear, m_orthographicFar);
        break;
      }
      default:
        IK_ASSERT(false);
    }
  }
  
} // namespace IKan

