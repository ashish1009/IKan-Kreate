//
//  EditorCamera.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "EditorCamera.hpp"
#include "Core/Input.hpp"

namespace IKan
{
  namespace CameraUtils
  {
    static void DisableMouse()
    {
      Input::SetCursorMode(CursorMode::Locked);
    }
    static void EnableMouse()
    {
      Input::SetCursorMode(CursorMode::Normal);
    }
  } // namespace CameraUtils
  
  EditorCamera::EditorCamera(const float degFov, const float width, const float height, const float nearP,
                             const float farP)
  : Camera(glm::perspectiveFov(glm::radians(degFov), width, height, farP, nearP),
           glm::perspectiveFov(glm::radians(degFov), width, height, nearP, farP)),
  m_focalPoint(0.0f), m_verticalFOV(glm::radians(degFov)), m_nearClip(nearP), m_farClip(farP)
  {
    IK_LOG_TRACE(LogModule::EditorCamera, "Creating Editor Camera");
    Initialize();
  }
  
  void EditorCamera::Initialize()
  {
    constexpr glm::vec3 position = { -5, 5, 10 };
    m_distance = glm::distance(position, m_focalPoint);
    
    m_yaw = 3.410f;
    m_pitch = 0.174f;
    
    m_position = CalculatePosition();
    const glm::quat orientation = GetOrientation();
    m_direction = glm::eulerAngles(orientation) * (180.0f / glm::pi<float>());
    m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
    m_viewMatrix = glm::inverse(m_viewMatrix);
  }
  
  void EditorCamera::OnUpdate(const TimeStep ts)
  {
    IK_PERFORMANCE_FUN();
    const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
    const glm::vec2 delta = (mouse - m_initialMousePosition) * 0.002f;
    
    if (m_isActive)
    {
      if (Input::IsMouseButtonPressed(MouseButton::Right) and !Input::IsKeyPressed(Key::LeftControl))
      {
        m_cameraMode = CameraMode::FLYCAM;
        CameraUtils::DisableMouse();
        const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        
        const float speed = GetCameraSpeed();
        
        if (Input::IsKeyPressed(Key::Q))
        {
          m_positionDelta -= ts.MilliSeconds() * speed * glm::vec3{ 0.f, yawSign, 0.f };
        }
        if (Input::IsKeyPressed(Key::E))
        {
          m_positionDelta += ts.MilliSeconds() * speed * glm::vec3{ 0.f, yawSign, 0.f };
        }
        if (Input::IsKeyPressed(Key::S))
        {
          m_positionDelta -= ts.MilliSeconds() * speed * m_direction;
        }
        if (Input::IsKeyPressed(Key::W))
        {
          m_positionDelta += ts.MilliSeconds() * speed * m_direction;
        }
        if (Input::IsKeyPressed(Key::A))
        {
          m_positionDelta -= ts.MilliSeconds() * speed * m_rightDirection;
        }
        if (Input::IsKeyPressed(Key::D))
        {
          m_positionDelta += ts.MilliSeconds() * speed * m_rightDirection;
        }
        
        constexpr float maxRate{ 0.12f };
        m_yawDelta += glm::clamp(yawSign * delta.x * RotationSpeed(), -maxRate, maxRate);
        m_pitchDelta += glm::clamp(delta.y * RotationSpeed(), -maxRate, maxRate);
        
        m_rightDirection = glm::cross(m_direction, glm::vec3{ 0.f, yawSign, 0.f });
        
        m_direction = glm::rotate(glm::normalize(glm::cross(glm::angleAxis(-m_pitchDelta, m_rightDirection),
                                                            glm::angleAxis(-m_yawDelta, glm::vec3{ 0.f, yawSign, 0.f }))),
                                  m_direction);
        
        const float distance = glm::distance(m_focalPoint, m_position);
        m_focalPoint = m_position + GetForwardDirection() * distance;
        m_distance = distance;
      }
      else if (Input::IsKeyPressed(Key::LeftControl))
      {
        m_cameraMode = CameraMode::ARCBALL;
        
        if (Input::IsMouseButtonPressed(MouseButton::Middle))
        {
          CameraUtils::DisableMouse();
          MousePan(delta);
        }
        else if (Input::IsMouseButtonPressed(MouseButton::Left))
        {
          CameraUtils::DisableMouse();
          MouseRotate(delta);
        }
        else if (Input::IsMouseButtonPressed(MouseButton::Right))
        {
          CameraUtils::DisableMouse();
          MouseZoom(delta.x + delta.y);
        }
        else
        {
          CameraUtils::EnableMouse();
        }
      }
      else
      {
        CameraUtils::EnableMouse();
      }
    }
    m_initialMousePosition = mouse;
    
    m_position += m_positionDelta;
    m_yaw += m_yawDelta;
    m_pitch += m_pitchDelta;
    
    if (m_cameraMode == CameraMode::ARCBALL)
    {
      m_position = CalculatePosition();
    }
    
    UpdateCameraView();
  }
  
  float EditorCamera::GetCameraSpeed() const
  {
    float speed = m_normalSpeed;
    if (Input::IsKeyPressed(Key::LeftControl))
    {
      speed /= 2 - glm::log(m_normalSpeed);
    }
    if (Input::IsKeyPressed(Key::LeftShift))
    {
      speed *= 2 - glm::log(m_normalSpeed);
    }
    
    return glm::clamp(speed, MIN_SPEED, MAX_SPEED);
  }
  
  void EditorCamera::UpdateCameraView()
  {
    const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
    
    // Extra step to handle the problem when the camera direction is the same as the up vector
    const float cosAngle = glm::dot(GetForwardDirection(), GetUpDirection());
    if (cosAngle * yawSign > 0.99f)
    {
      m_pitchDelta = 0.f;
    }
    
    const glm::vec3 lookAt = m_position + GetForwardDirection();
    m_direction = glm::normalize(lookAt - m_position);
    m_distance = glm::distance(m_position, m_focalPoint);
    m_viewMatrix = glm::lookAt(m_position, lookAt, glm::vec3{ 0.f, yawSign, 0.f });
    
    //damping for smooth camera
    m_yawDelta *= 0.6f;
    m_pitchDelta *= 0.6f;
    m_positionDelta *= 0.8f;
  }
  
  void EditorCamera::Focus(const glm::vec3& focusPoint)
  {
    m_focalPoint = focusPoint;
    m_cameraMode = CameraMode::FLYCAM;
    if (m_distance > m_minFocusDistance)
    {
      m_distance -= m_distance - m_minFocusDistance;
      m_position = m_focalPoint - GetForwardDirection() * m_distance;
    }
    m_position = m_focalPoint - GetForwardDirection() * m_distance;
    UpdateCameraView();
  }
  
  std::pair<float, float> EditorCamera::PanSpeed() const
  {
    const float x = glm::min(float(m_viewportWidth) / 1000.0f, 2.4f); // max = 2.4f
    const float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
    
    const float y = glm::min(float(m_viewportHeight) / 1000.0f, 2.4f); // max = 2.4f
    const float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
    
    return { xFactor, yFactor };
  }
  
  float EditorCamera::RotationSpeed() const
  {
    return 0.3f;
  }
  
  float EditorCamera::ZoomSpeed() const
  {
    float distance = m_distance * 0.2f;
    distance = glm::max(distance, 0.0f);
    float speed = distance * distance;
    speed = glm::min(speed, 50.0f); // max speed = 50
    return speed;
  }
  
  void EditorCamera::OnEvent(Event& event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) { return OnMouseScroll(e); });
  }
  
  bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
  {
    if (Input::IsMouseButtonPressed(MouseButton::Right))
    {
      m_normalSpeed += e.GetYOffset() * 0.3f * m_normalSpeed;
      m_normalSpeed = std::clamp(m_normalSpeed, MIN_SPEED, MAX_SPEED);
    }
    else
    {
      MouseZoom(e.GetYOffset() * 0.1f);
      UpdateCameraView();
    }
    
    return true;
  }
  
  void EditorCamera::MousePan(const glm::vec2& delta)
  {
    auto [xSpeed, ySpeed] = PanSpeed();
    m_focalPoint -= GetRightDirection() * delta.x * xSpeed * m_distance;
    m_focalPoint += GetUpDirection() * delta.y * ySpeed * m_distance;
  }
  
  void EditorCamera::MouseRotate(const glm::vec2& delta)
  {
    const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
    m_yawDelta += yawSign * delta.x * RotationSpeed();
    m_pitchDelta += delta.y * RotationSpeed();
  }
  
  void EditorCamera::MouseZoom(float delta)
  {
    m_distance -= delta * ZoomSpeed();
    const glm::vec3 forwardDir = GetForwardDirection();
    m_position = m_focalPoint - forwardDir * m_distance;
    if (m_distance < 1.0f)
    {
      m_focalPoint += forwardDir * m_distance;
      m_distance = 1.0f;
    }
    m_positionDelta += delta * ZoomSpeed() * forwardDir;
  }
  
  void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
  {
    if (m_viewportWidth == width and m_viewportHeight == height)
    {
      return;
    }
    
    SetPerspectiveProjectionMatrix(m_verticalFOV, (float)width, (float)height, m_nearClip, m_farClip);
    m_viewportWidth = width;
    m_viewportHeight = height;
  }
  
  void EditorCamera::SetActive(bool active)
  {
    IK_PERFORMANCE_FUN();
    m_isActive = active;
  }
  
  void EditorCamera::SetDistance(float distance)
  {
    m_distance = distance;
  }
  
  void EditorCamera::SetNormalSpeed(float camSpeed)
  {
    m_normalSpeed = camSpeed;
  }
  
  glm::vec3 EditorCamera::GetUpDirection() const
  {
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
  }
  
  glm::vec3 EditorCamera::GetRightDirection() const
  {
    return glm::rotate(GetOrientation(), glm::vec3(1.f, 0.f, 0.f));
  }
  
  glm::vec3 EditorCamera::GetForwardDirection() const
  {
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
  }
  
  glm::vec3 EditorCamera::CalculatePosition() const
  {
    return m_focalPoint - GetForwardDirection() * m_distance + m_positionDelta;
  }
  
  glm::quat EditorCamera::GetOrientation() const
  {
    return glm::quat(glm::vec3(-m_pitch - m_pitchDelta, -m_yaw - m_yawDelta, 0.0f));
  }
  
  bool EditorCamera::IsActive() const
  {
    return m_isActive;
  }
  
  CameraMode EditorCamera::GetCurrentMode() const
  {
    return m_cameraMode;
  }
  
  float EditorCamera::GetDistance() const
  {
    return m_distance;
  }
  
  const glm::vec3& EditorCamera::GetFocalPoint() const
  {
    return m_focalPoint;
  }
  
  const glm::mat4& EditorCamera::GetViewMatrix() const
  {
    return m_viewMatrix;
  }
  glm::mat4 EditorCamera::GetViewProjection() const
  {
    return GetProjectionMatrix() * m_viewMatrix;
  }
  glm::mat4 EditorCamera::GetUnReversedViewProjection() const
  {
    return GetUnReversedProjectionMatrix() * m_viewMatrix;
  }
  
  const glm::vec3& EditorCamera::GetPosition() const
  {
    return m_position;
  }
  
  float EditorCamera::GetNormalSpeed() const
  {
    return m_normalSpeed;
  }
  
  [[nodiscard]] float EditorCamera::GetVerticalFOV() const
  {
    return m_verticalFOV;
  }
  [[nodiscard]] float EditorCamera::GetAspectRatio() const
  {
    return m_aspectRatio;
  }
  [[nodiscard]] float EditorCamera::GetNearClip() const
  {
    return m_nearClip;
  }
  [[nodiscard]] float EditorCamera::GetFarClip() const
  {
    return m_farClip;
  }
  [[nodiscard]] float EditorCamera::GetPitch() const
  {
    return m_pitch;
  }
  [[nodiscard]] float EditorCamera::GetYaw() const
  {
    return m_yaw;
  }
  
} // namespace IKan
