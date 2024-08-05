//
//  AssetViewer.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/24.
//

#include "AssetViewer.hpp"
#include "Editor/RendererLayer.hpp"

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
    m_checkerboardTex = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Editor/CheckBoard.png");
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
    {
      UI::ScopedFont fixedSize(UI::Font::Get(UI::FontType::FixedWidth));
      ImGui::Text("Shader : %s", material->GetShader()->GetName().c_str());
      ImGui::Separator();
    }
    
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
            int count = data->DataSize / (int)sizeof(AssetHandle);
            
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
        bool useAlbedo = (bool)material->Get<float>("u_AlbedoTextureToggle");
        if (ImGui::Checkbox("Use", &useAlbedo))
        {
          material->Set("u_AlbedoTextureToggle", useAlbedo and albedoMap ? 1.0f : 0.0f);
          needsSerialize = true;
        }
        ImGui::SameLine();
        
        ImGui::ColorEdit3("Color##Albedo", glm::value_ptr(materialProperty.color), ImGuiColorEditFlags_NoInputs);
        if (ImGui::IsItemDeactivated())
        {
          needsSerialize = true;
        }
        
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        ImGui::DragFloat("Emission", &materialProperty.emission, 0.01f, 0.0f, 20.0f);
        if (ImGui::IsItemDeactivated())
        {
          needsSerialize = true;
        }
        
        ImGui::SetCursorPos(nextRowCursorPos);
        
        UI::PropertyGridHeaderEnd();
      } // Albedo
      
      // Normal
      if (UI::PropertyGridHeader("Normal", true, 4, 5))
      {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
        
        Ref<Image> normalMap = material->TryGetImage("u_NormalTexture");
        Ref<Texture> normalUITexture = normalMap ? normalMap : m_checkerboardTex;
        
        ImVec2 textureCursorPos = ImGui::GetCursorPos();
        UI::Image(normalUITexture, ImVec2(64, 64));
        if (ImGui::BeginDragDropTarget())
        {
          auto data = ImGui::AcceptDragDropPayload("asset_payload");
          if (data)
          {
            int count = data->DataSize / (int)sizeof(AssetHandle);
            
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
              
              normalMap = std::dynamic_pointer_cast<Image>(asset);
              m_materialAsset->SetNormalMap(normalMap);
              m_materialAsset->SetNormalMapToggle(true);
              needsSerialize = true;
            }
          }
          ImGui::EndDragDropTarget();
        } // Drag Drop
        ImGui::PopStyleVar();
        
        if (ImGui::IsItemHovered())
        {
          if (normalMap)
          {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(normalMap->GetfilePath().c_str());
            ImGui::PopTextWrapPos();
            UI::Image(normalUITexture, ImVec2(384, 384));
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
        if (normalMap and ImGui::Button("X", ImVec2(18, 18)))
        {
          m_materialAsset->SetNormalMap(nullptr);
          m_materialAsset->SetNormalMapToggle(false);
          needsSerialize = true;
        }
        ImGui::PopStyleVar();
        
        ImGui::SetCursorPos(properCursorPos);
        bool useNormal = (bool)material->Get<float>("u_NormalTextureToggle");
        if (ImGui::Checkbox("Use", &useNormal))
        {
          material->Set("u_NormalTextureToggle", useNormal and normalMap ? 1.0f : 0.0f);
          needsSerialize = true;
        }
        ImGui::SetCursorPos(nextRowCursorPos);
        UI::PropertyGridHeaderEnd();
      } // Normal
      
      // Metallic
      if (UI::PropertyGridHeader("Metallic", true, 4, 5))
      {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
        
        Ref<Image> metallicMap = material->TryGetImage("u_MetallicTexture");
        Ref<Texture> metallicUITexture = metallicMap ? metallicMap : m_checkerboardTex;
        
        ImVec2 textureCursorPos = ImGui::GetCursorPos();
        UI::Image(metallicUITexture, ImVec2(64, 64));
        if (ImGui::BeginDragDropTarget())
        {
          auto data = ImGui::AcceptDragDropPayload("asset_payload");
          if (data)
          {
            int count = data->DataSize / (int)sizeof(AssetHandle);
            
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
              
              metallicMap = std::dynamic_pointer_cast<Image>(asset);
              m_materialAsset->SetMetallicMap(metallicMap);
              m_materialAsset->SetMetallicMapToggle(true);
              needsSerialize = true;
            }
          }
          ImGui::EndDragDropTarget();
        } // Drag Drop
        ImGui::PopStyleVar();
        
        if (ImGui::IsItemHovered())
        {
          if (metallicMap)
          {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(metallicMap->GetfilePath().c_str());
            ImGui::PopTextWrapPos();
            UI::Image(metallicUITexture, ImVec2(384, 384));
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
        if (metallicMap and ImGui::Button("X", ImVec2(18, 18)))
        {
          m_materialAsset->SetMetallicMap(nullptr);
          m_materialAsset->SetMetallicMapToggle(false);
          needsSerialize = true;
        }
        ImGui::PopStyleVar();
        
        ImGui::SetCursorPos(properCursorPos);
        bool useMetallic = (bool)material->Get<float>("u_MetallicTextureToggle");
        if (ImGui::Checkbox("Use", &useMetallic))
        {
          material->Set("u_MetallicTextureToggle", useMetallic and metallicMap ? 1.0f : 0.0f);
          needsSerialize = true;
        }
        ImGui::SameLine();
        
        if (!metallicMap or !(bool)m_materialAsset->GetMetallicMapToggle())
        {
          if (ImGui::SliderFloat("Metallic", &materialProperty.metallic, 0.0f, 1.0f))
          {
            m_materialAsset->SetMetalness(materialProperty.metallic);
            needsSerialize = true;
          }
        }
        ImGui::SetCursorPos(nextRowCursorPos);
        UI::PropertyGridHeaderEnd();
      } // Metallic
      
      // Roughness
      if (UI::PropertyGridHeader("Roughness", true, 4, 5))
      {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
        
        Ref<Image> roughnessMap = material->TryGetImage("u_RoughnessTexture");
        Ref<Texture> roughnessUITexture = roughnessMap ? roughnessMap : m_checkerboardTex;
        
        ImVec2 textureCursorPos = ImGui::GetCursorPos();
        UI::Image(roughnessUITexture, ImVec2(64, 64));
        if (ImGui::BeginDragDropTarget())
        {
          auto data = ImGui::AcceptDragDropPayload("asset_payload");
          if (data)
          {
            int count = data->DataSize / (int)sizeof(AssetHandle);
            
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
              
              roughnessMap = std::dynamic_pointer_cast<Image>(asset);
              m_materialAsset->SetRoughnessMap(roughnessMap);
              m_materialAsset->SetRoughnessMapToggle(true);
              needsSerialize = true;
            }
          }
          ImGui::EndDragDropTarget();
        } // Drag Drop
        ImGui::PopStyleVar();
        
        if (ImGui::IsItemHovered())
        {
          if (roughnessMap)
          {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(roughnessMap->GetfilePath().c_str());
            ImGui::PopTextWrapPos();
            UI::Image(roughnessUITexture, ImVec2(384, 384));
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
        if (roughnessMap and ImGui::Button("X", ImVec2(18, 18)))
        {
          m_materialAsset->SetRoughnessMap(nullptr);
          m_materialAsset->SetRoughnessMapToggle(false);
          needsSerialize = true;
        }
        ImGui::PopStyleVar();
        
        ImGui::SetCursorPos(properCursorPos);
        bool useRoughness = bool(material->Get<float>("u_RoughnessTextureToggle"));
        if (ImGui::Checkbox("Use", &useRoughness))
        {
          material->Set("u_RoughnessTextureToggle", useRoughness and roughnessMap ? 1.0f : 0.0f);
          needsSerialize = true;
        }
        ImGui::SameLine();
        
        if (!roughnessMap or !(bool)m_materialAsset->GetRoughnessMapToggle())
        {
          if (ImGui::SliderFloat("Roughness ", &materialProperty.roughness, 0.0f, 1.0f))
          {
            m_materialAsset->SetRoughness(materialProperty.roughness);
            needsSerialize = true;
          }
        }
        ImGui::SetCursorPos(nextRowCursorPos);
        UI::PropertyGridHeaderEnd();
      } // Roughness
      
#if 0
      // Depth
      if (UI::PropertyGridHeader("Depth", true, 4, 5))
      {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
        
        Ref<Image> depthMap = material->TryGetImage("u_DepthTexture");
        Ref<Texture> depthUITexture = depthMap ? depthMap : m_checkerboardTex;
        
        ImVec2 textureCursorPos = ImGui::GetCursorPos();
        UI::Image(depthUITexture, ImVec2(64, 64));
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
              
              depthMap = std::dynamic_pointer_cast<Image>(asset);
              m_materialAsset->SetDepthMap(depthMap);
              m_materialAsset->SetDepthMapToggle(true);
              needsSerialize = true;
            }
          }
          ImGui::EndDragDropTarget();
        } // Drag Drop
        ImGui::PopStyleVar();
        
        if (ImGui::IsItemHovered())
        {
          if (depthMap)
          {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(depthMap->GetfilePath().c_str());
            ImGui::PopTextWrapPos();
            UI::Image(depthUITexture, ImVec2(384, 384));
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
        if (depthMap and ImGui::Button("X", ImVec2(18, 18)))
        {
          m_materialAsset->SetDepthMap(nullptr);
          m_materialAsset->SetDepthMapToggle(false);
          needsSerialize = true;
        }
        ImGui::PopStyleVar();
        
        ImGui::SetCursorPos(properCursorPos);
        bool useDepth = material->Get<float>("u_DepthTextureToggle");
        if (ImGui::Checkbox("Use", &useDepth))
        {
          material->Set("u_DepthTextureToggle", useDepth and depthMap ? 1.0f : 0.0f);
          needsSerialize = true;
        }
        ImGui::SameLine();
        
        if (depthMap and m_materialAsset->GetDepthMapToggle())
        {
          if (ImGui::DragFloat("Depth Scale", &materialProperty.depthScale))
          {
            m_materialAsset->SetDepthScale(materialProperty.depthScale);
            needsSerialize = true;
          }
        }
        ImGui::SetCursorPos(nextRowCursorPos);
        UI::PropertyGridHeaderEnd();
      } // Depth
#endif
    }
    
    if (needsSerialize)
    {
      IK_LOG_TRACE("Material Editor", "Serializing Material...");
      AssetImporter::Serialize(m_materialAsset);
    }
  }
} // namespace Kreator
