//
//  DefaultAssetViewer.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#include "DefaultAssetViewer.hpp"
#include "RendererLayer.hpp"

namespace Kreator
{
  ImageViewer::ImageViewer()
  : AssetEditor("Edit Texture")
  {
    SetMinSize(200, 600);
    SetMaxSize(500, 1000);
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
    
    Kreator_UI::BeginPropertyGrid();
    UI::BeginDisabled();
    Kreator_UI::Property("Width", textureWidth);
    Kreator_UI::Property("Height", textureHeight);
    UI::EndDisabled();
    Kreator_UI::EndPropertyGrid();
  }
  
  MaterialViewer::MaterialViewer()
  : AssetEditor("Material")
  {
    SetMinSize(200, 600);
    SetMaxSize(500, 1000);

    // No texture Image
    m_checkerboardTex = Image::Create(Utils::FileSystem::Absolute(RendererLayer::Get().GetClientResorucePath() /"Textures/Editor/CheckBoard.png"));
  }
  
  MaterialViewer::~MaterialViewer()
  {
    m_checkerboardTex.reset();
    OnClose();
  }
  
  void MaterialViewer::SetAsset(const Ref<Asset>& asset)
  {
    m_asset = std::dynamic_pointer_cast<MaterialAsset>(asset);
  }
  
  void MaterialViewer::OnClose()
  {
    m_asset = nullptr;
  }
  
  void MaterialViewer::ShowTextureProperty(const std::string& name, Ref<Material>& material)
  {
    ImGui::PushID(name.c_str());
    if (ImGui::CollapsingHeader(name.c_str(), nullptr, ImGuiTreeNodeFlags_DefaultOpen))
    {
      std::string textureString = "u_";
      textureString += name;
      textureString += "Texture";
      std::string texturetoggleString = textureString + "Toggle";
      
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
      float& useMap = material->Get<float>(texturetoggleString);
      
      Ref<Image> map = material->TryGetImage(textureString);
      bool hasMap = map != nullptr;
      Ref<Image> UITexture = hasMap? map : m_checkerboardTex;
      UI::Image(UITexture, ImVec2(48, 48));
      
      if (ImGui::BeginDragDropTarget())
      {
        auto data = ImGui::AcceptDragDropPayload("asset_payload");
        if (data)
        {
          int32_t count = data->DataSize / sizeof(AssetHandle);
          
          for (int32_t i = 0; i < count; i++)
          {
            if (count > 1)
            {
              break;
            }
            
            AssetHandle assetHandle = *(((AssetHandle*)data->Data) + i);
            Ref<Asset> asset = AssetManager::GetAsset<Asset>(assetHandle);
            if (!asset || asset->GetAssetType() != AssetType::Image)
            {
              break;
            }
            map = std::dynamic_pointer_cast<Image>(asset);
            material->Set(textureString, map);
          }
        }
        ImGui::EndDragDropTarget();
      }
      ImGui::PopStyleVar();
      
      if (ImGui::IsItemHovered())
      {
        if (hasMap)
        {
          ImGui::BeginTooltip();
          ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
          ImGui::TextUnformatted(map->GetfilePath().c_str());
          ImGui::PopTextWrapPos();
          UI::Image(UITexture, ImVec2(384, 384));
          ImGui::EndTooltip();
        }
        if (ImGui::IsItemClicked() and hasMap)
        {
          AssetEditorManager::OpenEditor(AssetManager::GetAsset<Asset>(map->handle));
        }
      }
      ImGui::SameLine();
      ImGui::BeginGroup();
      bool useFlag = static_cast<bool>(useMap);
      std::string useFlagStr = "Use##" + name;
      if (ImGui::Checkbox(useFlagStr.c_str(), &useFlag))
      {
        useMap = static_cast<float>(useFlag);
      }
      ImGui::EndGroup();
      
      if (name == "Albedo")
      {
        ImGui::SameLine();
        auto& albedoColor = material->Get<glm::vec3>("u_Material_AlbedoColor");
        ImGui::ColorEdit3("Color##Albedo", glm::value_ptr(albedoColor), ImGuiColorEditFlags_NoInputs);
      }
    }
    ImGui::PopID();
  }
  
  void MaterialViewer::Render()
  {
    if (!m_asset)
    {
      SetOpen(false);
    }
    
    float roundingVal = 15.0f;
    UI::ScopedStyle rounding (ImGuiStyleVar_FrameRounding, roundingVal);

    auto& material = m_asset->GetMaterial();
    std::string name = material->GetName();
    
    char buffer[256];
    memset(buffer, 0, 256);
    memcpy(buffer, name.c_str(), name.length());
    ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
    ImGui::PushItemWidth(contentRegionAvailable.x);
        
    if (ImGui::InputText("##Tag", buffer, 256))
    {
      name = std::string(buffer);
      material->SetName(name);
    }
    UI::DrawItemActivityOutline(roundingVal, false, Kreator_UI::Color::Accent);
    std::string shader = std::string("Shader: ") + material->GetShader()->GetName();
    UI::SetTooltip(shader.c_str());
    
    // Albedo
    Kreator_UI::BeginPropertyGrid();
    float& tilingFactor = material->Get<float>("u_TilingFactor");
    if (tilingFactor == 0)
    {
      tilingFactor = 1.0f;
    }
    
    Kreator_UI::Property("Metalness", material->Get<float>("u_Material_Metalness"), 0.01f, 0.0f, 1.0f);
    Kreator_UI::Property("Roughness", material->Get<float>("u_Material_Roughness"), 0.01f, 0.0f, 1.0f);
    Kreator_UI::Property("Tiling factor", tilingFactor, 1.0f, 1.0f, 1000.0f);
    Kreator_UI::EndPropertyGrid();
    
    ShowTextureProperty("Albedo", material);
    ShowTextureProperty("Normal", material);
    ShowTextureProperty("Metallic", material);
    ShowTextureProperty("Roughness", material);
  }
} // namespace Kreator
