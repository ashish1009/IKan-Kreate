//
//  UserPreference.cpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#include <yaml-cpp/yaml.h>
#include "UserPreference.hpp"

namespace IKan
{
  UserPreferencesSerializer::UserPreferencesSerializer(const Ref<UserPreferences>& preferences)
  : m_preferences(preferences)
  {
    
  }
  
  void UserPreferencesSerializer::Serialize(const std::filesystem::path& filepath)
  {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "UserPrefs" << YAML::Value;
    {
      out << YAML::BeginMap;
      out << YAML::Key << "ShowWelcomeScreen" << YAML::Value << m_preferences->showWelcomeScreen;
      
      if (!m_preferences->startupProject.empty())
      {
        out << YAML::Key << "StartupProject" << YAML::Value << m_preferences->startupProject;
      }
      
      {
        out << YAML::Key << "RecentProjects";
        out << YAML::Value << YAML::BeginSeq;
        for (const auto&[lastOpened, projectConfig] : m_preferences->recentProjects)
        {
          out << YAML::BeginMap;
          out << YAML::Key << "Name" << YAML::Value << projectConfig.name;
          out << YAML::Key << "ProjectPath" << YAML::Value << projectConfig.filePath;
          out << YAML::Key << "LastOpened" << YAML::Value << projectConfig.lastOpened;
          out << YAML::EndMap;
        }
        out << YAML::EndSeq;
      }
      
      out << YAML::EndMap;
    }
    out << YAML::EndMap;
    
    std::ofstream fout(filepath);
    fout << out.c_str();
    
    m_preferences->filePath = filepath.string();
    
    IK_LOG_TRACE(LogModule::UserPreference, "Saving the Default User preference in file {0}", Utils::FileSystem::IKanAbsolute(m_preferences->filePath));
  }
  
  void UserPreferencesSerializer::Deserialize(const std::filesystem::path& filepath)
  {
    std::ifstream stream(filepath);
    IK_ASSERT(stream);
    std::stringstream strStream;
    strStream << stream.rdbuf();
    
    YAML::Node data = YAML::Load(strStream.str());
    if (!data["UserPrefs"])
    {
      return;
    }
    
    YAML::Node rootNode = data["UserPrefs"];
    m_preferences->showWelcomeScreen = rootNode["ShowWelcomeScreen"].as<bool>();
    m_preferences->startupProject = rootNode["StartupProject"] ? rootNode["StartupProject"].as<std::string>() : "";
    
    for (auto recentProject : rootNode["RecentProjects"])
    {
      RecentProject entry;
      entry.name = recentProject["Name"].as<std::string>();
      entry.filePath = recentProject["ProjectPath"].as<std::string>();
      entry.lastOpened = recentProject["LastOpened"] ? recentProject["LastOpened"].as<time_t>() : time(NULL);
      m_preferences->recentProjects[entry.lastOpened] = entry;
    }
    
    stream.close();
    
    m_preferences->filePath = filepath.string();
    
    IK_LOG_TRACE(LogModule::UserPreference, "Using the User preference Settings from {0}", Utils::FileSystem::IKanAbsolute(m_preferences->filePath));
  }
} // namespace IKan
