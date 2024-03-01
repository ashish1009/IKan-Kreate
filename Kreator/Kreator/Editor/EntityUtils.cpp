//
//  EntityUtils.cpp
//  Kreator
//
//  Created by Ashish . on 29/02/24.
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
    if (ImGui::MenuItem("Mesh"))
    {
      newEntity = scene->CreateEntity("Mesh");
      auto& meshComp = newEntity.AddComponent<MeshComponent>();
      meshComp.mesh = 0;
      meshComp.materialTable->SetMaterial(0, AssetManager::GetAsset<MaterialAsset>("Materials/Default.ikmat"));
    }
    ImGui::Separator();
    
    static const std::filesystem::path DefaultMeshFile = "Meshes/Default/";
    auto menuForDefaultMesh = [scene](Entity& newEntity, const std::string& name) {
      if (ImGui::MenuItem(name.c_str()))
      {
        newEntity = scene->CreateEntity(name);
        std::filesystem::path filePath = DefaultMeshFile / std::string(name + ".fbx");
        auto& mc = newEntity.AddComponent<MeshComponent>(AssetManager::GetAsset<Mesh>(filePath)->handle);
        mc.materialTable->SetMaterial(0, AssetManager::GetAsset<MaterialAsset>("Materials/Default.ikmat"));
      }
    };
    
    menuForDefaultMesh(newEntity, "Cube");
    menuForDefaultMesh(newEntity, "Sphere");
    menuForDefaultMesh(newEntity, "Cylinder");
    
    if (newEntity and parent)
    {
      scene->ParentEntity(newEntity, parent);
    }
    
    return newEntity;
  }
  
  void UpdateChildrenTransform(Ref<Scene> scene, Entity entity, const glm::vec3& deltaPosition, const glm::vec3& deltaScale, const glm::vec3& deltaRotation, bool controlledByParent)
  {
    TransformComponent& entityTransform = entity.GetTransform();
    entityTransform.UpdatePosition(entityTransform.Position() + deltaPosition);
    entityTransform.UpdateScale(entityTransform.Scale() + deltaScale);
    entityTransform.UpdateRotation(entityTransform.Rotation() + deltaRotation);

    Entity parent = entity.GetFirstParent();
    if (parent and controlledByParent)
    {
      const auto& parentTc = parent.GetTransform();
      auto& tc = entity.GetTransform();
      const auto& ePos = tc.Position();
      const auto& parPos = parentTc.Position();

      if (deltaRotation.y != 0.0f)
      {
        float distance = Utils::Math::GetDistance(ePos.x, ePos.z, parPos.x, parPos.z);

        float temp = (parentTc.Position().z - ePos.z) / distance;
        float angle = (ePos.x > parentTc.Position().x) ? asin(temp) : M_PI - asin(temp);

        tc.UpdatePosition(TransformComponent::Axis::X, parentTc.Position().x + (distance * glm::cos(angle + deltaRotation.y)));
        tc.UpdatePosition(TransformComponent::Axis::Z, parentTc.Position().z - (distance * glm::sin(angle + deltaRotation.y)));
      }
    }
    else
    {
    }

    for (const auto& child : entity.Children())
    {
      Entity childEntity = scene->TryGetEntityWithUUID(child);
      UpdateChildrenTransform(scene, childEntity, deltaPosition, deltaScale, deltaRotation, true);
    }
  }
} // namespace Kreator::ECS_Utils
