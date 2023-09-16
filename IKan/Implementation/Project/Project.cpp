//
//  Project.cpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#include "Project.hpp"
#include "Asset/AssetManager.hpp"

namespace IKan
{
  Ref<Project> Project::GetActive()
  {
    return s_activeProject;
  }
  
  void Project::SetActive(Ref<Project> project)
  {
    IK_ASSERT(project, "Null Project");
    
    // Shutdown if already a project is set
    if (s_activeProject)
    {
      // Shutdown all asset
      AssetManager::Shutdown();
    }
    
    // Set new project and initialise the asset manager
    s_activeProject = project;
    
    // Initialize the assets
    AssetManager::Initialize();
  }
  
  void Project::CloseActive()
  {
    // Shutdown if already a project is set
    if (s_activeProject)
    {
      // Shutdown all asset
      AssetManager::Shutdown();
    }
    
    s_activeProject = nullptr;
  }
  
  const Project::Config& Project::GetConfig() const
  {
    return m_config;
  }
  Project::Config& Project::GetConfig()
  {
    return m_config;
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
  std::filesystem::path Project::GetMeshSourceDirectory()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().meshSourcePath;
  }
  std::filesystem::path Project::GetMaterialDirectory()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().materialPath;
  }
  std::filesystem::path Project::GetAssetRegistryPath()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().assetRegistryPath;
  }
  
  std::filesystem::path Project::GetAssetPath(const std::string& assetRelativePath)
  {
    return GetAssetDirectory() / assetRelativePath;
  }
  std::filesystem::path Project::GetMeshPath(const std::string& meshRelativePath)
  {
    return GetMeshDirectory() / meshRelativePath;
  }
  std::filesystem::path Project::GetFontPath(const std::string& fontRelativePath)
  {
    return GetFontDirectory() / fontRelativePath;
  }
  std::filesystem::path Project::GetScenePath(const std::string& sceneRelativePath)
  {
    return GetSceneDirectory() / sceneRelativePath;
  }
  std::filesystem::path Project::GetTexturePath(const std::string& textureRelativePath)
  {
    return GetTextureDirectory() / textureRelativePath;
  }
  std::filesystem::path Project::GetMeshSourcePath(const std::string& meshsourceRelativePath)
  {
    return GetMeshSourceDirectory() / meshsourceRelativePath;;
  }
  std::filesystem::path Project::GetMaterialPath(const std::string& materialRelativePath)
  {
    return GetMaterialDirectory() / materialRelativePath;;
  }
} // namespace IKan
