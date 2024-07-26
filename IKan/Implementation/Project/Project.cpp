//
//  Project.cpp
//  IKan
//
//  Created by Ashish . on 19/07/24.
//

#include "Project.hpp"

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

    s_activeProject = project;
    IK_LOG_INFO(LogModule::Project, "Setting the project '{0}' as active", s_activeProject->GetConfig().name);
  }
  
  void Project::CloseActive()
  {
    IK_PROFILE();
    s_activeProject = nullptr;
  }

  Ref<Project> Project::GetActive()
  {
    return s_activeProject;
  }
} // namespace IKan
