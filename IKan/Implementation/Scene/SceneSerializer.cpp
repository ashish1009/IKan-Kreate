//
//  SceneSerializer.cpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#include <yaml-cpp/yaml.h>
#include "SceneSerializer.hpp"
#include "Scene/Entity.hpp"
#include "Utils/YAMLSerializerHelper.h"

namespace IKan {
  
  SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
  : m_scene(scene)
  {
  }
  
  void SceneSerializer::Serialize(const std::string& filepath)
  {
    IK_LOG_TRACE(LogModule::SceneSerializer, "Serializing the Scene {0}", Utils::String::GetFileNameFromPath(filepath));
    
    m_scene->SetName(Utils::String::GetFileNameFromPath(filepath));
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene";
    out << YAML::Value << m_scene->GetName();

    out << YAML::Key << "Entities";
    out << YAML::Value << YAML::BeginSeq;

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

    return true;
  }  
} // namespace IKan
