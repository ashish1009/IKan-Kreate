//
//  Project.hpp
//  IKan
//
//  Created by Ashish . on 19/07/24.
//

#pragma once

#include "Project/ProjectConfig.hpp"

namespace IKan
{
  static const std::string ProjectExtension {".ikproj"};

  /// This class stores the project created by editor
  class Project
  {
  public:
    /// Default constructor
    Project() = default;
    
    // Setter APIs ---------------------------------------------------------------------------------------------------
    /// This function update the project config
    /// - Parameter config: project enw config
    void SetConfig(const ProjectConfig& config);
    
    // Getter APIs ---------------------------------------------------------------------------------------------------
    /// This function returns the project config
    const ProjectConfig& GetConfig() const;
    
    // Asset Path APIs -----------------------------------------------------------------------------------------------
    
    // Static APIs ---------------------------------------------------------------------------------------------------
    /// This function updates the active project
    /// - Parameter project: new active project
    static void SetActive(Ref<Project> project);

    /// This funciton closes the active project
    static void CloseActive();
    
    /// This function returns the active project
    static Ref<Project> GetActive();
    
    DELETE_COPY_MOVE_CONSTRUCTORS(Project);
    
  private:
    inline static Ref<Project> s_activeProject {nullptr};
    ProjectConfig m_config;
    friend class ProjectSerializer;
  };
} // namespace IKan
