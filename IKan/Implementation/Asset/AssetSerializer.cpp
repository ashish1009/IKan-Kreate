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
    IK_ASSERT(false);
    return true;
  }
  
  bool SceneAssetSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    IK_ASSERT(false);
    return true;
  }
  
  bool FontSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    IK_ASSERT(false);
    return true;
  }
  
  bool MeshSourceSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    IK_ASSERT(false);
    return true;
  }  
} // namespace IKan
