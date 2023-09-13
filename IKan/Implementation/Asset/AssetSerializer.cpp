//
//  AssetSerializer.cpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#include "AssetSerializer.hpp"
#include "Renderer/UI/Font.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Scene/Scene.hpp"
#include "Asset/AssetManager.hpp"
#include "Renderer/Mesh.hpp"

namespace IKan
{
  bool ImageSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = Image::Create(AssetManager::GetFileSystemPathString(metadata));
    asset->handle = metadata.handle;
    return true;
  }
  
  bool SceneAssetSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = Scene::Create();
    asset->handle = metadata.handle;
    return true;
  }
  
  bool FontSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = Font::Create(AssetManager::GetFileSystemPathString(metadata));
    asset->handle = metadata.handle;
    return true;
  }
  
  bool MeshSourceSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = MeshSource::Create(AssetManager::GetFileSystemPathString(metadata));
    asset->handle = metadata.handle;
    return true;
  }

  void MeshMaterialSerializer::Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset) const
  {
    
  }
  bool MeshMaterialSerializer::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const
  {
    return false;
  }

} // namespace IKan
