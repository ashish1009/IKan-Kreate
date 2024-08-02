//
//  SceneSerializer.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include <yaml-cpp/yaml.h>

#include "SceneSerializer.hpp"

namespace IKan
{
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
    
    return true;
  }

} // namespace IKan
