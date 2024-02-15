//
//  Project.hpp
//  IKan
//
//  Created by Ashish . on 15/02/24.
//

#pragma once

namespace IKan
{
  struct ProjectConfig
  {
    // General
    std::string name = "New Project";

    // Asset Data
    std::filesystem::path assetDirectory;
    std::filesystem::path assetRegistryPath;
    std::filesystem::path fontPath;
    std::filesystem::path meshPath;
    std::filesystem::path materialPath;
    std::filesystem::path scenePath;
    std::filesystem::path texturePath;
    
    // Scene Serialize
    std::string startScene;
    bool enableAutoSave = false;
    int32_t autoSaveIntervalSeconds = 300;

    // Not serialized
    std::string projectFileName;
    std::filesystem::path projectDirectory;
  };
  
  class Project
  {
  public:
    /// This constructor creates the project and creates the directories if needed
    Project();

    /// This funciton retusns the current project config
    const ProjectConfig& GetConfig() const;

    /// This function returns the active project
    static Ref<Project> GetActive();
    /// This function updates the active project instance
    /// - Parameter project: Active project
    static void SetActive(Ref<Project> project);
    /// This function closes the active project
    static void CloseActive();
    
  private:
    ProjectConfig m_config;
    inline static Ref<Project> s_activeProject;
  };
} // namespace IKan
