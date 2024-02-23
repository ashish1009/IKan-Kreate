//
//  MaterialAsset.hpp
//  IKan
//
//  Created by Ashish . on 23/02/24.
//

#pragma once

#include "Asset/Asset.hpp"
#include "Renderer/Material.hpp"

namespace IKan
{
  class MaterialAsset : public Asset
  {
  public:
    /// Default constructor of material asset
    MaterialAsset();
    /// Construct material asset with material instance
    /// - Parameter material: material instance
    MaterialAsset(Ref<Material> material);
    /// Default destructor of material asset
    ~MaterialAsset();
    
    /// This function returns the material
    Ref<Material> GetMaterial() const;

    ASSET_TYPE(Material);
    
  private:
    Ref<Material> m_material;
  };
  
  class MaterialTable
  {
  public:
    /// This creates the material table
    /// - Parameter materialCount: material count
    MaterialTable(uint32_t materialCount = 1);
    /// This copy the material table
    /// - Parameter other: material table
    MaterialTable(Ref<MaterialTable> other);
    /// Default destructor of material table
    ~MaterialTable() = default;
    
    /// This functin checks if material is present
    /// - Parameter materialIndex: material index
    bool HasMaterial(uint32_t materialIndex) const;
    /// This funcion set the amterial
    /// - Parameters:
    ///   - index: material index
    ///   - material: material
    void SetMaterial(uint32_t index, Ref<MaterialAsset> material);
    /// This function clears the material
    /// - Parameter index: material index
    void ClearMaterial(uint32_t index);
    
    /// This function returns the material from table
    /// - Parameter materialIndex: material index
    Ref<MaterialAsset> GetMaterial(uint32_t materialIndex) const;
    
    /// This function returns the reference of all the materials
    std::map<uint32_t, Ref<MaterialAsset>>& GetMaterials();
    
    /// This function returns the const reference of all the materials
    const std::map<uint32_t, Ref<MaterialAsset>>& GetMaterials() const;
    
    /// This function returns the count of materials
    uint32_t GetMaterialCount() const;

    /// This function updates the material count
    void SetMaterialCount(uint32_t materialCount);

    /// This function clears the materials
    void Clear();

  private:
    std::map<uint32_t, Ref<MaterialAsset>> m_materials;
    uint32_t m_materialCount;
  };
} // namespace IKan
