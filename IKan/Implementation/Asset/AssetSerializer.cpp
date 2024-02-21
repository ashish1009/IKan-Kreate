//
//  AssetSerializer.cpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#include "AssetSerializer.hpp"
#include "Asset/AssetManager.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Scene/Scene.hpp"

namespace IKan
{
  bool ImageSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = std::dynamic_pointer_cast<Image>(TextureFactory::Create(AssetManager::GetFileSystemPath(metadata)));
    asset->handle = metadata.handle;
    return true;
  }
  bool MeshSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = Mesh::Create(AssetManager::GetFileSystemPath(metadata));
    asset->handle = metadata.handle;
    return true;
  }
  bool SceneAssetSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = Scene::Create();
    asset->handle = metadata.handle;
    return true;
  }
} // namespace IKan
