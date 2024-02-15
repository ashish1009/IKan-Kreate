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
  }
  
  void Project::CloseActive()
  {
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
