//
//  Project.hpp
//  IKan
//
//  Created by Ashish . on 15/02/24.
//

#pragma once

namespace IKan
{
  static const std::string ProjectExtension {".ikproj"};

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
    std::filesystem::path startScene{};
    bool enableAutoSave {false};
    int32_t autoSaveIntervalSeconds {300};

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
    
    /// This function returns the project name
    static const std::string& GetProjectName();
    
    /// This function returns the project directory path
    static std::filesystem::path GetProjectDirectory();
    /// This function returns the project asset
    static std::filesystem::path GetAssetRegistryPath();

    /// This function returns the project asset path
    static std::filesystem::path GetAssetDirectory();
    /// This function returns the project Mesh path
    static std::filesystem::path GetMeshDirectory();
    /// This function returns the project Font path
    static std::filesystem::path GetFontDirectory();
    /// This function returns the project Scene path
    static std::filesystem::path GetSceneDirectory();
    /// This function returns the project Texture path
    static std::filesystem::path GetTextureDirectory();
    /// This function returns the project Mesh path
    static std::filesystem::path GetMaterialDirectory();
    
    /// This function returns the absolute file path of asset
    /// - Parameter assetRelativePath: asset raltive path
    static std::filesystem::path GetAssetPath(const std::filesystem::path& assetRelativePath);
    /// This function returns the absolute file path of asset
    /// - Parameter assetRelativePath: asset raltive path
    static std::filesystem::path GetMeshPath(const std::filesystem::path& meshsourceRelativePath);
    /// This function returns the absolute file path of asset
    /// - Parameter assetRelativePath: asset raltive path
    static std::filesystem::path GetFontPath(const std::filesystem::path& fontRelativePath);
    /// This function returns the absolute file path of asset
    /// - Parameter assetRelativePath: asset raltive path
    static std::filesystem::path GetScenePath(const std::filesystem::path& sceneRelativePath);
    /// This function returns the absolute file path of asset
    /// - Parameter assetRelativePath: asset raltive path
    static std::filesystem::path GetTexturePath(const std::filesystem::path& textureRelativePath);
    /// This function returns the absolute file path of asset
    /// - Parameter assetRelativePath: asset raltive path
    static std::filesystem::path GetMaterialPath(const std::filesystem::path& materialRelativePath);
  private:
    ProjectConfig m_config;
    inline static Ref<Project> s_activeProject;
    friend class ProjectSerializer;
  };
} // namespace IKan
