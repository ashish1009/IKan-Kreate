//
//  SceneSerializer.cpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#include <yaml-cpp/yaml.h>
#include "SceneSerializer.hpp"
#include "Scene/Entity.hpp"
#include "Utils/SerializeMacro.h"
#include "Utils/YAMLSerializerHelper.h"

namespace IKan {
  
  SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
  : m_scene(scene)
  {
  }
  
  void SceneSerializer::Serialize(const std::string& filepath)
  {
    IK_LOG_TRACE(LogModule::SceneSerializer, "Serializing the Scene {0}", Utils::String::GetFileNameFromPath(filepath));
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
  
  bool SceneSerializer::Deserialize(const std::string& filepath)
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
      std::filesystem::path path = filepath;
      sceneName = path.stem().string();
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
      IK_SERIALIZE_PROPERTY(Tag, tag, out);
      
      out << YAML::EndMap; // TagComponent
    }
    
    if (entity.HasComponent<RelationshipComponent>())
    {
      auto& relationshipComponent = entity.GetComponent<RelationshipComponent>();
      IK_SERIALIZE_PROPERTY(Parent, relationshipComponent.parentHandle, out);

      out << YAML::Key << "Children";
      out << YAML::Value << YAML::BeginSeq;
      
      for (auto child : relationshipComponent.children)
      {
        out << YAML::BeginMap;
        IK_SERIALIZE_PROPERTY(Handle, child, out);
        out << YAML::EndMap;
      }
      out << YAML::EndSeq;
    }
    
    if (entity.HasComponent<TransformComponent>())
    {
      out << YAML::Key << "TransformComponent";
      out << YAML::BeginMap; // TransformComponent
      
      auto& transform = entity.GetComponent<TransformComponent>();
      IK_SERIALIZE_PROPERTY(Position, transform.Position(), out);
      IK_SERIALIZE_PROPERTY(Rotation, transform.Rotation(), out);
      IK_SERIALIZE_PROPERTY(Scale, transform.Scale(), out);
      
      out << YAML::EndMap; // TransformComponent
    }
    
    if (entity.HasComponent<CameraComponent>())
    {
      out << YAML::Key << "CameraComponent";
      out << YAML::BeginMap; // CameraComponent
      
      auto& cameraComponent = entity.GetComponent<CameraComponent>();
      auto& camera = cameraComponent.camera;
      out << YAML::Key << "Camera" << YAML::Value;
      out << YAML::BeginMap; // Camera

      IK_SERIALIZE_PROPERTY(ProjectionType, (int)camera.GetProjectionType(), out);
      IK_SERIALIZE_PROPERTY(PerspectiveFOV, camera.GetDegPerspectiveVerticalFOV(), out);
      IK_SERIALIZE_PROPERTY(PerspectiveNear, camera.GetPerspectiveNearClip(), out);
      IK_SERIALIZE_PROPERTY(PerspectiveFar, camera.GetPerspectiveFarClip(), out);
      IK_SERIALIZE_PROPERTY(OrthographicSize, camera.GetOrthographicSize(), out);
      IK_SERIALIZE_PROPERTY(OrthographicNear, camera.GetOrthographicNearClip(), out);
      IK_SERIALIZE_PROPERTY(OrthographicFar, camera.GetOrthographicFarClip(), out);

      out << YAML::EndMap; // Camera
      out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
      
      out << YAML::EndMap; // CameraComponent
    }
    
    if (entity.HasComponent<SpriteRendererComponent>())
    {
      out << YAML::Key << "SpriteRendererComponent";
      out << YAML::BeginMap; // SpriteRendererComponent
      
      auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
      IK_SERIALIZE_PROPERTY(Color, spriteRendererComponent.color, out);
      
      if (spriteRendererComponent.texture)
      {
        IK_SERIALIZE_PROPERTY(TextureHandle, spriteRendererComponent.texture, out);
      }
      else
      {
        IK_SERIALIZE_PROPERTY(TextureHandle, 0, out);
      }

      IK_SERIALIZE_PROPERTY(TilingFactor, spriteRendererComponent.tilingFactor, out);
      
      out << YAML::EndMap; // SpriteRendererComponent
    }
    
    if (entity.HasComponent<QuadComponent>())
    {
      out << YAML::Key << "QuadComponent";
      out << YAML::BeginMap; // QuadComponent
      
      auto& quadComponent = entity.GetComponent<QuadComponent>();
      IK_SERIALIZE_PROPERTY(Color, quadComponent.color, out);
      
      if (quadComponent.texture)
      {
        IK_SERIALIZE_PROPERTY(TextureHandle, quadComponent.texture, out);
      }
      else
      {
        IK_SERIALIZE_PROPERTY(TextureHandle, 0, out);
      }
      
      IK_SERIALIZE_PROPERTY(TilingFactor, quadComponent.tilingFactor, out);
      
      out << YAML::EndMap; // QuadComponent
    }
    
    if (entity.HasComponent<CircleComponent>())
    {
      out << YAML::Key << "CircleComponent";
      out << YAML::BeginMap; // CircleComponent
      
      auto& circleComponent = entity.GetComponent<CircleComponent>();
      IK_SERIALIZE_PROPERTY(Color, circleComponent.color, out);
      
      if (circleComponent.texture)
      {
        IK_SERIALIZE_PROPERTY(TextureHandle, circleComponent.texture, out);
      }
      else
      {
        IK_SERIALIZE_PROPERTY(TextureHandle, 0, out);
      }
      
      IK_SERIALIZE_PROPERTY(TilingFactor, circleComponent.tilingFactor, out);
      
      IK_SERIALIZE_PROPERTY(Thickness, circleComponent.thickness, out);
      IK_SERIALIZE_PROPERTY(Fade, circleComponent.fade, out);

      out << YAML::EndMap; // CircleComponent
    }
    
    if (entity.HasComponent<TextComponent>())
    {
      out << YAML::Key << "TextComponent";
      out << YAML::BeginMap; // TextComponent
      
      auto& textComponent = entity.GetComponent<TextComponent>();
      IK_SERIALIZE_PROPERTY(TextString, textComponent.textString, out);

      if (textComponent.assetHandle)
      {
        IK_SERIALIZE_PROPERTY(FontHandle, textComponent.assetHandle, out);
      }
      else
      {
        IK_SERIALIZE_PROPERTY(FontHandle, 0, out);
      }
      
      IK_SERIALIZE_PROPERTY(Color, textComponent.color, out);
      
      out << YAML::EndMap; // TextComponent
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
      
      // CameraComponent --------------------------------------------------------------------------------------------
      auto cameraComponent = entity["CameraComponent"];
      if (cameraComponent)
      {
        auto& component = deserializedEntity.AddComponent<CameraComponent>();
        const auto& cameraNode = cameraComponent["Camera"];
        
        component.camera = SceneCamera();
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
      
      // SpriteRendererComponent ------------------------------------------------------------------------------------
      auto spriteRendererComponent = entity["SpriteRendererComponent"];
      if (spriteRendererComponent)
      {
        auto& component = deserializedEntity.AddComponent<SpriteRendererComponent>();
        component.color = spriteRendererComponent["Color"].as<glm::vec4>();
        component.tilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
        component.texture = spriteRendererComponent["TextureHandle"].as<AssetHandle>();
      }
      
      // QuadComponent ----------------------------------------------------------------------------------------------
      auto quadComponent = entity["QuadComponent"];
      if (quadComponent)
      {
        auto& component = deserializedEntity.AddComponent<QuadComponent>();
        component.color = quadComponent["Color"].as<glm::vec4>();
        component.tilingFactor = quadComponent["TilingFactor"].as<float>();
        component.texture = quadComponent["TextureHandle"].as<AssetHandle>();
      }
      
      // CircleComponent --------------------------------------------------------------------------------------------
      auto circleComponent = entity["CircleComponent"];
      if (circleComponent)
      {
        auto& component = deserializedEntity.AddComponent<CircleComponent>();
        component.color = circleComponent["Color"].as<glm::vec4>();
        component.tilingFactor = circleComponent["TilingFactor"].as<float>();
        component.texture = circleComponent["TextureHandle"].as<AssetHandle>();
        component.thickness = circleComponent["Thickness"].as<float>();
        component.fade = circleComponent["Fade"].as<float>();
      }
      
      // TextComponent --------------------------------------------------------------------------------------------
      auto textComponent = entity["TextComponent"];
      if (textComponent)
      {
        auto& component = deserializedEntity.AddComponent<TextComponent>();
        component.textString = textComponent["TextString"].as<std::string>();
        component.assetHandle  = textComponent["FontHandle"].as<AssetHandle>();
        component.color = textComponent["Color"].as<glm::vec4>();
      }
    }
  }
  
} // namespace IKan
