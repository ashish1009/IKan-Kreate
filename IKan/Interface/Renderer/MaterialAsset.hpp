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
    /// This Constructor loads the material asset shader
    /// - Parameter material: material reference
    MaterialAsset(const Ref<Shader>& shader);
    /// This Constructor loads the material asset shader
    /// - Parameter material: Shader reference
    MaterialAsset(const Ref<Material>& material);

    /// This function creates a font instance
    /// - Parameter filePath: shader file path
    static Ref<MaterialAsset> Create(const std::string& materialShaderPath);
    /// This functions Creates the material asset shader
    /// - Parameter shader: Shader reference
    static Ref<MaterialAsset> Create(const Ref<Shader>& shader);
    /// This functions Creates the material asset shader
    /// - Parameter material: material reference
    static Ref<MaterialAsset> Create(const Ref<Material>& material);

    ASSET_TYPE(MaterialAsset);
    
  private:
    std::string m_materialShaderPath;
    Ref<Material> m_material;
  };
  
  class MaterialTable
  {
  public:
    /// This creates material table
    /// - Parameter materialCount: material count
    MaterialTable(uint32_t materialCount);
    /// This destroys the material table
    ~MaterialTable();
    
  private:
    std::map<uint32_t /* Material Index */, Ref<MaterialAsset>> m_materials;
    uint32_t m_materialCount;
  };
} // namespace IKan
