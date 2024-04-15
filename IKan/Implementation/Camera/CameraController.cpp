//
//  CameraController.cpp
//  IKan
//
//  Created by Ashish . on 03/03/24.
//

#include "CameraController.hpp"
#include "Core/Application.hpp"
#include "Core/Input.hpp"
#include "Scene/Components.hpp"
#include "Scene/Scene.hpp"

namespace IKan
{
  CameraController::CameraController()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::SceneCamera, "Creating Camera controller for scene camera");
    
    const auto& window = Application::Get().GetWindow();
    m_windowSize = {window.GetWidth(), window.GetHeight()};
    m_windowHalfHeight = m_windowSize.y / 2.0f;
    m_centrePosition = m_windowSize / 2.0f;
  }
  
  CameraController::CameraController(const CameraController& other)
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::SceneCamera, "Copying Camera controller for scene camera");
    
    m_followEntity = other.m_followEntity;
    m_followEntityID = other.m_followEntityID;
    
    m_cameraViewType = other.m_cameraViewType;
    
    m_topOrbit = other.m_topOrbit;
    m_midOrbit = other.m_midOrbit;
    m_bottomOrbit = other.m_bottomOrbit;
    
    m_sensitivity = other.m_sensitivity;
    
    m_centrePosition = other.m_centrePosition;
    m_windowSize = other.m_windowSize;
    m_windowHalfHeight =  other.m_windowHalfHeight;
    
    // To add other required to copy date here
  }
  
  void CameraController::Initialize(Entity entity, Scene* scene)
  {
    IK_PROFILE();
    m_entity = entity;
    m_scene = scene;
  }
  
  static glm::vec2 extracted(const glm::vec2 &m_centerPosition) {
    return m_centerPosition;
  }
  
  void CameraController::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("CameraController::OnUpdate");
    
    if (m_followEntity)
    {
      // Mouse Move delta
      m_mousePos = { Input::GetMouseX(), Input::GetMouseY() };
      
      // Shift the center position of Y Axis as we need not to wrap the camera if mouse goes out of window
      if (m_mousePos.y < 0.0f)
      {
        m_centrePosition.y = m_windowHalfHeight + m_mousePos.y;
      }
      else if (m_mousePos.y > m_windowSize.y)
      {
        m_centrePosition.y = m_mousePos.y - m_windowHalfHeight;
      }
      else
      {
        m_centrePosition.y = m_windowHalfHeight;
      }
      
      m_mouseDelta = (m_mousePos - extracted(m_centrePosition));
      IK_LOG_INFO("", "{0}, {1}, {2}", m_mouseDelta.x, m_mouseDelta.y, m_centrePosition.y);
            
      switch (m_cameraViewType)
      {
        case ViewType::TPP:
        {
          UpdateTPP();
          break;
        }
        case ViewType::FPP:
        {
          UpdateFPP();
          break;
        }
        default:
          IK_ASSERT("Invalid View Type");
      }
    }
  }
  
  void CameraController::EventHandler(Event& event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(IK_BIND_EVENT_FN(CameraController::WindowResize));
  }
  
  bool CameraController::WindowResize(WindowResizeEvent &windowResizeEvent)
  {
    m_windowSize = {windowResizeEvent.GetWidth(), windowResizeEvent.GetHeight()};
    m_windowHalfHeight = m_windowSize.y / 2.0f;
    m_centrePosition = m_windowSize / 2.0f;
    return false;
  }

  void CameraController::UpdateTPP()
  {
    IK_PERFORMANCE("CameraController::UpdateTPP");
    if (m_followEntity.HasComponent<MeshComponent>())
    {
      // Disable mesh rendering for FPP
      m_followEntity.GetComponent<MeshComponent>().enable = true;
    }
  }
  
  void CameraController::UpdateFPP()
  {
    IK_PERFORMANCE("CameraController::UpdateFPP");
    if (m_followEntity.HasComponent<MeshComponent>())
    {
      // Disable mesh rendering for FPP
      m_followEntity.GetComponent<MeshComponent>().enable = false;
    }
  }
  
  void CameraController::ResetView()
  {
    IK_PROFILE();
    auto& tc = m_entity.GetComponent<TransformComponent>();
    if (m_followEntity)
    {
      const auto& followPos = m_followEntity.GetComponent<TransformComponent>().Position();
      
      switch (m_cameraViewType)
      {
        case ViewType::TPP:
        {
          tc.UpdatePosition({followPos.x, followPos.y + m_midOrbit.height, followPos.z + m_midOrbit.radius});
          if (m_followEntity.HasComponent<MeshComponent>())
          {
            // Disable mesh rendering for FPP
            m_followEntity.GetComponent<MeshComponent>().enable = true;
          }
          break;
        }
        case ViewType::FPP:
        {
          tc.UpdatePosition(followPos);
          if (m_followEntity.HasComponent<MeshComponent>())
          {
            // Disable mesh rendering for FPP
            m_followEntity.GetComponent<MeshComponent>().enable = false;
          }
          break;
        }
        default: 
          IK_ASSERT("Invalid View Type");
      }
    }
  }
  
  void CameraController::SetFollowEntity(UUID uuid)
  {
    m_followEntityID = uuid;
    m_followEntity = m_scene->TryGetEntityWithUUID(m_followEntityID);
    ResetView();
  }
  UUID CameraController::GetFollowEntityID() const
  {
    return m_followEntityID;
  }
  Entity CameraController::GetFollowEntity() const
  {
    return m_followEntity;
  }

  const Entity& CameraController::GetEntity() const
  {
    return m_entity;
  }
  
  const glm::vec3& CameraController::GetFrontVector() const
  {
    return m_frontVector;
  }
  const glm::vec3& CameraController::GetUpVector() const
  {
    return m_upVector;
  }
  const glm::vec3& CameraController::GetRightVector() const
  {
    return m_rightVector;
  }
  const glm::vec3& CameraController::GetPosition() const
  {
    return m_position;
  }
  
  const CameraController::Orbit& CameraController::GetTopOrbit() const
  {
    return m_topOrbit;
  }
  const CameraController::Orbit& CameraController::GetMidOrbit() const
  {
    return m_midOrbit;
  }
  const CameraController::Orbit& CameraController::GetBottomOrbit() const
  {
    return m_bottomOrbit;
  }
  
  float CameraController::GetSensitivity() const
  {
    return m_sensitivity;
  }
  
  CameraController::ViewType CameraController::GetCameraViewType() const
  {
    return m_cameraViewType;
  }
  
  void CameraController::SetTopOrbit(const CameraController::Orbit&  value)
  {
    m_topOrbit = value;
    ResetView();
  }
  void CameraController::SetMidOrbit(const CameraController::Orbit&  value)
  {
    m_midOrbit = value;
    ResetView();
  }
  void CameraController::SetBottomOrbit(const CameraController::Orbit&  value)
  {
    m_bottomOrbit = value;
    ResetView();
  }
  
  void CameraController::SetSensitivity(float value)
  {
    m_sensitivity = value;
  }
  void CameraController::SetCameraViewType(CameraController::ViewType value)
  {
    m_cameraViewType = value;
    ResetView();
  }

} // namespace IKan
