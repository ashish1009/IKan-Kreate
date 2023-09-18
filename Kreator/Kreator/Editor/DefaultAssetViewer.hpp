//
//  DefaultAssetViewer.hpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#pragma once

namespace Kreator
{
  class ImageViewer : public AssetEditor
  {
  public:
    /// Default Texture Viewer constructor
    ImageViewer();
    ~ImageViewer();
    
    /// @see AssetEditor
    virtual void SetAsset(const Ref<Asset>& asset) override;
    
  private:
    /// @see AssetEditor
    virtual void OnClose() override;
    /// @see AssetEditor
    virtual void Render() override;
    
  private:
    Ref<Image> m_asset;
  };
  
  class MaterialViewer : public AssetEditor
  {
  public:
    /// Default Texture Viewer constructor
    MaterialViewer();
    ~MaterialViewer();
    /// @see AssetEditor
    virtual void SetAsset(const Ref<Asset>& asset) override;
    
  private:
    /// This function rendere Texture property of material
    /// @param name name of material
    /// @param material material instance
    void ShowTextureProperty(const std::string& name, Ref<Material>& material);
    /// @see AssetEditor
    virtual void OnClose() override;
    /// @see AssetEditor
    virtual void Render() override;
    
  private:
    Ref<Image> m_checkerboardTex;
    Ref<MaterialAsset> m_asset;
  };
} // namespace Kreator
