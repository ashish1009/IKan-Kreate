//
//  AssetSerializer.cpp
//  IKan
//
//  Created by Ashish . on 19/07/24.
//

#include "AssetSerializer.hpp"

#include "Assets/AssetManager.hpp"

#include "Renderer/Mesh.hpp"
#include "Renderer/Graphics/Texture.hpp"

#include "Scene/Scene.hpp"
#include "Scene/Component.hpp"
#include "Scene/EntitySerializer.hpp"

#include "Project/Project.hpp"

namespace IKan
{
#define SerializeTextureMap(title, material) \
{\
Ref<Image> map = material->Get##title##Map(); \
AssetHandle mapHandle = map ? map->handle : (AssetHandle)0; \
std::string uniform = std::string(#title)+"Texture"; \
out << YAML::Key << uniform << YAML::Value << mapHandle; \
}
  
#define IK_DESERIALIZE_PROPERTY(propName, destination, node, defaultValue) destination = node[#propName] ? node[#propName].as<decltype(defaultValue)>() : defaultValue
  
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
    asset = Scene::Create(Project::GetActive()->GetConfig().sceneType);
    asset->handle = metadata.handle;
    return true;
  }
} // namespace IKan
