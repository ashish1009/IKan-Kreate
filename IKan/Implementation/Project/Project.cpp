//
//  Project.cpp
//  IKan
//
//  Created by Ashish . on 15/02/24.
//

#include "Project.hpp"

namespace IKan
{
  void Project::SetActive(Ref<Project> project)
  {
    IK_PROFILE();
    IK_ASSERT(project, "Null Project, TODO: Handle Later ...");

    // Set new project and initialise the asset manager
    s_activeProject = project;
  }
  
  void Project::CloseActive()
  {
    IK_PROFILE();
  }
  
  const ProjectConfig& Project::GetConfig() const
  {
    return m_config;
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

} // namespace IKan
