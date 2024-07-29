//
//  ProjectSerializer.hpp
//  IKan
//
//  Created by Ashish . on 26/07/24.
//

#pragma once

#include "Project/Project.hpp"

namespace IKan
{
  /// This class serialise and deserialise the project
  class ProjectSerializer
  {
  public:
    /// This Constructor creates the serialiser instnace
    /// - Parameter project: project instance
    ProjectSerializer(Ref<Project> project);
    
    /// This function serialises the project
    /// - Parameter filepath: Project Filepath
    void Serialize(const std::filesystem::path& filepath);
    /// This function deserialises the project
    /// - Parameter filepath: Project Filepath
    bool Deserialize(const std::filesystem::path& filepath);
    
  private:
    Ref<Project> m_project;
  };
} // namespace IKan
