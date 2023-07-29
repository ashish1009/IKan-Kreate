//
//  Scene.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Scene.hpp"

namespace IKan
{
  Scene::Scene()
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Scene, "Creating Scene ...");  }

  Scene::~Scene()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Scene, "Destroying Scene!!!");
  }
  
  /// This function creates the instance of EnTT Scene
  Ref<Scene> Scene::Create()
  {
    return CreateRef<Scene>();
  }
} // namespace IKan
