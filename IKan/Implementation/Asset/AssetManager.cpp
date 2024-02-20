//
//  AssetManager.cpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#include "AssetManager.hpp"
#include "Project/Project.hpp"

namespace IKan
{
  static AssetMetadata NullMetadata;
  
  void AssetManager::Initialize()
  {
    IK_PROFILE()
    IK_LOG_INFO(LogModule::Asset, "Initializing current Asset Manager");

    s_assetRegistry.Clear();
    AssetImporter::Initialize();

    // Loads the asset reguistry file path (If present) and update the registry data in manager
    LoadAssetRegistry();
    
    // Reload the assets. Process the directory recursively and write the asset data in registry file
    ReloadAssets();
  }
  
  void AssetManager::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Asset, "Shutting down current Asset Manager");

    s_assetRegistry.Clear();
  }
  
  void AssetManager::LoadAssetRegistry()
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Asset, "Loading Asset Registry");
    
    const auto& assetRegistryPath = Project::GetAssetRegistryPath();
    RETURN_IF (!std::filesystem::exists(assetRegistryPath))
    
    std::ifstream stream(assetRegistryPath);
    IK_ASSERT(stream);

    std::stringstream strStream;
    strStream << stream.rdbuf();
    
    YAML::Node data = YAML::Load(strStream.str());
    auto handles = data["Assets"];
    
    if (!handles)
    {
      IK_LOG_ERROR(LogModule::Asset, "Asset Registry appears to be corrupted!");
      return;
    }

    for (const auto& entry : handles)
    {
      // Relative file path from asset directory
      std::string filepath = entry["FilePath"].as<std::string>();
      
      AssetMetadata metadata;
      metadata.handle = entry["Handle"].as<uint64_t>();
      metadata.filePath = filepath;
      metadata.type = (AssetType)AssetUtils::AssetTypeFromString(entry["Type"].as<std::string>());

      RETURN_IF (metadata.type == AssetType::Invalid)

      // Relocate the file if not present in directory
      if (!std::filesystem::exists(AssetManager::GetFileSystemPath(metadata)))
      {
        IK_LOG_WARN("[AssetManager]", "Missing asset '{0}' detected in registry file, trying to locate...", metadata.filePath.string());
        
        std::string mostLikelyCandidate;
        uint32_t bestScore = 0;

        for (auto& pathEntry : std::filesystem::recursive_directory_iterator(Project::GetAssetDirectory()))
        {
          const std::filesystem::path& path = pathEntry.path();
          if (path.filename() != metadata.filePath.filename())
          {
            continue;
          }

          if (bestScore > 0)
          {
            IK_LOG_WARN("[AssetManager]", "Multiple candidates found...");
          }
          
          std::vector<std::string> candiateParts = Utils::String::SplitString(path.string(), "/\\");

          uint32_t score = 0;
          for (const auto& part : candiateParts)
          {
            if (filepath.find(part) != std::string::npos)
            {
              score++;
            }
          }

          IK_LOG_WARN("[AssetManager]", "'{0}' has a score of {1}, best score is {2}", path.string(), score, bestScore);

          if (bestScore > 0 and score == bestScore)
          {
            // TODO: How do we handle this?
            // Probably prompt the user at this point?
          }
          
          if (score <= bestScore)
          {
            continue;
          }
          
          bestScore = score;
          mostLikelyCandidate = path.string();
        }
        
        if (mostLikelyCandidate.empty() && bestScore == 0)
        {
          IK_LOG_WARN("[AssetManager]", "Failed to locate a potential match for '{0}'", metadata.filePath.string());
          continue;
        }
        
        std::replace(mostLikelyCandidate.begin(), mostLikelyCandidate.end(), '\\', '/');
        metadata.filePath = std::filesystem::relative(mostLikelyCandidate, Project::GetActive()->GetAssetDirectory());
        IK_LOG_WARN("[AssetManager]", "Found most likely match '{0}'", metadata.filePath.string());
      }
      
      if (metadata.handle == static_cast<AssetHandle>(0))
      {
        IK_LOG_WARN(LogModule::Asset, "AssetHandle for {0} is 0, this shouldn't happen.", metadata.filePath.c_str());
        continue;
      }
      
      s_assetRegistry[metadata.filePath] = metadata;
    }
    IK_LOG_INFO(LogModule::Asset, "Loaded {0} asset entries", s_assetRegistry.Count());
  }
  
  void AssetManager::ReloadAssets()
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Asset, "Processing Asset path and load in Registry");
    
    ProcessDirectory(Project::GetAssetDirectory().string());
    WriteRegistryToFile();
  }

  void AssetManager::ProcessDirectory(const std::string &directoryPath)
  {
    for (auto entry : std::filesystem::directory_iterator(directoryPath))
    {
      if (entry.is_directory())
      {
        ProcessDirectory(entry.path());
      }
      else
      {
        ImportAsset(entry.path());
      }
    }
  }
  
  AssetHandle AssetManager::ImportAsset(const std::filesystem::path& filepath)
  {
    std::filesystem::path path = GetRelativePath(filepath);
    if (s_assetRegistry.Contains(path))
    {
      return s_assetRegistry.GetMetadata(path).handle;
    }
    
    AssetType type = GetAssetTypeFromPath(path);
    if (type == AssetType::Invalid)
    {
      return static_cast<AssetHandle>(0);
    }
    
    AssetMetadata metadata;
    const auto& relPath = GetRelativePath(filepath);
    metadata.handle = Hash::GenerateFNV(relPath);
    metadata.filePath = relPath;
    metadata.type = type;
    s_assetRegistry[metadata.filePath] = metadata;
    
    return metadata.handle;
  }

  void AssetManager::WriteRegistryToFile()
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Asset, "Writting Assets in file");
    // Sort assets by UUID to make project managment easier
    struct AssetRegistryEntry
    {
      std::string filePath;
      AssetType type;
    };
    
    std::map<UUID, AssetRegistryEntry> sortedMap;
    for (auto& [filepath, metadata] : s_assetRegistry)
    {
      if (!std::filesystem::exists(GetFileSystemPath(metadata)))
      {
        continue;
      }
      
      if (metadata.isMemoryAsset)
      {
        continue;
      }
      
      std::string pathToSerialize = metadata.filePath.string();
      std::replace(pathToSerialize.begin(), pathToSerialize.end(), '\\', '/');
      sortedMap[metadata.handle] = { pathToSerialize, metadata.type };
    }
    
    IK_LOG_INFO(LogModule::Asset, "Serializing asset registry with {0} entries", sortedMap.size());
    
    YAML::Emitter out;
    out << YAML::BeginMap;
    
    out << YAML::Key << "Assets" << YAML::BeginSeq;
    for (auto& [handle, entry] : sortedMap)
    {
      IK_LOG_TRACE(LogModule::Asset, "  {0} : {1}", handle, entry.filePath);
      out << YAML::BeginMap;
      out << YAML::Key << "Handle" << YAML::Value << handle;
      out << YAML::Key << "FilePath" << YAML::Value << entry.filePath;
      out << YAML::Key << "Type" << YAML::Value << AssetUtils::AssetTypeToString(entry.type);
      out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;
    
    const std::string& assetRegistryPath = Project::GetAssetRegistryPath().string();
    std::ofstream fout(assetRegistryPath);
    fout << out.c_str();
  }
  
  std::filesystem::path AssetManager::GetRelativePath(const std::filesystem::path& filepath)
  {
    std::string temp = filepath.string();
    if (temp.find(Project::GetActive()->GetAssetDirectory().string()) != std::string::npos)
    {
      return std::filesystem::relative(filepath, Project::GetActive()->GetAssetDirectory());
    }
    return filepath;
  }
  AssetType AssetManager::GetAssetTypeFromPath(const std::filesystem::path& path)
  {
    return GetAssetTypeFromExtension(path.extension().string());
  }
  AssetType AssetManager::GetAssetTypeFromExtension(const std::string& extension)
  {
    std::string ext = Utils::String::ToLowerCopy(extension);
    if (s_assetExtensionMap.find(ext) == s_assetExtensionMap.end())
    {
      return AssetType::Invalid;
    }
    return s_assetExtensionMap.at(ext.c_str());
  }
  std::filesystem::path AssetManager::GetFileSystemPath(const AssetMetadata& metadata)
  {
    return Project::GetAssetDirectory() / metadata.filePath;
  }
  
  AssetHandle AssetManager::GetAssetHandleFromFilePath(const std::filesystem::path& filepath)
  {
    return s_assetRegistry.Contains(filepath) ? s_assetRegistry.GetMetadata(filepath).handle : static_cast<AssetHandle>(0);
  }
  
  const AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
  {
    return GetMetadataInternal(handle);
  }
  
  const AssetMetadata& AssetManager::GetMetadata(const std::filesystem::path& filepath)
  {
    if (s_assetRegistry.Contains(filepath))
    {
      return s_assetRegistry.GetMetadata(filepath);
    }
    return NullMetadata;
  }
  
  void AssetManager::OnAssetRenamed(AssetHandle assetHandle, const std::filesystem::path& newFilePath)
  {
    AssetMetadata metadata = GetMetadata(assetHandle);
    RETURN_IF (!metadata.IsValid())
    
    s_assetRegistry.Remove(metadata.filePath);
    metadata.filePath = GetRelativePath(newFilePath);
    s_assetRegistry[metadata.filePath] = metadata;
    WriteRegistryToFile();
  }
  
  void AssetManager::OnAssetMoved(AssetHandle assetHandle, const std::filesystem::path& destinationPath)
  {
    AssetMetadata metadata = GetMetadata(assetHandle);
    RETURN_IF (!metadata.IsValid())
    
    s_assetRegistry.Remove(metadata.filePath);
    metadata.filePath = destinationPath / metadata.filePath.filename();
    s_assetRegistry[metadata.filePath] = metadata;
    
    WriteRegistryToFile();
  }
  
  void AssetManager::OnAssetDeleted(AssetHandle assetHandle)
  {
    AssetMetadata metadata = GetMetadata(assetHandle);
    RETURN_IF (!metadata.IsValid())
    
    s_assetRegistry.Remove(metadata.filePath);
    s_loadedAssets.erase(assetHandle);
    WriteRegistryToFile();
  }
  
  bool AssetManager::ReloadData(AssetHandle assetHandle)
  {
    auto& metadata = GetMetadataInternal(assetHandle);
    Ref<Asset> asset;
    metadata.isDataLoaded = AssetImporter::TryLoadData(metadata, asset);
    if (metadata.isDataLoaded)
    {
      s_loadedAssets[assetHandle] = asset;
    }
    return metadata.isDataLoaded;
  }
  
  const AssetRegistry& AssetManager::GetAssetRegistry()
  {
    return s_assetRegistry;
  }
  
  AssetMetadata& AssetManager::GetMetadataInternal(AssetHandle handle)
  {
    for (auto& [filepath, metadata] : s_assetRegistry)
    {
      if (metadata.handle == handle)
      {
        return metadata;
      }
    }
    return NullMetadata;
  }
} // namespace IKan
