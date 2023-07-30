//
//  Project.cpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#include "Project.hpp"

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
    }
    
    // Set new project and initialise the asset manager
    s_activeProject = project;
    // Initialize the assets
  }
  
  const Project::Config& Project::GetConfig() const
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
  std::filesystem::path Project::GetAssetRegistryPath()
  {
    IK_ASSERT(s_activeProject);
    return std::filesystem::path(s_activeProject->GetConfig().projectDirectory) / s_activeProject->GetConfig().assetRegistryPath;
  }
  
  std::filesystem::path Project::GetAssetPath(const std::string& assetRelativePath)
  {
    return GetAssetDirectory() / assetRelativePath;
  }
  
} // namespace IKan
