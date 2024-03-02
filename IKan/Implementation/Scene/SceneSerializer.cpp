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
    
    out << YAML::Key << "Environment";
    out << YAML::Value << YAML::BeginMap;
    
    out << YAML::Key << "EnableDirectionLight" << YAML::Value << m_scene->GetDirectionLight().enable;
    out << YAML::Key << "LightDirection" << YAML::Value << m_scene->GetDirectionLight().direction;
    out << YAML::Key << "LightColor" << YAML::Value << m_scene->GetDirectionLight().color;
    out << YAML::Key << "LightIntensity" << YAML::Value << m_scene->GetDirectionLight().intensity;

    out << YAML::Key << "UseIBL" << YAML::Value << m_scene->UseIBL();
    out << YAML::Key << "UseIrradiance" << YAML::Value << m_scene->UseIrradiance();
    out << YAML::Key << "Skybox" << YAML::Value << m_scene->GetSkyboxAsset();

    out << YAML::EndMap;

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

    auto environment = data["Environment"];
    if (environment)
    {
      auto& directionLight = m_scene->GetDirectionLight();
      directionLight.enable = environment["EnableDirectionLight"].as<float>();
      directionLight.color = environment["LightColor"].as<glm::vec3>();
      directionLight.direction = environment["LightDirection"].as<glm::vec3>();
      directionLight.intensity = environment["LightIntensity"].as<float>();

      m_scene->SetIBLFlag(environment["UseIBL"].as<bool>());
      m_scene->SetIrradianceFlag(environment["UseIrradiance"].as<bool>());
      m_scene->SetSkyboxAsset(environment["Skybox"].as<AssetHandle>());

    }
    
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
      
      auto& transform = entity.GetTransform();
      
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
    
    if (entity.HasComponent<RigidBodyComponent>())
    {
      out << YAML::Key << "RigidBodyComponent";
      out << YAML::BeginMap; // RigidBodyComponent
      
      auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
      out << YAML::Key << "Enable" << YAML::Value << rigidBodyComponent.enable;
      out << YAML::Key << "BodyType" << YAML::Value << (uint32_t)rigidBodyComponent.bodyType;
      out << YAML::Key << "LiniarDamping" << YAML::Value << rigidBodyComponent.linearDamping;
      out << YAML::Key << "AngularDamping" << YAML::Value << rigidBodyComponent.angularDamping;
      out << YAML::Key << "AllowSleep" << YAML::Value << rigidBodyComponent.allowSleep;
      out << YAML::Key << "EnableGravity" << YAML::Value << rigidBodyComponent.enableGravity;
      out << YAML::Key << "AngularAxisMove" << YAML::Value << rigidBodyComponent.angularAxisMove;
      out << YAML::EndMap; // RigidBodyComponent
    }
    
    if (entity.HasComponent<Box3DColliderComponent>())
    {
      out << YAML::Key << "Box3DColliderComponent";
      out << YAML::BeginMap; // Box3DColliderComponent
      
      auto& box3DBodyComponent = entity.GetComponent<Box3DColliderComponent>();
      out << YAML::Key << "Enable" << YAML::Value << box3DBodyComponent.enable;
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
      out << YAML::Key << "Enable" << YAML::Value << sphereColliderComponent.enable;
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
      out << YAML::Key << "Enable" << YAML::Value << capsuleColliderComponent.enable;
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
      out << YAML::Key << "Enable" << YAML::Value << jointComponent.enable;
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
      
      // RigidBodyComponent -----------------------------------------------------------------------------------------
      auto rigidBodyComponent = entity["RigidBodyComponent"];
      if (rigidBodyComponent)
      {
        auto& component = deserializedEntity.AddComponent<RigidBodyComponent>();
        component.enable = rigidBodyComponent["Enable"].as<bool>();
        component.bodyType = static_cast<RigidBodyComponent::BodyType>(rigidBodyComponent["BodyType"].as<uint32_t>());
        component.linearDamping = rigidBodyComponent["LiniarDamping"].as<float>();
        component.angularDamping = rigidBodyComponent["AngularDamping"].as<float>();
        component.allowSleep = rigidBodyComponent["AllowSleep"].as<bool>();
        component.enableGravity = rigidBodyComponent["EnableGravity"].as<bool>();
        component.angularAxisMove = rigidBodyComponent["AngularAxisMove"].as<glm::vec3>();
      }
      
      // Box3DColliderComponent -------------------------------------------------------------------------------------
      auto box3DColliderComponent = entity["Box3DColliderComponent"];
      if (box3DColliderComponent)
      {
        auto& component = deserializedEntity.AddComponent<Box3DColliderComponent>();
        component.enable = box3DColliderComponent["Enable"].as<bool>();
        
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
        component.enable = sphereColliderComponent["Enable"].as<bool>();
        
        component.radius = sphereColliderComponent["Radius"].as<float>();
        component.positionOffset = sphereColliderComponent["PositionOffset"].as<glm::vec3>();
        component.quaternionOffset = glm::quat(sphereColliderComponent["QuaternionOffset"].as<glm::vec3>());
        component.frictionCoefficient = sphereColliderComponent["FrictionCoefficient"].as<float>();
        component.massDensity = sphereColliderComponent["MassDensity"].as<float>();
        component.bounciness = sphereColliderComponent["Bounciness"].as<float>();
      }
      
      // CapsuleColliderComponent -----------------------------------------------------------------------------------
      auto capsuleColliderComponent = entity["CapsuleColliderComponent"];
      if (capsuleColliderComponent)
      {
        auto& component = deserializedEntity.AddComponent<CapsuleColliderComponent>();
        component.enable = capsuleColliderComponent["Enable"].as<bool>();
        
        component.radius = capsuleColliderComponent["Radius"].as<float>();
        component.height = capsuleColliderComponent["Height"].as<float>();
        component.positionOffset = capsuleColliderComponent["PositionOffset"].as<glm::vec3>();
        component.quaternionOffset = glm::quat(capsuleColliderComponent["QuaternionOffset"].as<glm::vec3>());
        component.frictionCoefficient = capsuleColliderComponent["FrictionCoefficient"].as<float>();
        component.massDensity = capsuleColliderComponent["MassDensity"].as<float>();
        component.bounciness = capsuleColliderComponent["Bounciness"].as<float>();
      }
      
      // JointComponent ---------------------------------------------------------------------------------------------
      auto jointComponent = entity["JointComponent"];
      if (jointComponent)
      {
        auto& component = deserializedEntity.AddComponent<JointComponent>();
        component.enable = jointComponent["Enable"].as<bool>();
        
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
    } // For each entity
  }
} // namespace IKan
