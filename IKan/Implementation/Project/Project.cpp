//
//  Project.cpp
//  IKan
//
//  Created by Ashish . on 19/07/24.
//

#include "Project.hpp"
#include "Assets/AssetManager.hpp"

namespace IKan
{
  // Project ---------------------------------------------------------------------------------------------------------
  const ProjectConfig& Project::GetConfig() const
  {
    return m_config;
  }
  
  void Project::SetConfig(const ProjectConfig &config)
  {
    m_config = config;
  }

  void Project::UpdateAutoSaveScene(bool flag)
  {
    m_config.enableAutoSave = flag;
  }
  void Project::UpdateAutoSaveSceneTimeInterval(int32_t time)
  {
    m_config.autoSaveIntervalSeconds = time;
  }
  void Project::UpdateStartupScene(const std::filesystem::path& path)
  {
    m_config.startScene = path;
  }
  void Project::UpdateSceneType(SceneType sceneType)
  {
    m_config.sceneType = sceneType;
  }

  void Project::SetActive(Ref<Project> project)
  {
    IK_PROFILE();
    IK_ASSERT(project, "Null Project !!!");

    // Shutdown if already a project is set
    if (s_activeProject)
    {
      // Shutdown all asset
      AssetManager::Shutdown();
    }

    IK_LOG_INFO(LogModule::Project, "Setting the project '{0}' as active", project->GetConfig().name);
    s_activeProject = project;

    AssetManager::Initialize();
  }
  
  void Project::CloseActive()
  {
    IK_PROFILE();
    
    // Shutdown if already a project is set
    if (s_activeProject)
    {
      // Shutdown all asset
      AssetManager::Shutdown();
    }

    s_activeProject = nullptr;
  }

  Ref<Project> Project::GetActive()
  {
    return s_activeProject;
  }
  
  const std::string& Project::GetProjectName()
  {
    IK_ASSERT(s_activeProject);
    return s_activeProject->GetConfig().name;
  }
  
  std::filesystem::path Project::GetProjectDirectory()
  {
    IK_ASSERT(s_activeProject);
    return s_activeProject->GetConfig().projectDirectory;
  }
  std::filesystem::path Project::GetAssetRegistryPath()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().assetRegistryPath;
  }
  
  std::filesystem::path Project::GetAssetDirectory()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().assetDirectory;
  }
  std::filesystem::path Project::GetMeshDirectory()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().meshPath;
  }
  std::filesystem::path Project::GetFontDirectory()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().fontPath;
  }
  std::filesystem::path Project::GetSceneDirectory()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().scenePath;
  }
  std::filesystem::path Project::GetTextureDirectory()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().texturePath;
  }
  std::filesystem::path Project::GetMaterialDirectory()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().materialPath;
  }
  std::filesystem::path Project::GetPrefabDirectory()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().prefabPath;
  }
  std::filesystem::path Project::GetPhysicsDirectory()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().physicsPath;
  }

  std::filesystem::path Project::GetAssetPath(const std::filesystem::path& assetRelativePath)
  {
    return GetAssetDirectory() / assetRelativePath;
  }
  std::filesystem::path Project::GetMeshPath(const std::filesystem::path& meshRelativePath)
  {
    return GetMeshDirectory() / meshRelativePath;
  }
  std::filesystem::path Project::GetFontPath(const std::filesystem::path& fontRelativePath)
  {
    return GetFontDirectory() / fontRelativePath;
  }
  std::filesystem::path Project::GetScenePath(const std::filesystem::path& sceneRelativePath)
  {
    return GetSceneDirectory() / sceneRelativePath;
  }
  std::filesystem::path Project::GetTexturePath(const std::filesystem::path& textureRelativePath)
  {
    return GetTextureDirectory() / textureRelativePath;
  }
  std::filesystem::path Project::GetMaterialPath(const std::filesystem::path& materialRelativePath)
  {
    return GetMaterialDirectory() / materialRelativePath;;
  }
  std::filesystem::path Project::GetPrefabPath(const std::filesystem::path &prefabRelativePath)
  {
    return GetPrefabDirectory() / prefabRelativePath;;
  }
  std::filesystem::path Project::GetPhysicsPath(const std::filesystem::path &physicsRelativePath)
  {
    return GetPhysicsDirectory() / physicsRelativePath;;
  }
} // namespace IKan
