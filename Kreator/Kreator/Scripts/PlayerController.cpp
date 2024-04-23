//
//  PlayerController.cpp
//  Kreator
//
//  Created by Ashish . on 23/04/24.
//

#include "PlayerController.hpp"
#include "UI/ImGuiProperty.hpp"

namespace Kreator
{
  PlayerController::PlayerController()
  {
    
  }
  
  PlayerController::~PlayerController()
  {
    
  }
  
  void PlayerController::Create(Entity entity)
  {
    m_entity = entity;
    m_cameraEntity = m_scene->GetMainCameraEntity();
    
    const auto& rbc = m_entity.GetComponent<RigidBodyComponent>();
    m_body = (reactphysics3d::RigidBody*)rbc.runtimeBody;
  }
  
  void PlayerController::Update(TimeStep ts)
  {
    RETURN_IF(!m_cameraEntity);
    if (m_keyController == KeyController::ArrowKeys)
    {
      KeyUpdate(ts, Key::Up, Key::Down, Key::Left, Key::Right);
    }
    else
    {
      KeyUpdate(ts, Key::W, Key::S, Key::A, Key::D);
    }
    
    // Check on ground
    {
      m_onGround = m_body->getLinearVelocity().y == 0.0f;
    }
  }
  
  void PlayerController::KeyUpdate(TimeStep ts, Key up, Key down, Key left, Key right)
  {
    if (m_entity.HasComponent<RigidBodyComponent>())
    {
      MoveRigidBody(ts, up, down, left, right);
    }
    else
    {
      MoveEntity(ts, up, down, left, right);
    }
  }
  
  void PlayerController::MoveRigidBody(TimeStep ts, Key up, Key down, Key left, Key right)
  {
    const auto& cc = m_cameraEntity.GetComponent<CameraComponent>();
    
    const glm::vec3& frontVector = cc.controller.GetFrontVector();
    const glm::vec3& rightVector = cc.controller.GetRightVector();
    
    if (Input::IsKeyPressed(up))
    {
      reactphysics3d::Vector3 force = {frontVector.x, frontVector.y, frontVector.z};
      m_body->applyWorldForceAtCenterOfMass(force * m_speed);
    }
    if (Input::IsKeyPressed(down))
    {
      reactphysics3d::Vector3 force = {frontVector.x, frontVector.y, frontVector.z};
      m_body->applyWorldForceAtCenterOfMass(force * -m_speed);
    }
    if (Input::IsKeyPressed(right))
    {
      reactphysics3d::Vector3 force = {rightVector.x, rightVector.y, rightVector.z};
      m_body->applyWorldForceAtCenterOfMass(force * m_speed);
    }
    if (Input::IsKeyPressed(left))
    {
      reactphysics3d::Vector3 force = {rightVector.x, rightVector.y, rightVector.z};
      m_body->applyWorldForceAtCenterOfMass(force * -m_speed);
    }
  }
  
  void PlayerController::MoveEntity(TimeStep ts, Key up, Key down, Key left, Key right)
  {
    IK_ASSERT(false);
    if (Input::IsKeyPressed(up))
    {
      
    }
    if (Input::IsKeyPressed(down))
    {
      
    }
    if (Input::IsKeyPressed(right))
    {
      
    }
    if (Input::IsKeyPressed(left))
    {
      
    }
  }
  
  void PlayerController::RenderGui()
  {
    UI::BeginPropertyGrid();
    UI::PropertyDropdown("Key Controller", {"Arrow Key", "WSAD"}, 2, (int*)(&m_keyController));
    UI::Property("Speed", m_speed);
    UI::EndPropertyGrid();
  }
  
  void PlayerController::Copy(void* script)
  {
    RETURN_IF(!script)
    PlayerController* otherScript = reinterpret_cast<PlayerController*>(script);
    IK_ASSERT(otherScript);
    
    m_speed = otherScript->m_speed;
    m_keyController = otherScript->m_keyController;
  }
  
  void PlayerController::EventHandler(Event& event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) { return OnKeyPressed(e); });
  }
  
  bool PlayerController::OnKeyPressed(KeyPressedEvent& e)
  {
    const auto& rbc = m_entity.GetComponent<RigidBodyComponent>();
    reactphysics3d::RigidBody* body = (reactphysics3d::RigidBody*)rbc.runtimeBody;
    
    if (e.GetKeyCode() == Key::Space and m_onGround)
    {
      body->applyWorldForceAtCenterOfMass({0, 1000, 0});
    }
    return false;
  }
  
  void PlayerController::OnContact(Entity* collidedEntity, const reactphysics3d::CollisionCallback::ContactPoint& contactPoint)
  {
    
  }
  
  void PlayerController::OnTrigger(Entity* collidedEntity, const reactphysics3d::CollisionCallback::ContactPoint& contactPoint)
  {
    
  }
} // namespace Kreator
