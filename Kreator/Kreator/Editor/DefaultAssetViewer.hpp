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
} // namespace Kreator
