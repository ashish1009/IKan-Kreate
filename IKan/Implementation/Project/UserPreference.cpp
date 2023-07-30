//
//  UserPreference.cpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#include <yaml-cpp/yaml.h>
#include "UserPreference.hpp"
#include "Utils/SerializeMacro.h"

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
      IK_SERIALIZE_PROPERTY(ShowWelcomeScreen, m_preferences->showWelcomeScreen, out);
      
      if (!m_preferences->startupProject.empty())
      {
        IK_SERIALIZE_PROPERTY(StartupProject, m_preferences->startupProject, out);
      }
      
      {
        out << YAML::Key << "RecentProjects";
        out << YAML::Value << YAML::BeginSeq;
        for (const auto&[lastOpened, projectConfig] : m_preferences->recentProjects)
        {
          out << YAML::BeginMap;
          IK_SERIALIZE_PROPERTY(Name, projectConfig.name, out);
          IK_SERIALIZE_PROPERTY(ProjectPath, projectConfig.filePath, out);
          IK_SERIALIZE_PROPERTY(LastOpened, projectConfig.lastOpened, out);
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
    IK_DESERIALIZE_PROPERTY(ShowWelcomeScreen, m_preferences->showWelcomeScreen, rootNode, true);
    IK_DESERIALIZE_PROPERTY(StartupProject, m_preferences->startupProject, rootNode, std::string(""));
    
    for (auto recentProject : rootNode["RecentProjects"])
    {
      RecentProject entry;
      IK_DESERIALIZE_PROPERTY(Name, entry.name, rootNode, std::string(""));
      IK_DESERIALIZE_PROPERTY(ProjectPath, entry.filePath, rootNode, std::string(""));
      IK_DESERIALIZE_PROPERTY(LastOpened, entry.lastOpened, rootNode, time(NULL));
      m_preferences->recentProjects[entry.lastOpened] = entry;
    }
    
    stream.close();
    
    m_preferences->filePath = filepath.string();
  }
} // namespace IKan
