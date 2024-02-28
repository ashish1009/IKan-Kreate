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
  
  // Material Editor -------------------------------------------------------------------------------------------------
  MaterialEditor::MaterialEditor()
  : AssetEditor("Material Editor")
  {
    m_checkerboardTex = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Editor/CheckBoard.png");
  }
  
  void MaterialEditor::SetAsset(const Ref<Asset>& asset)
  {
    m_materialAsset = std::dynamic_pointer_cast<MaterialAsset>(asset);
  }
  
  void MaterialEditor::OnClose()
  {
    m_materialAsset = nullptr;
  }
  
  void MaterialEditor::Render()
  {
    Ref<Material> material = m_materialAsset->GetMaterial();
    ImGui::Text("Shader: %s", material->GetShader()->GetName().c_str());
    ImGui::Separator();
    
    bool needsSerialize = false;
    // Materials ------------------------------------------------------------------------------
    {
      auto& materialProperty = material->Get<MaterialProperty>("u_Material");

      // Albedo
      if (UI::PropertyGridHeader("Albedo", true, 4, 5))
      {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
        
        Ref<Image> albedoMap = material->TryGetImage("u_AlbedoTexture");
        Ref<Texture> albedoUITexture = albedoMap ? albedoMap : m_checkerboardTex;

        ImVec2 textureCursorPos = ImGui::GetCursorPos();
        UI::Image(albedoUITexture, ImVec2(64, 64));
        if (ImGui::BeginDragDropTarget())
        {
          auto data = ImGui::AcceptDragDropPayload("asset_payload");
          if (data)
          {
            int count = data->DataSize / sizeof(AssetHandle);
            
            for (int i = 0; i < count; i++)
            {
              if (count > 1)
                break;
              
              AssetHandle assetHandle = *(((AssetHandle*)data->Data) + i);
              Ref<Asset> asset = AssetManager::GetAsset<Asset>(assetHandle);
              if (!asset or asset->GetAssetType() != AssetType::Image)
              {
                break;
              }
              
              albedoMap = std::dynamic_pointer_cast<Image>(asset);
              m_materialAsset->SetAlbedoMap(albedoMap);
              m_materialAsset->SetAlbedoMapToggle(true);
              needsSerialize = true;
            }
          }
          ImGui::EndDragDropTarget();
        } // Drag Drop
        ImGui::PopStyleVar();
        
        if (ImGui::IsItemHovered())
        {
          if (albedoMap)
          {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(albedoMap->GetfilePath().c_str());
            ImGui::PopTextWrapPos();
            UI::Image(albedoUITexture, ImVec2(384, 384));
            ImGui::EndTooltip();
          }
          if (ImGui::IsItemClicked())
          {
          }
        }

        ImVec2 nextRowCursorPos = ImGui::GetCursorPos();
        ImGui::SameLine();
        ImVec2 properCursorPos = ImGui::GetCursorPos();
        ImGui::SetCursorPos(textureCursorPos);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        if (albedoMap and ImGui::Button("X", ImVec2(18, 18)))
        {
          m_materialAsset->SetAlbedoMap(nullptr);
          m_materialAsset->SetAlbedoMapToggle(false);
          needsSerialize = true;
        }
        ImGui::PopStyleVar();

        ImGui::SetCursorPos(properCursorPos);
        ImGui::ColorEdit3("Color##Albedo", glm::value_ptr(materialProperty.color), ImGuiColorEditFlags_NoInputs);
        if (ImGui::IsItemDeactivated())
        {
          needsSerialize = true;
        }
        
        ImGui::SameLine();
        bool useAlbedo = material->Get<float>("u_AlbedoTextureToggle");
        if (ImGui::Checkbox("Use", &useAlbedo))
        {
          material->Set("u_AlbedoTextureToggle", useAlbedo ? 1.0f : 0.0f);
          needsSerialize = true;
        }

        UI::PropertyGridHeaderEnd();
      }
    }
  }

} // namespace Kreator
