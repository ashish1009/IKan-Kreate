//
//  MaterialAsset.hpp
//  IKan
//
//  Created by Ashish . on 10/09/23.
//

#pragma once

#include "Asset/Asset.hpp"
#include "Renderer/MeshMaterial.hpp"

namespace IKan
{
  /// - Note: Create the Material Asset using AssetManager::CreateMemoryOnlyAsset<>() to store the asset in Manager
  ///         And load the same easily
  class MaterialAsset : public Asset
  {
  public:
    /// This function creates a font instance
    /// - Parameter filePath: font file path
    MaterialAsset(const std::string& materialShaderPath, const std::string &name);
    /// This Constructor loads the material asset shader
    /// - Parameter material: material reference
    MaterialAsset(const Ref<Shader>& shader, const std::string &name);
    /// This Constructor loads the material asset shader
    /// - Parameter material: Shader reference
    MaterialAsset(const Ref<Material>& material, const std::string &name);
     
    /// This function returns the material from asset
    Ref<Material>& GetMaterial();
    
    /// This function creates a font instance
    /// - Parameter filePath: shader file path
    static Ref<MaterialAsset> Create(const std::string& materialShaderPath, const std::string &name);
    /// This functions Creates the material asset shader
    /// - Parameter shader: Shader reference
    static Ref<MaterialAsset> Create(const Ref<Shader>& shader, const std::string &name);
    /// This functions Creates the material asset shader
    /// - Parameter material: material reference
    static Ref<MaterialAsset> Create(const Ref<Material>& material, const std::string &name);

    ASSET_TYPE(MaterialAsset);
    
  private:
    // Member Functions ---------------------------------------------------------------------------------------------

    // Member Variables ---------------------------------------------------------------------------------------------
    std::string m_materialShaderPath;
    Ref<Material> m_material;
  };
  
  class MaterialTable
  {
  public:
    /// This creates material table
    /// - Parameter baseMaterial: Base material instance
    MaterialTable(Ref<Material> baseMaterial);
    /// This creates material table
    /// - Parameter baseMaterialShader: Base material shader instance
    MaterialTable(Ref<Shader> baseMaterialShader);
    /// This creates material table
    /// - Parameter baseMaterialShaderFile: Base material shader file path
    MaterialTable(const std::string& baseMaterialShaderFile);
    /// This destroys the material table
    ~MaterialTable();
    
    /// This function creates new material assets
    void CreateNewMaterialAsset(const std::filesystem::path& materialBaseDir, const std::string& materialName);
    /// This function sets the material Asset in table
    /// - Parameters:
    ///   - index: material index
    ///   - material: material asset
    void SetMaterialAsset(uint32_t index, Ref<MaterialAsset> materialAsset);
    /// This function serialzes the material
    void Serialize();

    /// This function returns the size of material assets
    uint32_t GetSize() const;
    
    /// This function returns the materials
    std::map<uint32_t, Ref<MaterialAsset>>& GetMaterialAssets();
    /// This function returns the materials
    const std::map<uint32_t, Ref<MaterialAsset>>& GetMaterialAssets() const;
    
  private:
    std::map<uint32_t /* Material Index */, Ref<MaterialAsset>> m_materialAssetess;
    uint32_t m_materialCount = 0;
    Ref<Material> m_baseMaterial;
  };
} // namespace IKan
