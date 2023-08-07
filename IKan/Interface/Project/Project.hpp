//
//  Project.hpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#pragma once

namespace IKan
{
  static const std::string ProjectExtension = ".ikproj";

  class Project
  {
  public:
    /// This structure stores the project confguration
    struct Config
    {
      std::string name = "New_Project";
      std::string assetDirectory;
      std::string assetRegistryPath;
      std::string meshPath;
      std::string meshSourcePath;

      std::string startScene;
      bool enableAutoSave = false;
      int32_t autoSaveIntervalSeconds = 300;
      
      // Not serialized
      std::string projectFileName;
      std::string projectDirectory;
    };
    
    /// This funciton retusns the current project config
    const Config& GetConfig() const;
    /// This funciton retusns the current project config
    Config& GetConfig();

    /// This function returns the active project
    static Ref<Project> GetActive();
    /// This function updates the active project instance
    /// - Parameter project: Active project
    static void SetActive(Ref<Project> project);
    
    /// This function returns the project name
    static const std::string& GetProjectName();
    /// This function returns the project directory path
    static std::filesystem::path GetProjectDirectory();
    /// This function returns the project asset path
    static std::filesystem::path GetAssetDirectory();
    /// This function returns the project Mesh path
    static std::filesystem::path GetMeshDirectory();
    /// This function returns the project Mesh source path
    static std::filesystem::path GetMeshSourceDirectory();
    /// This function returns the project asset
    static std::filesystem::path GetAssetRegistryPath();
    /// This function returns the absolute file path of asset
    /// - Parameter assetRelativePath: asset raltive path
    static std::filesystem::path GetAssetPath(const std::string& assetRelativePath);
    /// This function returns the absolute file path of asset
    /// - Parameter assetRelativePath: asset raltive path
    static std::filesystem::path GetMeshPath(const std::string& meshsourceRelativePath);
    /// This function returns the absolute file path of asset
    /// - Parameter assetRelativePath: asset raltive path
    static std::filesystem::path GetMeshSourcePath(const std::string& meshRelativePath);

  private:
    Config m_config;
    inline static Ref<Project> s_activeProject;
    friend class ProjectSerializer;
  };
} // namespace IKan
