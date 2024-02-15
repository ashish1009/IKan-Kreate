//
//  Project.cpp
//  IKan
//
//  Created by Ashish . on 15/02/24.
//

#include "Project.hpp"

namespace IKan
{
  Project::Project()
  {
  }

  void Project::SetActive(Ref<Project> project)
  {
    IK_PROFILE();
    IK_ASSERT(project, "Null Project, Handle Later ...");

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
} // namespace IKan
