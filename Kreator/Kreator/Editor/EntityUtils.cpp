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
    ImGui::Separator();

    if (ImGui::MenuItem("Camera"))
    {
      newEntity = scene->CreateEntity("Camera");
      auto& cc = newEntity.AddComponent<CameraComponent>();
      cc.camera.SetProjectionType(IKan::SceneCamera::ProjectionType::Perspective);
    }
    ImGui::Separator();

    if (ImGui::MenuItem("Mesh"))
    {
      newEntity = scene->CreateEntity("Mesh");
      auto& meshComp = newEntity.AddComponent<MeshComponent>();
      meshComp.mesh = 0;
    }

    ImGui::Separator();
    static const std::filesystem::path DefaultMeshFile = "Meshes/Default/";
    auto menuForDefaultMesh = [scene](Entity& newEntity, const std::string& name) {
      if (ImGui::MenuItem(name.c_str()))
      {
        newEntity = scene->CreateEntity(name);
        std::filesystem::path filePath = DefaultMeshFile / std::string(name + ".fbx");
        
        newEntity.AddComponent<MeshComponent>(AssetManager::GetAsset<Mesh>(filePath)->handle);
      }
    };
    
    menuForDefaultMesh(newEntity, "Cube");
    menuForDefaultMesh(newEntity, "Sphere");
    menuForDefaultMesh(newEntity, "Cylinder");
    
    ImGui::Separator();

    if (newEntity and parent)
    {
      IK_ASSERT(false, "Implement later");
    }
    return newEntity;
  }
} // namespace Kreator::ECS_Utils
