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
} // namespace Kreator::ECS_Utils
