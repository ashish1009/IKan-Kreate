//
//  DefaultAssetViewer.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#include "DefaultAssetViewer.hpp"

namespace Kreator
{
  ImageViewer::ImageViewer()
  : AssetEditor("Edit Texture")
  {
    SetMinSize(200, 600);
    SetMaxSize(500, 1000);
  }
  
  void ImageViewer::SetAsset(const Ref<Asset>& asset)
  {
    m_asset = std::dynamic_pointer_cast<Image>(asset);
  }
  
  void ImageViewer::OnClose()
  {
    m_asset = nullptr;
  }
  
  void ImageViewer::Render()
  {
    if (!m_asset)
    {
      SetOpen(false);
    }
    
    float textureWidth = (float)m_asset->GetWidth();
    float textureHeight = (float)m_asset->GetHeight();
    float imageSize = ImGui::GetWindowWidth() - 40;
    imageSize = glm::min(imageSize, 500.0f);
    
    ImGui::SetCursorPosX(20);
    ImGui::Image(INT2VOIDP(m_asset->GetRendererID()), { imageSize, imageSize });
    
    Kreator_UI::BeginPropertyGrid();
    UI::BeginDisabled();
    Kreator_UI::Property("Width", textureWidth);
    Kreator_UI::Property("Height", textureHeight);
    UI::EndDisabled();
    Kreator_UI::EndPropertyGrid();
  }
  
  MaterialViewer::MaterialViewer()
  : AssetEditor("Edit Material")
  {
    SetMinSize(200, 600);
    SetMaxSize(500, 1000);
  }
  
  void MaterialViewer::SetAsset(const Ref<Asset>& asset)
  {
    m_asset = std::dynamic_pointer_cast<MaterialAsset>(asset);
  }
  
  void MaterialViewer::OnClose()
  {
    m_asset = nullptr;
  }
  
  void MaterialViewer::Render()
  {
    if (!m_asset)
    {
      SetOpen(false);
    }    
  }
} // namespace Kreator
