//
//  ApplicationSettings.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "ApplicationSettings.hpp"

#include <yaml-cpp/yaml.h>

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
    
    if (!std::filesystem::exists(s_editorSettingsPath))
    {
      std::filesystem::create_directory(s_editorSettingsPath);
    }
    s_editorSettingsPath /= "ApplicationSettings.yaml";
    
    LoadSettings();
  }
  
#define IK_ENTER_GROUP(name) currentGroup = rootNode[name];
#define IK_READ_VALUE(name, type, var, defaultValue) var = currentGroup[name].as<type>(defaultValue)
  
  void ApplicationSettingsSerializer::LoadSettings()
  {
    // Generate default settings file if one doesn't exist
    if (!std::filesystem::exists(s_editorSettingsPath))
    {
      SaveSettings();
      return;
    }
    
    std::ifstream stream(s_editorSettingsPath);
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
      IK_READ_VALUE("ShowAssetTypes", bool, settings.contentBrowserShowAssetTypes, true);
      IK_READ_VALUE("ThumbnailSize", int, settings.contentBrowserThumbnailSize, 128);
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
        IK_SERIALIZE_VALUE("ShowAssetTypes", settings.contentBrowserShowAssetTypes);
        IK_SERIALIZE_VALUE("ThumbnailSize", settings.contentBrowserThumbnailSize);
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
