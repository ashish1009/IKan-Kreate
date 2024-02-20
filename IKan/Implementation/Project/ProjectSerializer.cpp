//
//  ProjectSerializer.cpp
//  IKan
//
//  Created by Ashish . on 15/02/24.
//

#include "ProjectSerializer.hpp"

namespace IKan
{
  ProjectSerializer::ProjectSerializer(Ref<Project> project)
  : m_project(project)
  {
    
  }
  
  void ProjectSerializer::Serialize(const std::filesystem::path& filepath)
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Project, "Serialising the project {0}", filepath.filename().string());
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Project" << YAML::Value;
    {
      out << YAML::BeginMap;
      out << YAML::Key << "Name" << YAML::Value << m_project->m_config.name;

      out << YAML::Key << "AssetDirectory" << YAML::Value << m_project->m_config.assetDirectory;
      out << YAML::Key << "AssetRegistry" << YAML::Value << m_project->m_config.assetRegistryPath;
      out << YAML::Key << "MeshDirectory" << YAML::Value << m_project->m_config.meshPath;
      out << YAML::Key << "FontDirectory" << YAML::Value << m_project->m_config.fontPath;
      out << YAML::Key << "SceneDirectory" << YAML::Value << m_project->m_config.scenePath;
      out << YAML::Key << "TextureDirectory" << YAML::Value << m_project->m_config.texturePath;
      out << YAML::Key << "MaterialPath" << YAML::Value << m_project->m_config.materialPath;
      
      out << YAML::Key << "StartScene" << YAML::Value << m_project->m_config.startScene;
      out << YAML::Key << "AutoSave" << YAML::Value << m_project->m_config.enableAutoSave;
      out << YAML::Key << "AutoSaveInterval" << YAML::Value << m_project->m_config.autoSaveIntervalSeconds;
      out << YAML::EndMap;
    }
    out << YAML::EndMap;
    
    std::ofstream fout(filepath);
    fout << out.c_str();
  }
  
  bool ProjectSerializer::Deserialize(const std::filesystem::path& filepath)
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Project, "Deserialising the project {0}", filepath.filename().string());
    
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
    
    config.name = rootNode["Name"].as<std::string>();
    
    config.assetDirectory = rootNode["AssetDirectory"].as<std::string>();
    config.assetRegistryPath = rootNode["AssetRegistry"].as<std::string>();
    config.meshPath = rootNode["MeshDirectory"].as<std::string>();
    config.fontPath = rootNode["FontDirectory"].as<std::string>();
    config.scenePath = rootNode["SceneDirectory"].as<std::string>();
    config.texturePath = rootNode["TextureDirectory"].as<std::string>();
    config.materialPath = rootNode["MaterialPath"].as<std::string>();
    
    config.startScene = rootNode["StartScene"].as<std::string>("");
    config.enableAutoSave = rootNode["AutoSave"].as<bool>(false);
    config.autoSaveIntervalSeconds = rootNode["AutoSaveInterval"].as<int>(300);
    
    // Unserialized data
    config.projectFileName = filepath.filename().string();
    config.projectDirectory = filepath.parent_path().string();
    
    return true;
  }
} // namespace IKan
