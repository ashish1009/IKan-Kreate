//
//  SceneSerializer.cpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#include <yaml-cpp/yaml.h>
#include "SceneSerializer.hpp"
#include "Scene/EntitySerializer.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components.hpp"
#include "Renderer/Mesh.hpp"
#include "Asset/AssetManager.hpp"
#include "Utils/YAMLSerializerUtils.h"

namespace IKan {
  
  SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
  : m_scene(scene)
  {
  }
  
  void SceneSerializer::Serialize(const std::filesystem::path& filepath)
  {
    IK_PROFILE();
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
      EntitySerializer::SerializeEntity(out, { entity, m_scene.get() });
    }
    
    out << YAML::EndSeq;
    out << YAML::EndMap;
    
    std::ofstream fout(filepath);
    fout << out.c_str();
  }
  
  bool SceneSerializer::Deserialize(const std::filesystem::path& filepath)
  {
    IK_PROFILE();
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
      EntitySerializer::DeserializeEntities(m_scene, entities);
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
  
} // namespace IKan
