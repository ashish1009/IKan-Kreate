//
//  PlayerController.hpp
//  Kreator
//
//  Created by Ashish . on 23/04/24.
//

#pragma once

namespace Kreator
{
  class PlayerController : public ScriptableEntity
  {
  public:
    /// Default Constructor of Player controller
    PlayerController();
    /// Default Desructor of Player controller
    virtual ~PlayerController();
    
    // Documentation @ScriptableEntity
    void Create(Entity entity) override;
    void Update(TimeStep ts) override;
    void RenderGui() override;
    void Copy(void* script) override;
    void EventHandler(Event& event) override;
    void OnContact(Entity* collidedEntity, const reactphysics3d::Vector3& worldContactPoint) override;
    void OnTrigger(Entity* collidedEntity, const reactphysics3d::Vector3& worldContactPoint) override;
    
  private:
    bool OnKeyPressed(KeyPressedEvent& e);
    void KeyUpdate(TimeStep ts, Key up, Key down, Key left, Key right);
    void MoveRigidBody(TimeStep ts, Key up, Key down, Key left, Key right);
    void MoveEntity(TimeStep ts, Key up, Key down, Key left, Key right);
    
    Entity m_cameraEntity {};
    
    // Controller
    enum class KeyController {ArrowKeys, WSAD};
    KeyController m_keyController = KeyController::WSAD;
    reactphysics3d::RigidBody* m_body;
    
    bool m_onGround = false;
    float m_speed = 10.0f;
  };
} // namespace Kreator
