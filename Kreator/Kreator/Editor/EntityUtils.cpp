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
    if (ImGui::MenuItem("Camera"))
    {
      newEntity = scene->CreateEntity("Camera");
      auto& cc = newEntity.AddComponent<CameraComponent>();
      cc.camera.SetProjectionType(IKan::SceneCamera::ProjectionType::Perspective);
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
        
        newEntity.AddComponent<RigidBodyComponent>();
        if (name == "Cube")
        {
          newEntity.AddComponent<Box3DColliderComponent>();
        }
        else if (name == "Sphere")
        {
          newEntity.AddComponent<SphereColliderComponent>();
        }
        else if (name == "Cylinder")
        {
          newEntity.AddComponent<CapsuleColliderComponent>();
        }
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
  
  void UpdateChildrenTransform(Ref<Scene> scene, Entity entity, const glm::vec3& deltaPosition, const glm::vec3& deltaScale, const glm::vec3& deltaRotation, Entity controllerParent)
  {
    TransformComponent& entityTransform = entity.GetTransform();
    if (deltaPosition.x or deltaPosition.y or deltaPosition.z)
    {
      entityTransform.UpdatePosition(entityTransform.Position() + deltaPosition);
    }
    if (deltaScale.x or deltaScale.y or deltaScale.z)
    {
      entityTransform.UpdateScale(entityTransform.Scale() + deltaScale);
    }
    if (deltaRotation.x or deltaRotation.y or deltaRotation.z)
    {
      entityTransform.UpdateRotation(entityTransform.Rotation() + deltaRotation);
    }
    
    if (controllerParent and controllerParent != entity)
    {
      const auto& parentTc = controllerParent.GetTransform();
      auto& tc = entity.GetTransform();
      const auto& ePos = tc.Position();
      const auto& pPos = parentTc.Position();

      // Rotation ------------------------
      {
        // X - Axis
        if (deltaRotation.x != 0.0f)
        {
          float distanceYZ = Utils::Math::GetDistance(ePos.y, ePos.z, pPos.y, pPos.z);
          if (distanceYZ)
          {
            float mlDistanceYZ = (ePos.z - parentTc.Position().z) / distanceYZ;
            float angleXZOffset = (ePos.y > parentTc.Position().y) ? asin(mlDistanceYZ) : M_PI - asin(mlDistanceYZ);
            
            float xPos = ePos.x;
            float yPos = parentTc.Position().y + (distanceYZ * glm::cos(angleXZOffset + deltaRotation.x));
            float zPos = parentTc.Position().z + (distanceYZ * glm::sin(angleXZOffset + deltaRotation.x));
            
            tc.UpdatePosition({xPos, yPos, zPos});
          }
        }
        
        // Y - Axis
        if (deltaRotation.y != 0.0f)
        {
          float distanceXZ = Utils::Math::GetDistance(ePos.x, ePos.z, pPos.x, pPos.z);
          if (distanceXZ)
          {
            float mlDistanceXZ = (parentTc.Position().z - ePos.z) / distanceXZ;
            float angleXZOffset = (ePos.x > parentTc.Position().x) ? asin(mlDistanceXZ) : M_PI - asin(mlDistanceXZ);
            
            float xPos = parentTc.Position().x + (distanceXZ * glm::cos(angleXZOffset + deltaRotation.y));
            float yPos = ePos.y;
            float zPos = parentTc.Position().z - (distanceXZ * glm::sin(angleXZOffset + deltaRotation.y));
            
            tc.UpdatePosition({xPos, yPos, zPos});
          }
        }
        
        // Z - Axis
        if (deltaRotation.z != 0.0f)
        {
          float distanceXY = Utils::Math::GetDistance(ePos.x, ePos.y, pPos.x, pPos.y);
          if (distanceXY)
          {
            float mlDistanceXY = (ePos.x - parentTc.Position().x) / distanceXY;
            float angleXYOffset = (ePos.y > parentTc.Position().y) ? acos(mlDistanceXY) : (2 * M_PI) - acos(mlDistanceXY);
            
            float xPos = parentTc.Position().x + (distanceXY * glm::cos(angleXYOffset + deltaRotation.z));
            float yPos = parentTc.Position().y + (distanceXY * glm::sin(angleXYOffset + deltaRotation.z));
            float zPos = ePos.z;
            
            tc.UpdatePosition({xPos, yPos, zPos});
          }
        }
      } // Rotation
      
      // Scale ---------------------
      {
        // X - Axis
        if (deltaScale.x != 0.0f)
        {
        }
        
        // Y - Axis
        if (deltaScale.y != 0.0f)
        {
        }
        
        // Z - Axis
        if (deltaScale.z != 0.0f)
        {
        }
      }
    }

    for (const auto& child : entity.Children())
    {
      Entity childEntity = scene->TryGetEntityWithUUID(child);
      UpdateChildrenTransform(scene, childEntity, deltaPosition, deltaScale, deltaRotation, controllerParent);
    }
  }
} // namespace Kreator::ECS_Utils
