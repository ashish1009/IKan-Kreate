//
//  ApplicationSettings.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#include <yaml-cpp/yaml.h>
#include "ApplicationSettings.hpp"

namespace Kreator
{
  static std::filesystem::path s_editorSettingsPath;
  
  ApplicationSettings& ApplicationSettings::Get()
  {
    static ApplicationSettings s_settings;
    return s_settings;
  }
  
  void ApplicationSettingsSerializer::Initialize()
  {
    IK_PROFILE();
    IK_LOG_TRACE("ApplicationSettings", "Initialising the ApplicationSettingsSerializer");
    
    s_editorSettingsPath = std::filesystem::absolute("Config");
    
    if (!Utils::FileSystem::Exists(s_editorSettingsPath))
    {
      Utils::FileSystem::CreateDirectory(s_editorSettingsPath);
    }
    s_editorSettingsPath /= "ApplicationSettings.yaml";
    
    LoadSettings();
  }
  
#define IK_ENTER_GROUP(name) currentGroup = rootNode[name];
#define IK_READ_VALUE(name, type, var, defaultValue) var = currentGroup[name].as<type>(defaultValue)
  
  void ApplicationSettingsSerializer::LoadSettings()
  {
    // Generate default settings file if one doesn't exist
    if (!Utils::FileSystem::Exists(s_editorSettingsPath))
    {
      SaveSettings();
      return;
    }
    
    std::ifstream stream(s_editorSettingsPath);
    IK_LOG_VERIFY(stream, "Invalid steream");
    std::stringstream ss;
    ss << stream.rdbuf();
    
    YAML::Node data = YAML::Load(ss.str());
    if (!data["ApplicationSettings"])
    {
      return;
    }
    
    YAML::Node rootNode = data["ApplicationSettings"];
    YAML::Node currentGroup = rootNode;
    
    auto& settings = ApplicationSettings::Get();
    
    IK_ENTER_GROUP("ContentBrowser");
    {
      IK_READ_VALUE("ShowAssetTypes", bool, settings.ContentBrowserShowAssetTypes, true);
      IK_READ_VALUE("ThumbnailSize", int, settings.ContentBrowserThumbnailSize, 128);
    }
    
    stream.close();
  }
  
#define IK_BEGIN_GROUP(name)\
out << YAML::Key << name << YAML::Value << YAML::BeginMap;
#define IK_END_GROUP() out << YAML::EndMap;
  
#define IK_SERIALIZE_VALUE(name, value) out << YAML::Key << name << YAML::Value << value;
  
  void ApplicationSettingsSerializer::SaveSettings()
  {
    const auto& settings = ApplicationSettings::Get();
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    IK_BEGIN_GROUP("ApplicationSettings");
    {
      IK_BEGIN_GROUP("ContentBrowser");
      {
        IK_SERIALIZE_VALUE("ShowAssetTypes", settings.ContentBrowserShowAssetTypes);
        IK_SERIALIZE_VALUE("ThumbnailSize", settings.ContentBrowserThumbnailSize);
      }
      IK_END_GROUP();
    }
    IK_END_GROUP();
    out << YAML::EndMap;
    
    std::ofstream fout(s_editorSettingsPath);
    fout << out.c_str();
    fout.close();
  }
} // namespace Kreator
