//
//  ProjectSerializer.cpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#include <yaml-cpp/yaml.h>
#include "ProjectSerializer.hpp"
#include "Utils/SerializeMacro.h"

namespace IKan
{
  ProjectSerializer::ProjectSerializer(Ref<Project> project)
  : m_project(project)
  {
    
  }
  
  void ProjectSerializer::Serialize(const std::string& filepath)
  {
    IK_LOG_TRACE(LogModule::Project, "Serialising the project {0}", Utils::String::GetFileNameFromPath(filepath));
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Project" << YAML::Value;
    {
      out << YAML::BeginMap;
      IK_SERIALIZE_PROPERTY(Name, m_project->m_config.name, out);
      IK_SERIALIZE_PROPERTY(AssetDirectory, m_project->m_config.assetDirectory, out);
      IK_SERIALIZE_PROPERTY(AssetRegistry, m_project->m_config.assetRegistryPath, out);
      
      out << YAML::EndMap;
    }
    out << YAML::EndMap;
    
    std::ofstream fout(filepath);
    fout << out.c_str();
  }
  
  bool ProjectSerializer::Deserialize(const std::string& filepath)
  {
    std::ifstream stream(filepath);
    IK_ASSERT(stream);
    std::stringstream strStream;
    strStream << stream.rdbuf();
    
    YAML::Node data = YAML::Load(strStream.str());
    if (!data["Project"])
    {
      return false;
    }
    
    YAML::Node rootNode = data["Project"];
    if (!rootNode["Name"])
    {
      return false;
    }
    
    auto& config = m_project->m_config;
    
    IK_DESERIALIZE_PROPERTY(Name, config.name, rootNode, std::string(""));
    IK_DESERIALIZE_PROPERTY(AssetDirectory, config.assetDirectory, rootNode, std::string(""));
    IK_DESERIALIZE_PROPERTY(AssetRegistry, config.assetRegistryPath, rootNode, std::string(""));

    // Update the Project Direcotry and Name which was not serialised
    std::filesystem::path projectPath = filepath;
    config.projectFileName = projectPath.filename().string();
    config.projectDirectory = projectPath.parent_path().string();
    return true;
  }
} // namespace IKan
