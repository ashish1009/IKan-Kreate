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
  void MaterialSerializer::Serialize(const AssetMetadata &metadata, const Ref<Asset> &asset) const
  {
    Ref<MaterialAsset> material = std::dynamic_pointer_cast<MaterialAsset>(asset);

    YAML::Emitter out;
    out << YAML::BeginMap; // Material
    out << YAML::Key << "Material" << YAML::Value;
    {
      out << YAML::BeginMap;

      out << YAML::Key << "AlbedoColor" << YAML::Value <<  material->GetAlbedoColor();
      out << YAML::Key << "Metallic" << YAML::Value <<  material->GetMetalness();
      out << YAML::Key << "Roughness" << YAML::Value <<  material->GetRoughness();
      out << YAML::Key << "DepthScale" << YAML::Value <<  material->GetDepthScale();
      out << YAML::Key << "TilingFactor" << YAML::Value <<  material->GetTilingFactor();

      out << YAML::EndMap;
    }
    out << YAML::EndMap; // Material

    std::ofstream fout(AssetManager::GetFileSystemPath(metadata));
    fout << out.c_str();
  }
} // namespace IKan
