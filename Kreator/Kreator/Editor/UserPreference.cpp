//
//  UserPreference.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "UserPreference.hpp"

namespace Kreator
{
  UserPreferencesSerializer::UserPreferencesSerializer(const Ref<UserPreferences>& preferences)
  : m_preferences(preferences)
  {
    
  }
  
  void UserPreferencesSerializer::Serialize(const std::filesystem::path& filepath)
  {
    IK_PROFILE();
    IK_LOG_INFO("UserPreferences", "Serializing preferences   : {0}", IKan::Utils::FileSystem::IKanAbsolute(filepath).string());
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "UserPrefs" << YAML::Value;
    {
      out << YAML::BeginMap;
      out << YAML::Key << "ShowWelcomeScreen" << YAML::Value << m_preferences->showWelcomeScreen;
      out << YAML::Key << "Theme" << YAML::Value << (uint32_t)m_preferences->theme;
      
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
  }
  
  void UserPreferencesSerializer::Deserialize(const std::filesystem::path& filepath)
  {
    IK_PROFILE();
    IK_LOG_INFO("UserPreferences", "Deserializing preferences : {0}", IKan::Utils::FileSystem::IKanAbsolute(filepath).string());
    
    std::ifstream stream(filepath);
    IK_ASSERT(stream);
    
    std::stringstream strStream {};
    strStream << stream.rdbuf();
    
    YAML::Node data = YAML::Load(strStream.str());
    if (!data["UserPrefs"])
    {
      return;
    }
    
    YAML::Node rootNode = data["UserPrefs"];
    m_preferences->showWelcomeScreen = rootNode["ShowWelcomeScreen"].as<bool>();
    m_preferences->theme = static_cast<UserPreferences::Theme>(rootNode["Theme"].as<uint32_t>());
    m_preferences->startupProject = rootNode["StartupProject"] ? rootNode["StartupProject"].as<std::string>() : "";
    
    for (const auto& recentProject : rootNode["RecentProjects"])
    {
      RecentProject entry;
      entry.name = recentProject["Name"].as<std::string>();
      entry.filePath = recentProject["ProjectPath"].as<std::string>();
      entry.lastOpened = recentProject["LastOpened"] ? recentProject["LastOpened"].as<time_t>() : time(NULL);
      m_preferences->recentProjects[entry.lastOpened] = entry;
    }
    stream.close();
    
    m_preferences->filePath = filepath.string();
  }
} // namespace Kreator
