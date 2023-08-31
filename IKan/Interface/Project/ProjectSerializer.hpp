//
//  ProjectSerializer.hpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#pragma once

#include "Project/Project.hpp"

namespace IKan
{
  /// This class serialise and deserialise the project
  class ProjectSerializer
  {
  public:
    /// This Constructor creates theserialiser instnace
    /// - Parameter project: project instance
    ProjectSerializer(Ref<Project> project);
    
    /// This function serialise the project
    /// - Parameter filepath: Project Filepath
    void Serialize(const std::string& filepath);
    /// This function deserialise the project
    /// - Parameter filepath: Project Filepath
    bool Deserialize(const std::string& filepath);
    
  private:
    Ref<Project> m_project;
  };
} // namespace IKan
