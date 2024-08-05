//
//  MaterialAsset.hpp
//  IKan
//
//  Created by Ashish . on 05/08/24.
//

#pragma once

#include "Assets/Asset.hpp"
#include "Renderer/Materials.hpp"

namespace IKan
{
#define TextureMapAPIs(title) \
float& Get##title##MapToggle(); \
void Set##title##MapToggle(float value); \
Ref<Image> Get##title##Map(); \
void Set##title##Map(Ref<Image> texture); \

  struct MaterialProperty
  {
    glm::vec3 color = {1.0f, 1.0f, 1.0f};
    float emission = 0.0f;
    float metallic = 0.5f;
    float roughness = 0.5f;
    float depthScale = 0.001f;
  };
  
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
    
    /// This function returns the Albedo color
    glm::vec3& GetAlbedoColor();
    /// This function sets the Albedo color
    /// - Parameter value: new value
    void SetAlbedoColor(const glm::vec3& color);
    
    /// This function returns the Emmision
    float& GetEmission();
    /// This function sets the Emmision
    /// - Parameter value: new value
    void SetEmission(float value);
    
    /// This function returns the Metalness
    float& GetMetalness();
    /// This function sets the Metalness
    /// - Parameter value: new value
    void SetMetalness(float value);
    
    /// This function returns the Roughness
    float& GetRoughness();
    /// This function sets the Roughness
    /// - Parameter value: new value
    void SetRoughness(float value);
    
    /// This function returns the DepthScale
    float& GetDepthScale();
    /// This function sets the DepthScale
    /// - Parameter value: new value
    void SetDepthScale(float value);
    
    TextureMapAPIs(Albedo);
    TextureMapAPIs(Normal);
    TextureMapAPIs(Metallic);
    TextureMapAPIs(Roughness);
    TextureMapAPIs(Depth);
    TextureMapAPIs(Ao);
    
    /// This function returns the material
    Ref<Material> GetMaterial();
    
    /// This function creates material asset
    static Ref<MaterialAsset> Create();
    
    ASSET_TYPE(Material);
    
  private:
    Ref<Material> m_material;
  };
  
  class MaterialTable
  {
  public:
    /// This creates the material table
    /// - Parameter materialCount: material count
    MaterialTable();
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
