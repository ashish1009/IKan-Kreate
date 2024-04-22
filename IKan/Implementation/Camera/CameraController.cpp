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
    
    float sensitivityRadians = glm::radians(m_sensitivity);
    m_angleFactor = (float)(sensitivityRadians * 2) / m_windowSize;
  }
  
  CameraController::CameraController(const CameraController& other)
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::SceneCamera, "Copying Camera controller for scene camera");
    
    m_followEntityID = other.m_followEntityID;

    m_cameraViewType = other.m_cameraViewType;
    
    m_topOrbit = other.m_topOrbit;
    m_midOrbit = other.m_midOrbit;
    m_bottomOrbit = other.m_bottomOrbit;
    
    m_sensitivity = other.m_sensitivity;
    m_angleFactor = other.m_angleFactor;

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
    
  void CameraController::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("CameraController::OnUpdate");
    
    if (m_followEntity)
    {
      // Mouse Move delta
      m_mousePos = { Input::GetMouseX(), Input::GetMouseY() };
            
      // Update the mouse delta based on center position
      m_mouseDelta = (m_mousePos - m_centrePosition);
      
      // Get the angle moved with mouse position
      m_angleMovedAroundYAxis = m_mouseDelta.x * m_angleFactor.x;
      m_angleMovedAroundXAxis = m_mouseDelta.y * m_angleFactor.y;

      // Update the camera
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
    } // if followEntity
    else
    {
      if (m_followEntityID != 0)
      {
        m_followEntity = m_scene->TryGetEntityWithUUID(m_followEntityID);
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

    float sensitivityRadians = glm::radians(m_sensitivity);
    m_angleFactor = (float)(sensitivityRadians * 2) / m_windowSize;

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

    auto& tc = m_entity.GetComponent<TransformComponent>();
    const auto& followPos = m_followEntity.GetComponent<TransformComponent>().Position();

    // Radius of orbit that mouse will follow around Y Axis
    static float currentRadius = m_midOrbit.radius;

    // X Mouse Move ---
    float xAxisDelta = followPos.x + (currentRadius * glm::sin(m_angleMovedAroundYAxis));
    float zAxisDelta = followPos.z + (currentRadius * glm::cos(m_angleMovedAroundYAxis));

    // Y Mouse Move ---
    if (m_mouseDelta.y > 0.0f)
    {
      m_heightPerMouseMoveY = (m_topOrbit.height - m_midOrbit.height) / m_windowHalfHeight;
      m_radiusPerMouseMoveY = (m_topOrbit.radius - m_midOrbit.radius) / m_windowHalfHeight;
    }
    else
    {
      m_heightPerMouseMoveY = (m_midOrbit.height - m_bottomOrbit.height) / m_windowHalfHeight;
      m_radiusPerMouseMoveY = (m_midOrbit.radius - m_bottomOrbit.radius) / m_windowHalfHeight;
    }

    // Uptate the position offset for Y Axis
    float yAxisDelta = followPos.y + m_midOrbit.height + (m_mouseDelta.y * m_heightPerMouseMoveY);
    
    // Update the current tracing radius of Camera
    currentRadius = m_midOrbit.radius + (m_mouseDelta.y * m_radiusPerMouseMoveY);

    // Check the Max and min height limit to limit the mouse movement
    if (yAxisDelta > followPos.y + m_topOrbit.height)
    {
      m_centrePosition.y = m_windowHalfHeight + (m_mousePos.y - m_windowSize.y);
      yAxisDelta = followPos.y + m_topOrbit.height;
      currentRadius = m_topOrbit.radius;
    }
    else if (yAxisDelta < followPos.y + m_bottomOrbit.height)
    {
      m_centrePosition.y = m_windowHalfHeight - (0 - m_mousePos.y);
      yAxisDelta = followPos.y + m_bottomOrbit.height;
      currentRadius = m_bottomOrbit.radius;
    }
    
    // Update the position of camera
    tc.UpdatePosition({xAxisDelta, yAxisDelta, zAxisDelta});

    m_position = tc.Position();
  }
  
  void CameraController::UpdateFPP()
  {
    IK_PERFORMANCE("CameraController::UpdateFPP");
    if (m_followEntity.HasComponent<MeshComponent>())
    {
      // Disable mesh rendering for FPP
      m_followEntity.GetComponent<MeshComponent>().enable = false;
    }
    
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
    
    float sensitivityRadians = glm::radians(m_sensitivity);
    m_angleFactor = (float)(sensitivityRadians * 2) / m_windowSize;
  }
  void CameraController::SetCameraViewType(CameraController::ViewType value)
  {
    m_cameraViewType = value;
    ResetView();
  }

} // namespace IKan
