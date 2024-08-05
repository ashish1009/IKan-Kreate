//
//  AssetViewer.hpp
//  Kreator
//
//  Created by Ashish . on 05/08/24.
//

#pragma once

#include "Panels/SceneHierarchyPanel.hpp"

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
  
  class MaterialEditor : public AssetEditor
  {
  public:
    /// Default Material Editor constructor
    MaterialEditor();
    
    /// @see AssetEditor
    virtual void SetAsset(const Ref<Asset>& asset) override;
    
  private:
    /// @see AssetEditor
    virtual void OnClose() override;
    /// @see AssetEditor
    virtual void Render() override;
    
  private:
    Ref<MaterialAsset> m_materialAsset;
    Ref<Texture> m_checkerboardTex;
  };
} // namespace Kreator
