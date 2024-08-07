//
//  EntitySerializer.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "EntitySerializer.hpp"

#include "Scene/Entity.hpp"
#include "Scene/Component.hpp"
#include "Assets/AssetManager.hpp"

namespace IKan
{
  void EntitySerializer::SerializeEntity(YAML::Emitter& out, Entity entity)
  {
    IK_PROFILE();
    UUID uuid = entity.GetComponent<IDComponent>().ID;
    out << YAML::BeginMap; // Entity
    out << YAML::Key << "Entity";
    out << YAML::Value << uuid;
    
    if (entity.HasComponent<VisibilityComponent>())
    {
      out << YAML::Key << "VisibilityComponent";
      out << YAML::BeginMap; // VisibilityComponent
      
      out << YAML::Key << "Visibility" << YAML::Value << entity.GetComponent<VisibilityComponent>().isVisible;
      
      out << YAML::EndMap; // VisibilityComponent
    }
    
    if (entity.HasComponent<TagComponent>())
    {
      out << YAML::Key << "TagComponent";
      out << YAML::BeginMap; // TagComponent
      
      auto& tag = entity.GetComponent<TagComponent>().tag;
      out << YAML::Key << "Tag" << YAML::Value << tag;
      
      out << YAML::EndMap; // TagComponent
    }
    
    if (entity.HasComponent<RelationshipComponent>())
    {
      auto& relationshipComponent = entity.GetComponent<RelationshipComponent>();
      out << YAML::Key << "Parent" << YAML::Value << relationshipComponent.parentHandle;
      
      out << YAML::Key << "Children";
      out << YAML::Value << YAML::BeginSeq;
      
      for (auto child : relationshipComponent.children)
      {
        out << YAML::BeginMap;
        out << YAML::Key << "Handle" << YAML::Value << child;
        out << YAML::EndMap;
      }
      out << YAML::EndSeq;
    }
    
    if (entity.HasComponent<TransformComponent>())
    {
      out << YAML::Key << "TransformComponent";
      out << YAML::BeginMap; // TransformComponent
      
      auto& transform = entity.GetTransform();
      
      out << YAML::Key << "Position" << YAML::Value << transform.Position();
      out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation();
      out << YAML::Key << "Scale" << YAML::Value << transform.Scale();
      
      out << YAML::EndMap; // TransformComponent
    }
    
    if (entity.HasComponent<CameraComponent>())
    {
      out << YAML::Key << "CameraComponent";
      out << YAML::BeginMap; // CameraComponent
      
      auto& cameraComponent = entity.GetComponent<CameraComponent>();
      auto& camera = cameraComponent.camera;
      out << YAML::Key << "Enable" << YAML::Value << cameraComponent.enable;
      
      out << YAML::Key << "Camera" << YAML::Value;
      out << YAML::BeginMap; // Camera
      out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
      out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetDegPerspectiveVerticalFOV();
      out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
      out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
      out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
      out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
      out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
      
      out << YAML::EndMap; // Camera
      out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
      
      out << YAML::EndMap; // CameraComponent
    }
    
    if (entity.HasComponent<MeshComponent>())
    {
      out << YAML::Key << "MeshComponent";
      out << YAML::BeginMap; // MeshComponent
      
      auto& meshComponent = entity.GetComponent<MeshComponent>();
      
      out << YAML::Key << "Enable" << YAML::Value << meshComponent.enable;
      out << YAML::Key << "MeshHandle" << YAML::Value << meshComponent.mesh;
      out << YAML::Key << "TilingFactor" << YAML::Value << meshComponent.tilingFactor;
      
      auto materialTable = meshComponent.materialTable;
      if (materialTable->GetMaterialCount() > 0)
      {
        out << YAML::Key << "MaterialTable" << YAML::Value << YAML::BeginMap; // MaterialTable
        
        for (uint32_t i = 0; i < materialTable->GetMaterialCount(); i++)
        {
          AssetHandle handle = (materialTable->HasMaterial(i) ? materialTable->GetMaterial(i)->handle : (AssetHandle)0);
          out << YAML::Key << i << YAML::Value << handle;
        }
        out << YAML::EndMap; // MaterialTable
      }
      out << YAML::EndMap; // MeshComponent
    }
    
    out << YAML::EndMap; // Entity
  }
  
  void EntitySerializer::DeserializeEntities(Ref<Scene> scene, YAML::Node& entitiesNode)
  {
    IK_PROFILE();
    for (auto entity : entitiesNode)
    {
      uint64_t uuid = entity["Entity"].as<uint64_t>();
      
      std::string name;
      
      // TagComponent ------------------------------------------------------------------------------------------------
      auto tagComponent = entity["TagComponent"];
      if (tagComponent)
      {
        name = tagComponent["Tag"].as<std::string>();
      }
      
      Entity deserializedEntity = scene->CreateEntityWithID(uuid, name);
      
      // VisibilityComponent ---------------------------------------------------------------------------------------
      auto visibilityComponent = entity["VisibilityComponent"];
      if (visibilityComponent)
      {
        auto& vc = deserializedEntity.GetComponent<VisibilityComponent>();
        vc.isVisible = visibilityComponent["Visibility"].as<bool>();
      }
      
      // RelationshipComponent ---------------------------------------------------------------------------------------
      auto& relationshipComponent = deserializedEntity.GetComponent<RelationshipComponent>();
      uint64_t parentHandle = entity["Parent"] ? entity["Parent"].as<uint64_t>() : 0;
      relationshipComponent.parentHandle = parentHandle;
      
      auto children = entity["Children"];
      if (children)
      {
        for (auto child : children)
        {
          uint64_t childHandle = child["Handle"].as<uint64_t>();
          relationshipComponent.children.push_back(childHandle);
        }
      }
      
      // TransformComponent ------------------------------------------------------------------------------------------
      auto transformComponent = entity["TransformComponent"];
      if (transformComponent)
      {
        // Entities always have transforms
        auto& transform = deserializedEntity.GetTransform();
        transform.UpdatePosition(transformComponent["Position"].as<glm::vec3>());
        const auto& rotationNode = transformComponent["Rotation"];
        // Rotations used to be stored as quaternions
        if (rotationNode.size() == 4)
        {
          glm::quat rotation = transformComponent["Rotation"].as<glm::quat>();
          transform.UpdateRotation(glm::eulerAngles(rotation));
        }
        else
        {
          IK_ASSERT(rotationNode.size() == 3);
          transform.UpdateRotation(transformComponent["Rotation"].as<glm::vec3>());
        }
        transform.UpdateScale(transformComponent["Scale"].as<glm::vec3>());
      }
      
      // CameraComponent --------------------------------------------------------------------------------------------
      auto cameraComponent = entity["CameraComponent"];
      if (cameraComponent)
      {
        auto& component = deserializedEntity.AddComponent<CameraComponent>();
        component.enable = cameraComponent["Enable"].as<bool>();
        const auto& cameraNode = cameraComponent["Camera"];
        
        auto& camera = component.camera;
        
        if (cameraNode.IsMap())
        {
          if (cameraNode["ProjectionType"])
          {
            camera.SetProjectionType((SceneCamera::ProjectionType)cameraNode["ProjectionType"].as<int>());
          }
          if (cameraNode["PerspectiveFOV"])
          {
            camera.SetDegPerspectiveVerticalFOV(cameraNode["PerspectiveFOV"].as<float>());
          }
          if (cameraNode["PerspectiveNear"])
          {
            camera.SetPerspectiveNearClip(cameraNode["PerspectiveNear"].as<float>());
          }
          if (cameraNode["PerspectiveFar"])
          {
            camera.SetPerspectiveFarClip(cameraNode["PerspectiveFar"].as<float>());
          }
          if (cameraNode["OrthographicSize"])
          {
            camera.SetOrthographicSize(cameraNode["OrthographicSize"].as<float>());
          }
          if (cameraNode["OrthographicNear"])
          {
            camera.SetOrthographicNearClip(cameraNode["OrthographicNear"].as<float>());
          }
          if (cameraNode["OrthographicFar"])
          {
            camera.SetOrthographicFarClip(cameraNode["OrthographicFar"].as<float>());
          }
        }
        component.primary = cameraComponent["Primary"].as<bool>();
      }
      
      // MeshComponent ----------------------------------------------------------------------------------------------
      auto meshComponent = entity["MeshComponent"];
      if (meshComponent)
      {
        auto& component = deserializedEntity.AddComponent<MeshComponent>();
        component.enable = meshComponent["Enable"].as<bool>();
        component.mesh = meshComponent["MeshHandle"].as<AssetHandle>();
        component.tilingFactor = meshComponent["TilingFactor"].as<float>();
        
        if (meshComponent["MaterialTable"])
        {
          YAML::Node materialTableNode = meshComponent["MaterialTable"];
          for (auto materialEntry : materialTableNode)
          {
            uint32_t index = materialEntry.first.as<uint32_t>();
            AssetHandle materialAsset = materialEntry.second.as<AssetHandle>();
            if (materialAsset && AssetManager::IsAssetHandleValid(materialAsset))
            {
              component.materialTable->SetMaterial(index, AssetManager::GetAsset<MaterialAsset>(materialAsset));
            }
          }
        }
      }
      
    } // For each entity
  }
} // namespace IKan
