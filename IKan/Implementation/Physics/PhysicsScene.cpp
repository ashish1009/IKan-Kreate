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
    
    // Create the world settings
    PhysicsWorld::WorldSettings settings;
    settings.defaultVelocitySolverNbIterations = m_settings.solverVelocityIterations;
    settings.defaultPositionSolverNbIterations = m_settings.solverPositionIterations;
    settings.isSleepingEnabled = m_settings.isAllowSleep;
    settings.gravity = Vector3(m_settings.gravity.x, m_settings.gravity.y, m_settings.gravity.z);
    
    // Create the physics world with your settings
    m_world = m_common.createPhysicsWorld(settings);
    
    // Debug Renderer
    m_world->setIsDebugRenderingEnabled(true);
    
    // Get a reference to the debug renderer
    DebugRenderer& debugRenderer = m_world->getDebugRenderer();
    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE, true);
    
    // Override contact listener
    m_world->setEventListener(&m_contactListener);
  }
  
  PhysicsScene::~PhysicsScene()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Physics, "Destroying Physics Scene");
    m_common.destroyPhysicsWorld(m_world);
  }

} // namespace IKan
