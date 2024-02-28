//
//  SceneSerializer.cpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#include <yaml-cpp/yaml.h>
#include "SceneSerializer.hpp"
#include "Scene/Entity.hpp"
#include "Renderer/Mesh.hpp"
#include "Scene/Components.hpp"
#include "Asset/AssetManager.hpp"
#include "Utils/YAMLSerializerUtils.h"

namespace IKan {
  
  SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
  : m_scene(scene)
  {
  }
  
  void SceneSerializer::Serialize(const std::filesystem::path& filepath)
  {
    IK_LOG_TRACE(LogModule::SceneSerializer, "Serializing the Scene {0}", filepath.filename().string());
    
    const std::string filename = filepath.filename();
    if (filename != m_scene->GetName())
    {
      m_scene->SetName(filename);
    }
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene";
    out << YAML::Value << m_scene->GetName();
    
    out << YAML::Key << "Entities";
    out << YAML::Value << YAML::BeginSeq;
    
    // Sort entities by UUID (for better serializing)
    std::map<UUID, entt::entity> sortedEntityMap;
    auto idComponentView = m_scene->m_registry.view<IDComponent>();
    
    for (auto entity : idComponentView)
    {
      sortedEntityMap[idComponentView.get<IDComponent>(entity).ID] = entity;
    }
    
    // Serialize sorted entities
    for (auto [id, entity] : sortedEntityMap)
    {
      SerializeEntity(out, { entity, m_scene.get() });
    }
    
    out << YAML::EndSeq;
    out << YAML::EndMap;
    
    std::ofstream fout(filepath);
    fout << out.c_str();
  }
  
  bool SceneSerializer::Deserialize(const std::filesystem::path& filepath)
  {
    std::ifstream stream(filepath);
    IK_ASSERT(stream);
    
    std::stringstream strStream;
    strStream << stream.rdbuf();
    
    YAML::Node data = YAML::Load(strStream.str());
    if (!data["Scene"])
      return false;
    
    std::string sceneName = data["Scene"].as<std::string>();
    IK_LOG_TRACE(LogModule::SceneSerializer, "Deserializing scene '{0}'", sceneName);
    
    if (sceneName == "UntitledScene")
    {
      sceneName = filepath.stem().string();
    }
    
    m_scene->SetName(sceneName);
    
    auto entities = data["Entities"];
    if (entities)
    {
      DeserializeEntities(entities, m_scene);
    }
    
    // Sort IdComponent by by entity handle (which is essentially the order in which they were created)
    // This ensures a consistent ordering when iterating IdComponent (for example: when rendering scene hierarchy panel)
    m_scene->m_registry.sort<IDComponent>([this](const auto lhs, const auto rhs)
                                          {
      auto lhsEntity = m_scene->m_entityIDMap.find(lhs.ID);
      auto rhsEntity = m_scene->m_entityIDMap.find(rhs.ID);
      return static_cast<uint32_t>(lhsEntity->second) < static_cast<uint32_t>(rhsEntity->second);
    });
    
    return true;
  }
  
  void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity)
  {
    UUID uuid = entity.GetComponent<IDComponent>().ID;
    out << YAML::BeginMap; // Entity
    out << YAML::Key << "Entity";
    out << YAML::Value << uuid;
    
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
      
      auto& transform = entity.GetComponent<TransformComponent>();
      
      out << YAML::Key << "Position" << YAML::Value << transform.Position();
      out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation();
      out << YAML::Key << "Scale" << YAML::Value << transform.Scale();
      
      out << YAML::EndMap; // TransformComponent
    }
    
    if (entity.HasComponent<MeshComponent>())
    {
      out << YAML::Key << "MeshComponent";
      out << YAML::BeginMap; // MeshComponent
      
      auto& meshComponent = entity.GetComponent<MeshComponent>();
      
      out << YAML::Key << "Enable" << YAML::Value << meshComponent.enable;
      out << YAML::Key << "MeshHandle" << YAML::Value << meshComponent.mesh;
      
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
  
  void SceneSerializer::DeserializeEntities(YAML::Node& entitiesNode, Ref<Scene> scene)
  {
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
        auto& transform = deserializedEntity.GetComponent<TransformComponent>();
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
      
      // MeshComponent ----------------------------------------------------------------------------------------------
      auto meshComponent = entity["MeshComponent"];
      if (meshComponent)
      {
        auto& component = deserializedEntity.AddComponent<MeshComponent>();
        component.enable = meshComponent["Enable"].as<bool>();
        component.mesh = meshComponent["MeshHandle"].as<AssetHandle>();
        
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
