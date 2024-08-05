//
//  EntityUtils.cpp
//  Kreator
//
//  Created by Ashish . on 02/08/24.
//

#include "EntityUtils.hpp"

namespace Kreator::ECS_Utils
{
  Entity DrawCreateEntityMenu(Ref<Scene> scene, Entity parent)
  {
    Entity newEntity;
    if (ImGui::MenuItem("Empty Entity"))
    {
      newEntity = scene->CreateEntity("Empty Entity");
    }

    if (ImGui::MenuItem("Camera"))
    {
      newEntity = scene->CreateEntity("Camera");
      auto& cc = newEntity.AddComponent<CameraComponent>();
      cc.camera.SetProjectionType(IKan::SceneCamera::ProjectionType::Perspective);
    }

    if (newEntity and parent)
    {
      IK_ASSERT(false, "Implement later");
    }
    return newEntity;
  }
} // namespace Kreator::ECS_Utils
