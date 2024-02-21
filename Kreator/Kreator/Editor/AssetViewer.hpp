//
//  AssetViewer.hpp
//  Kreator
//
//  Created by Ashish . on 21/02/24.
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
} // namespace Kreator
