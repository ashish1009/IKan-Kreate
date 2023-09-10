//
//  MaterialAsset.hpp
//  IKan
//
//  Created by Ashish . on 10/09/23.
//

#pragma once

#include "Asset/Asset.hpp"
#include "Renderer/Material.hpp"

namespace IKan
{
  class MaterialAsset : public Asset
  {
  public:
    /// This function creates a font instance
    /// - Parameter filePath: font file path
    MaterialAsset(const std::string& materialShaderPath);
    
    /// This function creates a font instance
    /// - Parameter filePath: shader file path
    static Ref<MaterialAsset> Create(const std::string& materialShaderPath);

    ASSET_TYPE(MaterialAsset);
    
  private:
    std::string m_materialShaderPath;
    Ref<Material> m_material;
  };
} // namespace IKan
