//
//  PhysicsScene.cpp
//  IKan
//
//  Created by Ashish . on 02/03/24.
//

#include "PhysicsScene.hpp"
#include "Scene/Scene.hpp"

using namespace reactphysics3d;

namespace IKan
{
  PhysicsScene::PhysicsScene(const PhysicsSettings& setting, Scene* scene)
  : m_settings(setting), m_scene(scene)
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Physics, "Creating Physics Scene");
    IK_LOG_TRACE(LogModule::Physics, "  Gravity {0} | {1} | {2}", setting.gravity.x, setting.gravity.y, setting.gravity.z);
  }
  
  PhysicsScene::~PhysicsScene()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Physics, "Destroying Physics Scene");
    m_common.destroyPhysicsWorld(m_world);
  }

} // namespace IKan
