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
    const auto& window = Application::Get().GetWindow();
    m_windowSize = {window.GetWidth(), window.GetHeight()};
    m_halfWindowHeight = m_windowSize.y / 2.0f;
    m_centerPosition = m_windowSize / 2.0f;
  }
  
  CameraController::CameraController(const CameraController& other)
  {
    IK_PROFILE();
    m_followEntity = other.m_followEntity;
    
    m_centerPosition = other.m_centerPosition;
    m_windowSize = other.m_windowSize;
    m_halfWindowHeight = other.m_halfWindowHeight;
    
    m_anglePerMouseMoveX = other.m_anglePerMouseMoveX;
    m_sensitivity = other.m_sensitivity;
    
    m_topOrbit = other.m_topOrbit;
    m_midOrbit = other.m_midOrbit;
    m_bottomOrbit = other.m_bottomOrbit;
    
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
    
    m_anglePerMouseMoveX = m_sensitivity / m_windowSize.x;
    
    auto& tc = m_entity.GetComponent<TransformComponent>();
    
    Entity followEntity = m_scene->TryGetEntityWithUUID(m_followEntity);
    if (followEntity)
    {
      switch (m_cameraViewType)
      {
        case ViewType::TPP:     UpdateTPP(followEntity); break;
        case ViewType::FPP:     UpdateFPP(followEntity); break;
        case ViewType::FlyCam:
        default: break;
      }
    } // if Follow entity
    
    m_position = tc.Position();
    
    UpdateVectors();
    tc.UpdateTransform(glm::inverse(glm::lookAt(m_position, m_position + m_frontVector, m_upVector)));
  }
  
  void CameraController::EventHandler(Event& event)
  {
    
  }
  
  void CameraController::UpdateVectors()
  {
    IK_PERFORMANCE("CameraController::UpdateVectors");
    Entity followEntity = m_scene->TryGetEntityWithUUID(m_followEntity);
    if (followEntity)
    {
      const auto& followPos = followEntity.GetComponent<TransformComponent>().Position();
      m_frontVector = glm::normalize(followPos - m_position);
      m_rightVector = glm::normalize(glm::cross(m_frontVector, m_worldUpVector));
      m_upVector    = glm::normalize(glm::cross(m_rightVector, m_frontVector));
    }
  }
  
  void CameraController::ResetView()
  {
    IK_PROFILE();
    auto& tc = m_entity.GetComponent<TransformComponent>();
    
    Entity followEntity = m_scene->TryGetEntityWithUUID(m_followEntity);
    if (followEntity)
    {
      const auto& followPos = followEntity.GetComponent<TransformComponent>().Position();
      tc.UpdatePosition({followPos.x, followPos.y + m_midOrbit.height, followPos.z + m_midOrbit.radius});
      
      m_frontVector = glm::normalize(followPos - m_position);
      m_rightVector = glm::normalize(glm::cross(m_frontVector, m_worldUpVector));
      m_upVector    = glm::normalize(glm::cross(m_rightVector, m_frontVector));
      m_position = tc.Position();
      tc.UpdateTransform(glm::inverse(glm::lookAt(m_position, m_position + m_frontVector, m_upVector)));
    }    
  }
  
  void CameraController::UpdateTPP(Entity followEntity)
  {
    if (followEntity.HasComponent<MeshComponent>())
    {
      // Disable mesh rendering for FPP
      followEntity.GetComponent<MeshComponent>().enable = true;
    }
    
    auto& tc = m_entity.GetComponent<TransformComponent>();
    const auto& followPos = followEntity.GetComponent<TransformComponent>().Position();
    
    // Radius of orbit that mouse will follow around Y Axis
    static float currentRadius = m_midOrbit.radius;
    
    // Mouse Move delta
    const glm::vec2 mouse{ Input::GetMouseX(), Input::GetMouseY() };
    glm::vec2 delta = (mouse - m_centerPosition);
    
    // X Mouse Move -----------------------------------------------
    float angleMouseMovedAroundYAxis = glm::radians(delta.x * m_anglePerMouseMoveX);
    tc.UpdatePosition(TransformComponent::Axis::X, followPos.x + (currentRadius * glm::sin(angleMouseMovedAroundYAxis)));
    tc.UpdatePosition(TransformComponent::Axis::Z, followPos.z + (currentRadius * glm::cos(angleMouseMovedAroundYAxis)));
    
    // Y Mouse Move -----------------------------------------------
    static float radiusPerMouseMoveY = 0.0f;
    static float heightPerMouseMoveY = 0.0f;
    if (delta.y > 0.0f)
    {
      heightPerMouseMoveY = (m_topOrbit.height - m_midOrbit.height) / m_halfWindowHeight;
      radiusPerMouseMoveY = (m_topOrbit.radius - m_midOrbit.radius) / m_halfWindowHeight;
    }
    else
    {
      heightPerMouseMoveY = (m_midOrbit.height - m_bottomOrbit.height) / m_halfWindowHeight;
      radiusPerMouseMoveY = (m_midOrbit.radius - m_bottomOrbit.radius) / m_halfWindowHeight;
    }
    
    // Uptate the position offset for Y Axis
    float positionOffsetY = followPos.y + m_midOrbit.height + (delta.y * heightPerMouseMoveY);
    
    // Update the current tracing radius of Camera
    currentRadius = m_midOrbit.radius + (delta.y * radiusPerMouseMoveY);
    
    // Check the Max and min height limit to limit the mouse movement
    if (positionOffsetY > followPos.y + m_topOrbit.height)
    {
      m_centerPosition.y = m_halfWindowHeight + (mouse.y - m_windowSize.y);
      positionOffsetY = followPos.y + m_topOrbit.height;
      currentRadius = m_topOrbit.radius;
    }
    else if (positionOffsetY < followPos.y + m_bottomOrbit.height)
    {
      m_centerPosition.y = m_halfWindowHeight - (0 - mouse.y);
      positionOffsetY = followPos.y + m_bottomOrbit.height;
      currentRadius = m_bottomOrbit.radius;
    }
    
    // Update the Y position of camera
    tc.UpdatePosition(TransformComponent::Axis::Y, positionOffsetY);
  }
  
  void CameraController::UpdateFPP(Entity followEntity)
  {
    if (followEntity.HasComponent<MeshComponent>())
    {
      // Disable mesh rendering for FPP
      followEntity.GetComponent<MeshComponent>().enable = false;
      
      // Set the camera position as current entity positon
      auto& tc = m_entity.GetComponent<TransformComponent>();
      auto followPos = followEntity.GetComponent<TransformComponent>().Position();
      
      // Hack to fix z position 0
      if (followPos.z == 0.0f)
      {
        followPos.z -= 0.0001f;
      }
      tc.UpdatePosition(followPos);
    }
  }
  
  void CameraController::SetFollowEntity(UUID uuid)
  {
    m_followEntity = uuid;
    ResetView();
  }
  UUID CameraController::GetFollowEntity() const
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
