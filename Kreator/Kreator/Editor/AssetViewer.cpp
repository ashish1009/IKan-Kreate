//
//  AssetViewer.cpp
//  Kreator
//
//  Created by Ashish . on 21/02/24.
//

#include "AssetViewer.hpp"
#include "Layer/KreatorLayer.hpp"

namespace Kreator
{
  ImageViewer::ImageViewer()
  : AssetEditor("Edit Texture")
  {
    SetMinSize(200, 600);
    SetMaxSize(1000, 1000);
  }
  
  ImageViewer::~ImageViewer()
  {
    OnClose();
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
    
    UI::BeginPropertyGrid();
    UI::BeginDisabled();
    UI::Property("Width", textureWidth);
    UI::Property("Height", textureHeight);
    UI::EndDisabled();
    UI::EndPropertyGrid();
  }
} // namespace Kreator
