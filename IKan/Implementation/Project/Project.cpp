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
} // namespace IKan
