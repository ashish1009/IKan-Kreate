//
//  SceneCamera.cpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#include "SceneCamera.hpp"

namespace IKan
{
  namespace CameraUtils
  {
    /// This function returns the Projection type in string
    /// - Parameter projType: Projection type
    [[maybe_unused]] static std::string_view GetProjectionNameFromType(SceneCamera::ProjectionType projType)
    {
      switch (projType)
      {
        case SceneCamera::ProjectionType::Orthographic: return "Orthographic";
        case SceneCamera::ProjectionType::Perspective: return "Perspective";
        default: IK_ASSERT(false, "Invalid Projection Type!");
      }
      return "";
    }
  } // namespace CameraUtils
  
  SceneCamera::SceneCamera(ProjectionType projType)
  : m_projectionType(projType)
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::SceneCamera, "Creating {0} Scene Camera ...", CameraUtils::GetProjectionNameFromType(m_projectionType));
    RecalculateProjection();
  }
  
  void SceneCamera::SetPerspective(float degVerticalFOV, float nearClip, float farClip)
  {
    IK_PROFILE();
    m_projectionType = ProjectionType::Perspective;
    m_degPerspectiveFOV = degVerticalFOV;
    m_perspectiveNear = nearClip;
    m_perspectiveFar = farClip;
    RecalculateProjection();
  }
  
  void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
  {
    IK_PROFILE();
    m_projectionType = ProjectionType::Orthographic;
    m_orthographicSize = size;
    m_orthographicNear = nearClip;
    m_orthographicFar = farClip;
    RecalculateProjection();
  }
  
  void SceneCamera::SetViewportSize(const uint32_t width, const uint32_t height)
  {
    IK_PROFILE()
    if (0 == width or 0 == height or (width == m_viewportWidth and height == m_viewportHeight))
    {
      return;
    }
    
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
  
  uint32_t SceneCamera::GetWidth() const
  {
    return m_viewportWidth;
  }
  uint32_t SceneCamera::GetHeight() const
  {
    return m_viewportHeight;
  }
  float SceneCamera::GetAspectRatio() const
  {
    IK_ASSERT(0 == m_viewportHeight, "Height is 0!");
    return (float)m_viewportWidth / (float)m_viewportHeight;
  }
  
  void SceneCamera::RecalculateProjection()
  {
    IK_PROFILE();
    switch (m_projectionType)
    {
      case ProjectionType::Perspective:
      {
        SetPerspectiveProjectionMatrix(glm::radians(m_degPerspectiveFOV), (float)m_viewportWidth, (float)m_viewportHeight, m_perspectiveNear, m_perspectiveFar);
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
