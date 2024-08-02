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
    
    /// This funcion updates auto save scene
    /// - Parameter flag: auto save flag
    void UpdateAutoSaveScene(bool flag);
    /// This funcion updates auto save scene time interval
    /// - Parameter time: auto save time interval
    void UpdateAutoSaveSceneTimeInterval(int32_t time);
    /// This funcion updates auto save scene time interval
    /// - Parameter path: scene file path
    void UpdateStartupScene(const std::filesystem::path& path);
    /// This funcion updates Scene type of project
    /// - Parameter sceneType: scene sceneType
    void UpdateSceneType(SceneType sceneType);

    // Getter APIs ---------------------------------------------------------------------------------------------------
    /// This function returns the project config
    const ProjectConfig& GetConfig() const;
    /// This function returns the project name
    static const std::string& GetProjectName();

    // Asset Path APIs -----------------------------------------------------------------------------------------------
    /// This function returns the project directory path
    static std::filesystem::path GetProjectDirectory();
    /// This function returns the project asset
    static std::filesystem::path GetAssetRegistryPath();
    
    /// This function returns the project asset path
    static std::filesystem::path GetAssetDirectory();
    /// This function returns the project mesh path
    static std::filesystem::path GetMeshDirectory();
    /// This function returns the project font path
    static std::filesystem::path GetFontDirectory();
    /// This function returns the project scene path
    static std::filesystem::path GetSceneDirectory();
    /// This function returns the project texture path
    static std::filesystem::path GetTextureDirectory();
    /// This function returns the project material path
    static std::filesystem::path GetMaterialDirectory();
    /// This function returns the project physics path
    static std::filesystem::path GetPhysicsDirectory();
    /// This function returns the project prefab path
    static std::filesystem::path GetPrefabDirectory();

    /// This function returns the absolute file path of asset
    /// - Parameter assetRelativePath: asset relative path
    static std::filesystem::path GetAssetPath(const std::filesystem::path& assetRelativePath);
    /// This function returns the absolute file path of mesh
    /// - Parameter assetRelativePath: mesh relative path
    static std::filesystem::path GetMeshPath(const std::filesystem::path& meshsourceRelativePath);
    /// This function returns the absolute file path of font
    /// - Parameter assetRelativePath: font relative path
    static std::filesystem::path GetFontPath(const std::filesystem::path& fontRelativePath);
    /// This function returns the absolute file path of scene
    /// - Parameter assetRelativePath: scene relative path
    static std::filesystem::path GetScenePath(const std::filesystem::path& sceneRelativePath);
    /// This function returns the absolute file path of texture
    /// - Parameter assetRelativePath: texture relative path
    static std::filesystem::path GetTexturePath(const std::filesystem::path& textureRelativePath);
    /// This function returns the absolute file path of material
    /// - Parameter assetRelativePath: material relative path
    static std::filesystem::path GetMaterialPath(const std::filesystem::path& materialRelativePath);
    /// This function returns the absolute file path of physics
    /// - Parameter assetRelativePath: physics relative path
    static std::filesystem::path GetPhysicsPath(const std::filesystem::path& physicsRelativePath);
    /// This function returns the absolute file path of prefab
    /// - Parameter assetRelativePath: prefab relative path
    static std::filesystem::path GetPrefabPath(const std::filesystem::path& prefabRelativePath);

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
