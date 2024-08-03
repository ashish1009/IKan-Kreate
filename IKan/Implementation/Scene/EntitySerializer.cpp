//
//  EntitySerializer.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "EntitySerializer.hpp"

#include "Scene/Entity.hpp"
#include "Scene/Component.hpp"

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
    } // For each entity
  }
} // namespace IKan
