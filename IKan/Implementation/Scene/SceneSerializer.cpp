//
//  SceneSerializer.cpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#include <yaml-cpp/yaml.h>
#include "SceneSerializer.hpp"
#include "Renderer/Mesh.hpp"
#include "Scene/CoreEntity.hpp"
#include "Asset/AssetManager.hpp"
#include "Utils/YAMLSerializerHelper.h"

namespace IKan {
  
  SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
  : m_scene(scene)
  {
  }
  
  void SceneSerializer::Serialize(const std::string& filepath)
  {
    IK_LOG_TRACE(LogModule::SceneSerializer, "Serializing the Scene {0}", Utils::String::GetFileNameFromPath(filepath));
    
    const std::string filename = Utils::String::GetFileNameFromPath(filepath);
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
    
    // Clear all unsaved asset handles`
    m_scene->ClearUnsavedAssets();
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
    
    if (entity.HasComponent<CameraComponent>())
    {
      out << YAML::Key << "CameraComponent";
      out << YAML::BeginMap; // CameraComponent
      
      auto& cameraComponent = entity.GetComponent<CameraComponent>();
      auto& camera = cameraComponent.camera;
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
    
    if (entity.HasComponent<SpriteRendererComponent>())
    {
      out << YAML::Key << "SpriteRendererComponent";
      out << YAML::BeginMap; // SpriteRendererComponent
      
      auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
      out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.color;
      out << YAML::Key << "TextureHandle" << YAML::Value << spriteRendererComponent.texture;
      out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.tilingFactor;
      
      out << YAML::EndMap; // SpriteRendererComponent
    }
    
    if (entity.HasComponent<QuadComponent>())
    {
      out << YAML::Key << "QuadComponent";
      out << YAML::BeginMap; // QuadComponent
      
      auto& quadComponent = entity.GetComponent<QuadComponent>();
      out << YAML::Key << "Color" << YAML::Value << quadComponent.color;
      out << YAML::Key << "TextureHandle" << YAML::Value << quadComponent.texture;
      out << YAML::Key << "TilingFactor" << YAML::Value << quadComponent.tilingFactor;
      
      out << YAML::EndMap; // QuadComponent
    }
    
    if (entity.HasComponent<CircleComponent>())
    {
      out << YAML::Key << "CircleComponent";
      out << YAML::BeginMap; // CircleComponent
      
      auto& circleComponent = entity.GetComponent<CircleComponent>();
      out << YAML::Key << "Color" << YAML::Value << circleComponent.color;
      out << YAML::Key << "TextureHandle" << YAML::Value << circleComponent.texture;
      out << YAML::Key << "TilingFactor" << YAML::Value << circleComponent.tilingFactor;      
      out << YAML::Key << "Thickness" << YAML::Value << circleComponent.thickness;
      out << YAML::Key << "Fade" << YAML::Value << circleComponent.fade;
      
      out << YAML::EndMap; // CircleComponent
    }
    
    if (entity.HasComponent<TextComponent>())
    {
      out << YAML::Key << "TextComponent";
      out << YAML::BeginMap; // TextComponent
      
      auto& textComponent = entity.GetComponent<TextComponent>();
      out << YAML::Key << "TextString" << YAML::Value << textComponent.textString;
      out << YAML::Key << "FontHandle" << YAML::Value << textComponent.assetHandle;
      out << YAML::Key << "Color" << YAML::Value << textComponent.color;
      
      out << YAML::EndMap; // TextComponent
    }
    
    if (entity.HasComponent<StaticMeshComponent>())
    {
      out << YAML::Key << "StaticMeshComponent";
      out << YAML::BeginMap; // StaticMeshComponent
      
      auto& staticMeshComponent = entity.GetComponent<StaticMeshComponent>();
      if (staticMeshComponent.staticMesh)
      {
        out << YAML::Key << "MeshHandle" << YAML::Value << staticMeshComponent.staticMesh;
        Ref<MeshSource> meshSource = AssetManager::GetAsset<MeshSource>(staticMeshComponent.staticMesh);
        meshSource->SerializeMaterials();
        out << YAML::Key << "MaterialIndex" << YAML::Value << meshSource->GetMaterialIndex();
        
        out << YAML::Key << "MaterialTable";
        out << YAML::Value << YAML::BeginSeq;
        if (meshSource->GetMaterialTable()->GetSize() > 0)
        {
          for (auto materialAssets : meshSource->GetMaterialTable()->GetMaterialAssets())
          {
            out << YAML::BeginMap; // MaterialTable
            out << YAML::Key << "MaterialAsset" << YAML::Value << materialAssets.second->handle;
            out << YAML::EndMap; // MaterialTable
          }
        }
        out << YAML::EndSeq;
      }
      else
      {
        out << YAML::Key << "MeshHandle" << YAML::Value << 0;
      }
      out << YAML::EndMap; // StaticMeshComponent
    }
    
    if (entity.HasComponent<RigidBodyComponent>())
    {
      out << YAML::Key << "RigidBodyComponent";
      out << YAML::BeginMap; // RigidBodyComponent
      
      auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
      out << YAML::Key << "BodyType" << YAML::Value << (uint32_t)rigidBodyComponent.bodyType;
      out << YAML::Key << "LiniarDamping" << YAML::Value << rigidBodyComponent.liniarDamping;
      out << YAML::Key << "AngularDamping" << YAML::Value << rigidBodyComponent.angularDamping;
      out << YAML::Key << "AllowSleep" << YAML::Value << rigidBodyComponent.allowSleep;
      out << YAML::EndMap; // RigidBodyComponent
    }
    
    if (entity.HasComponent<Box3DColliderComponent>())
    {
      out << YAML::Key << "Box3DColliderComponent";
      out << YAML::BeginMap; // Box3DColliderComponent
      
      auto& box3DBodyComponent = entity.GetComponent<Box3DColliderComponent>();
      out << YAML::Key << "Size" << YAML::Value << box3DBodyComponent.size;
      out << YAML::Key << "PositionOffset" << YAML::Value << box3DBodyComponent.positionOffset;
      out << YAML::Key << "QuaternionOffset" << YAML::Value << glm::eulerAngles(box3DBodyComponent.quaternionOffset);
      out << YAML::Key << "FrictionCoefficient" << YAML::Value << box3DBodyComponent.frictionCoefficient;
      out << YAML::Key << "Bounciness" << YAML::Value << box3DBodyComponent.bounciness;
      out << YAML::Key << "MassDensity" << YAML::Value << box3DBodyComponent.massDensity;
      out << YAML::EndMap; // Box3DColliderComponent
    }
    
    if (entity.HasComponent<SphereColliderComponent>())
    {
      out << YAML::Key << "SphereColliderComponent";
      out << YAML::BeginMap; // SphereColliderComponent
      
      auto& sphereColliderComponent = entity.GetComponent<SphereColliderComponent>();
      out << YAML::Key << "Radius" << YAML::Value << sphereColliderComponent.radius;
      out << YAML::Key << "PositionOffset" << YAML::Value << sphereColliderComponent.positionOffset;
      out << YAML::Key << "QuaternionOffset" << YAML::Value << glm::eulerAngles(sphereColliderComponent.quaternionOffset);
      out << YAML::Key << "FrictionCoefficient" << YAML::Value << sphereColliderComponent.frictionCoefficient;
      out << YAML::Key << "Bounciness" << YAML::Value << sphereColliderComponent.bounciness;
      out << YAML::Key << "MassDensity" << YAML::Value << sphereColliderComponent.massDensity;
      out << YAML::EndMap; // SphereColliderComponent
    }
    
    if (entity.HasComponent<CapsuleColliderComponent>())
    {
      out << YAML::Key << "CapsuleColliderComponent";
      out << YAML::BeginMap; // CapsuleColliderComponent
      
      auto& capsuleColliderComponent = entity.GetComponent<CapsuleColliderComponent>();
      out << YAML::Key << "Radius" << YAML::Value << capsuleColliderComponent.radius;
      out << YAML::Key << "Height" << YAML::Value << capsuleColliderComponent.height;
      out << YAML::Key << "PositionOffset" << YAML::Value << capsuleColliderComponent.positionOffset;
      out << YAML::Key << "QuaternionOffset" << YAML::Value << glm::eulerAngles(capsuleColliderComponent.quaternionOffset);
      out << YAML::Key << "FrictionCoefficient" << YAML::Value << capsuleColliderComponent.frictionCoefficient;
      out << YAML::Key << "Bounciness" << YAML::Value << capsuleColliderComponent.bounciness;
      out << YAML::Key << "MassDensity" << YAML::Value << capsuleColliderComponent.massDensity;
      
      out << YAML::EndMap; // CapsuleColliderComponent
    }

    if (entity.HasComponent<JointComponent>())
    {
      out << YAML::Key << "JointComponent";
      out << YAML::BeginMap; // JointComponent
      
      auto& jointComponent = entity.GetComponent<JointComponent>();
      out << YAML::Key << "ConnectedEntity" << YAML::Value << jointComponent.connectedEntity;
      out << YAML::Key << "Type" << YAML::Value << (uint32_t)jointComponent.type;
      out << YAML::Key << "IsWorldSpace" << YAML::Value << jointComponent.isWorldSpace;
      out << YAML::Key << "IsCollisionEnabled" << YAML::Value << jointComponent.isCollisionEnabled;
      out << YAML::Key << "WorldAnchorPoint" << YAML::Value << jointComponent.worldAnchorPoint;
      out << YAML::Key << "LocalAnchorPoint1" << YAML::Value << jointComponent.localAnchorPoint1;
      out << YAML::Key << "LocalAnchorPoint2" << YAML::Value << jointComponent.localAnchorPoint2;

      out << YAML::Key << "ConeLimit" << YAML::Value << jointComponent.ballSocketData.coneLimit;
      out << YAML::Key << "ConeAngle" << YAML::Value << jointComponent.ballSocketData.coneAngle;

      out << YAML::Key << "WorldAxis" << YAML::Value << jointComponent.hingeData.worldAxis;
      out << YAML::Key << "LocalAxis1" << YAML::Value << jointComponent.hingeData.localAxis1;
      out << YAML::Key << "LocalAxis2" << YAML::Value << jointComponent.hingeData.localAxis2;

      out << YAML::Key << "Limit" << YAML::Value << jointComponent.hingeData.limit;
      out << YAML::Key << "MinAngle" << YAML::Value << jointComponent.hingeData.initMinAngleLimit;
      out << YAML::Key << "MaxAngle" << YAML::Value << jointComponent.hingeData.initMaxAngleLimit;

      out << YAML::Key << "Motor" << YAML::Value << jointComponent.hingeData.motor;
      out << YAML::Key << "Speed" << YAML::Value << jointComponent.hingeData.initMotorSpeed;
      out << YAML::Key << "Torque" << YAML::Value << jointComponent.hingeData.initMaxMotorTorque;

      out << YAML::Key << "SliderWorldAxis" << YAML::Value << jointComponent.sliderData.worldAxis;
      out << YAML::Key << "SliderLocalAxis1" << YAML::Value << jointComponent.sliderData.localAxis1;
      
      out << YAML::Key << "SliderLimit" << YAML::Value << jointComponent.sliderData.limit;
      out << YAML::Key << "SliderMinTranse" << YAML::Value << jointComponent.sliderData.initMinTransLimit;
      out << YAML::Key << "SliderMaxTranse" << YAML::Value << jointComponent.sliderData.initMaxTransLimit;
      
      out << YAML::Key << "SliderMotor" << YAML::Value << jointComponent.sliderData.motor;
      out << YAML::Key << "SliderSpeed" << YAML::Value << jointComponent.sliderData.initMotorSpeed;
      out << YAML::Key << "SliderForce" << YAML::Value << jointComponent.sliderData.initMaxMotorForce;

      out << YAML::EndMap; // JointComponent
    }
    
    if (entity.HasComponent<PointLightComponent>())
    {
      out << YAML::Key << "PointLightComponent";
      out << YAML::BeginMap; // PointLightComponent
      
      auto& pointLightComponent = entity.GetComponent<PointLightComponent>();
 
      out << YAML::Key << "Active" << YAML::Value << pointLightComponent.active;
      out << YAML::Key << "Radiance" << YAML::Value << pointLightComponent.radiance;

      out << YAML::EndMap; // PointLightComponent
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
      
      // StaticMeshComponent ----------------------------------------------------------------------------------------------
      auto staticMeshComponent = entity["StaticMeshComponent"];
      if (staticMeshComponent)
      {
        auto& component = deserializedEntity.AddComponent<StaticMeshComponent>();
        component.staticMesh = staticMeshComponent["MeshHandle"].as<AssetHandle>();
        if (component.staticMesh != 0)
        {
          auto meshSource = AssetManager::GetAsset<MeshSource>(component.staticMesh);
          auto materialTable = staticMeshComponent["MaterialTable"];
          if (materialTable)
          {
            for (auto materialAsset : materialTable)
            {
              meshSource->AddNewMaterial(materialAsset["MaterialAsset"].as<AssetHandle>());
            }
            meshSource->SetMaterialIndex(staticMeshComponent["MaterialIndex"].as<int32_t>());
          }
        }
      }
      
      // RigidBodyComponent ----------------------------------------------------------------------------------------------
      auto rigidBodyComponent = entity["RigidBodyComponent"];
      if (rigidBodyComponent)
      {
        auto& component = deserializedEntity.AddComponent<RigidBodyComponent>();
        component.bodyType = static_cast<RigidBodyComponent::BodyType>(rigidBodyComponent["BodyType"].as<uint32_t>());
        component.liniarDamping = rigidBodyComponent["LiniarDamping"].as<float>();
        component.angularDamping = rigidBodyComponent["AngularDamping"].as<float>();
        component.allowSleep = rigidBodyComponent["AllowSleep"].as<bool>();
      }
      
      // Box3DColliderComponent ----------------------------------------------------------------------------------------------
      auto box3DColliderComponent = entity["Box3DColliderComponent"];
      if (box3DColliderComponent)
      {
        auto& component = deserializedEntity.AddComponent<Box3DColliderComponent>();
        component.size = box3DColliderComponent["Size"].as<glm::vec3>();
        component.positionOffset = box3DColliderComponent["PositionOffset"].as<glm::vec3>();
        component.quaternionOffset = glm::quat(box3DColliderComponent["QuaternionOffset"].as<glm::vec3>());
        component.frictionCoefficient = box3DColliderComponent["FrictionCoefficient"].as<float>();
        component.massDensity = box3DColliderComponent["MassDensity"].as<float>();
        component.bounciness = box3DColliderComponent["Bounciness"].as<float>();
      }
      
      // SphereColliderComponent ------------------------------------------------------------------------------------
      auto sphereColliderComponent = entity["SphereColliderComponent"];
      if (sphereColliderComponent)
      {
        auto& component = deserializedEntity.AddComponent<SphereColliderComponent>();
        component.radius = sphereColliderComponent["Radius"].as<float>();
        component.positionOffset = sphereColliderComponent["PositionOffset"].as<glm::vec3>();
        component.quaternionOffset = glm::quat(sphereColliderComponent["QuaternionOffset"].as<glm::vec3>());
        component.frictionCoefficient = sphereColliderComponent["FrictionCoefficient"].as<float>();
        component.massDensity = sphereColliderComponent["MassDensity"].as<float>();
        component.bounciness = sphereColliderComponent["Bounciness"].as<float>();
      }
      
      // MeshColliderComponent --------------------------------------------------------------------------------------
      auto meshColliderComponent = entity["CapsuleColliderComponent"];
      if (meshColliderComponent)
      {
        auto& component = deserializedEntity.AddComponent<CapsuleColliderComponent>();
        component.radius = meshColliderComponent["Radius"].as<float>();
        component.height = meshColliderComponent["Height"].as<float>();
        component.positionOffset = meshColliderComponent["PositionOffset"].as<glm::vec3>();
        component.quaternionOffset = glm::quat(meshColliderComponent["QuaternionOffset"].as<glm::vec3>());
        component.frictionCoefficient = meshColliderComponent["FrictionCoefficient"].as<float>();
        component.massDensity = meshColliderComponent["MassDensity"].as<float>();
        component.bounciness = meshColliderComponent["Bounciness"].as<float>();
      }

      // JointComponent --------------------------------------------------------------------------------------
      auto jointComponent = entity["JointComponent"];
      if (jointComponent)
      {
        auto& component = deserializedEntity.AddComponent<JointComponent>();
        component.connectedEntity = jointComponent["ConnectedEntity"].as<uint64_t>();
        component.type = static_cast<JointComponent::Type>(jointComponent["Type"].as<uint32_t>());
        component.isWorldSpace = jointComponent["IsWorldSpace"].as<bool>();
        component.isCollisionEnabled = jointComponent["IsCollisionEnabled"].as<bool>();
        component.worldAnchorPoint = jointComponent["WorldAnchorPoint"].as<glm::vec3>();
        component.localAnchorPoint1 = jointComponent["LocalAnchorPoint1"].as<glm::vec3>();
        component.localAnchorPoint2 = jointComponent["LocalAnchorPoint2"].as<glm::vec3>();
        
        component.ballSocketData.coneLimit = jointComponent["ConeLimit"].as<bool>();
        component.ballSocketData.coneAngle = jointComponent["ConeAngle"].as<float>();

        component.hingeData.worldAxis = jointComponent["WorldAxis"].as<glm::vec3>();
        component.hingeData.localAxis1 = jointComponent["LocalAxis1"].as<glm::vec3>();
        component.hingeData.localAxis2 = jointComponent["LocalAxis2"].as<glm::vec3>();
        component.hingeData.limit = jointComponent["Limit"].as<bool>();
        component.hingeData.initMinAngleLimit = jointComponent["MinAngle"].as<float>();
        component.hingeData.initMaxAngleLimit = jointComponent["MaxAngle"].as<float>();
        component.hingeData.motor = jointComponent["Motor"].as<bool>();
        component.hingeData.initMotorSpeed = jointComponent["Speed"].as<float>();
        component.hingeData.initMaxMotorTorque = jointComponent["Torque"].as<float>();

        component.sliderData.worldAxis = jointComponent["SliderWorldAxis"].as<glm::vec3>();
        component.sliderData.localAxis1 = jointComponent["SliderLocalAxis1"].as<glm::vec3>();
        component.sliderData.limit = jointComponent["SliderLimit"].as<bool>();
        component.sliderData.initMinTransLimit = jointComponent["SliderMinTranse"].as<float>();
        component.sliderData.initMaxTransLimit = jointComponent["SliderMaxTranse"].as<float>();
        component.sliderData.motor = jointComponent["SliderMotor"].as<bool>();
        component.sliderData.initMotorSpeed = jointComponent["SliderSpeed"].as<float>();
        component.sliderData.initMaxMotorForce = jointComponent["SliderForce"].as<float>();
      }
      
      // PointLightComponent --------------------------------------------------------------------------------------
      auto pointLightComponent = entity["PointLightComponent"];
      if (pointLightComponent)
      {
        auto& component = deserializedEntity.AddComponent<PointLightComponent>();
        component.active = pointLightComponent["Active"].as<bool>();
        component.radiance = pointLightComponent["Radiance"].as<glm::vec3>();
      }
    }
  }
} // namespace IKan
