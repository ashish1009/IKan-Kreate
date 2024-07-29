//
//  ImGuiProperty.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

namespace Kreator::UI
{
  enum class PropertyAssetReferenceError
  {
    None = 0, InvalidMetadata
  };
  
  static AssetHandle s_propertyAssetReferenceAssetHandle;
  
  struct PropertyAssetReferenceSettings
  {
    bool advanceToNextColumn = true;
    bool noItemSpacing = false; // After label
    float widthOffset = 0.0f;
    bool allowMemoryOnlyAssets = false;
    ImVec4 buttonLabelColor = ImGui::ColorConvertU32ToFloat4(UI::Color::Text);
    ImVec4 buttonLabelColorError = ImGui::ColorConvertU32ToFloat4(UI::Color::TextError);
    bool showFullFilePath = false;
  };
  
  /// This function render property header
  /// - Parameter name: header name
  bool PropertyGridHeader(const std::string& name, bool openByDefault = true, float height = 6, float rounding = 8);
  /// This function Ends the tree node
  void PropertyGridHeaderEnd();
  /// This function starts the property header
  void BeginPropertyGrid(uint32_t columns = 2, float height = 2, float spacing = 5);
  /// This function ends the property header
  void EndPropertyGrid();
  
  /// Tooltip as help marker
  void HelpMarker(const char* desc);
  
  // Proeprty --------------------------------------------------------------------------------------------------------
  bool Property(const char* label, std::string& value);
  bool PropertyMultiline(const char* label, std::string& value);
  void Property(const char* label, const std::string& value);
  void Property(const char* label, const char* value);
  bool Property(const char* label, char* value, size_t length);
  bool Property(const char* label, bool& value, const char* helpText = "");
  bool Property(const char* label, int8_t& value, int8_t min = 0, int8_t max = 0);
  bool Property(const char* label, int16_t& value, int16_t min = 0, int16_t max = 0);
  bool Property(const char* label, int32_t& value, int32_t min = 0, int32_t max = 0);
  bool Property(const char* label, int64_t& value, int64_t min = 0, int64_t max = 0);
  bool Property(const char* label, uint8_t& value, uint8_t minValue = 0, uint8_t maxValue = 0);
  bool Property(const char* label, uint16_t& value, uint16_t minValue = 0, uint16_t maxValue = 0);
  bool Property(const char* label, uint32_t& value, uint32_t minValue = 0, uint32_t maxValue = 0);
  bool Property(const char* label, uint64_t& value, uint64_t minValue = 0, uint64_t maxValue = 0);
  bool Property(const char* label, float& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f, const char* helpText = "");
  bool Property(const char* label, glm::vec2& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f);
  bool Property(const char* label, glm::vec3& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f);
  bool Property(const char* label, glm::vec4& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f);
  bool PropertySlider(const char* label, int& value, int min = -INT_MAX, int max = INT_MAX);
  bool PropertySlider(const char* label, float& value, float min = -FLT_MAX, float max = FLT_MAX);
  bool PropertySlider(const char* label, glm::vec2& value, float min = -FLT_MAX, float max = FLT_MAX);
  bool PropertySlider(const char* label, glm::vec3& value, float min = -FLT_MAX, float max = FLT_MAX);
  bool PropertySlider(const char* label, glm::vec4& value, float min = -FLT_MAX, float max = FLT_MAX);
  bool PropertyInput(const char* label, int8_t& value, int8_t step = 1, int8_t stepFast = 1);
  bool PropertyInput(const char* label, int16_t& value, int16_t step = 1, int16_t stepFast = 1);
  bool PropertyInput(const char* label, int32_t& value, int32_t step = 1, int32_t stepFast = 1);
  bool PropertyInput(const char* label, int64_t& value, int64_t step = 1, int64_t stepFast = 1);
  bool PropertyInput(const char* label, uint8_t& value, uint8_t step = 1, uint8_t stepFast = 1);
  bool PropertyInput(const char* label, uint16_t& value, uint16_t step = 1, uint16_t stepFast = 1);
  bool PropertyInput(const char* label, uint32_t& value, uint32_t step = 1, uint32_t stepFast = 1);
  bool PropertyInput(const char* label, uint64_t& value, uint64_t step = 1, uint64_t stepFast = 1);
  bool PropertyColor(const char* label, glm::vec3& value);
  bool PropertyColor(const char* label, glm::vec4& value);
  bool PropertyDropdown(const char* label, const char** options, int32_t optionCount, int32_t* selected);
  bool PropertyDropdownNoLabel(const char* strID, const char** options, int32_t optionCount, int32_t* selected);
  bool PropertyDropdown(const char* label, const std::vector<std::string>& options, int32_t optionCount, int32_t* selected);
  bool PropertyDropdownNoLabel(const char* strID, const std::vector<std::string>& options, int32_t optionCount, int32_t* selected);
    
  template<typename T>
  static bool PropertyAssetReference(const char* label, AssetHandle& outHandle, PropertyAssetReferenceError* outError = nullptr,
                                     const PropertyAssetReferenceSettings& settings = {})
  {
    bool modified = false;
    if (outError)
    {
      *outError = PropertyAssetReferenceError::None;
    }
    
    UI::ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    UI::ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    ImVec2 originalButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
    {
      ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
      float width = ImGui::GetContentRegionAvail().x - settings.widthOffset;
      float itemHeight = 18.0f;
      
      std::string buttonText = "Null";
      bool valid = true;
      if (AssetManager::IsAssetHandleValid(outHandle))
      {
        auto object = AssetManager::GetAsset<T>(outHandle);
        valid = object and !object->IsFlagSet(AssetFlag::Invalid) and !object->IsFlagSet(AssetFlag::Missing);
        
        if (object and !object->IsFlagSet(AssetFlag::Missing))
        {
          if (settings.showFullFilePath)
          {
            buttonText = AssetManager::GetMetadata(outHandle).filePath.string();
          }
          else
          {
            buttonText = AssetManager::GetMetadata(outHandle).filePath.stem().string();
          }
        }
        else
        {
          buttonText = "Missing";
        }
      }
      
      // PropertyAssetReference could be called multiple times in same "context"
      // and so we need a unique id for the asset search popup each time.
      // notes
      // - don't use GenerateID(), that's inviting id clashes, which would be super confusing.
      // - don't store return from GenerateLabelId in a const char* here. Because its pointing to an internal
      //   buffer which may get overwritten by the time you want to use it later on.
      std::string assetSearchPopupID = UI::GenerateLabelID("ARSP");
      {
        UI::ScopedColor buttonLabelColor(ImGuiCol_Text, valid? settings.buttonLabelColor : settings.buttonLabelColorError);
        if (ImGui::Button(UI::GenerateLabelID(buttonText), { width, itemHeight }))
        {
          ImGui::OpenPopup(assetSearchPopupID.c_str());
        }
      }
      ImGui::GetStyle().ButtonTextAlign = originalButtonTextAlign;
      
      bool clear = false;
      if (Widgets::AssetSearchPopup(assetSearchPopupID.c_str(), T::GetStaticType(), outHandle, settings.allowMemoryOnlyAssets, &clear))
      {
        if (clear)
        {
          outHandle = 0;
        }
        
        modified = true;
        s_propertyAssetReferenceAssetHandle = outHandle;
      }
    }
    
    if (!UI::IsItemDisabled())
    {
      // Drop Content
      if (ImGui::BeginDragDropTarget())
      {
        auto data = ImGui::AcceptDragDropPayload("asset_payload");
        
        if (data)
        {
          AssetHandle assetHandle = *(AssetHandle*)data->Data;
          s_propertyAssetReferenceAssetHandle = assetHandle;
          Ref<Asset> asset = AssetManager::GetAsset<Asset>(assetHandle);
          if (asset and asset->GetAssetType() == T::GetStaticType())
          {
            outHandle = assetHandle;
            modified = true;
          }
        }
      }
      
      // Draw Outline
      UI::DrawItemActivityOutline(2.0f, true, IM_COL32(256, 18, 36, 255));
    }
    
    ImGui::PopItemWidth();
    
    if (settings.advanceToNextColumn)
    {
      ImGui::NextColumn();
      UI::ShiftCursorY(4.0f);
      ImGui::PushItemWidth(-1);
    }
    
    return modified;
  }
  
  template<typename T, typename Fn>
  static bool PropertyAssetReferenceTarget(const char* label, const char* assetName, AssetHandle& outHandle, Fn&& targetFunc, const PropertyAssetReferenceSettings& settings = {})
  {
    bool modified = false;
    
    ShiftCursor(10.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    if (settings.noItemSpacing)
    {
      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });
    }
    
    ImVec2 originalButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
    ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
    float width = ImGui::GetContentRegionAvail().x - settings.widthOffset;
    UI::PushID();
    
    float itemHeight = 28.0f;
    
    std::string buttonText = "Null";
    bool valid = true;
    
    if (AssetManager::IsAssetHandleValid(outHandle))
    {
      auto source = AssetManager::GetAsset<T>(outHandle);
      valid = source && !source->IsFlagSet(AssetFlag::Invalid) && !source->IsFlagSet(AssetFlag::Missing);
      if (source && !source->IsFlagSet(AssetFlag::Missing))
      {
        if (assetName)
        {
          buttonText = assetName;
        }
        else
        {
          buttonText = AssetManager::GetMetadata(outHandle).filePath.stem().string();
          assetName = buttonText.c_str();
        }
      }
      else
      {
        buttonText = "Missing";
      }
    }
    
    // PropertyAssetReferenceTarget could be called multiple times in same "context"
    // and so we need a unique id for the asset search popup each time.
    // notes
    // - don't use GenerateID(), that's inviting id clashes, which would be super confusing.
    // - don't store return from GenerateLabelId in a const char* here. Because its pointing to an internal
    //   buffer which may get overwritten by the time you want to use it later on.
    std::string assetSearchPopupID = GenerateLabelID("ARTSP");
    {
      UI::ScopedColor buttonLabelColor(ImGuiCol_Text, valid ? settings.buttonLabelColor : settings.buttonLabelColorError);
      if (ImGui::Button(GenerateLabelID(buttonText), { width, itemHeight }))
      {
        ImGui::OpenPopup(assetSearchPopupID.c_str());
      }
    }
    
    ImGui::GetStyle().ButtonTextAlign = originalButtonTextAlign;
    
    bool clear = false;
    if (Widgets::AssetSearchPopup(assetSearchPopupID.c_str(), T::GetStaticType(), outHandle, settings.allowMemoryOnlyAssets, &clear))
    {
      if (clear)
        outHandle = 0;
      
      targetFunc(AssetManager::GetAsset<T>(outHandle));
      modified = true;
    }
    
    UI::PopID();
    
    if (!IsItemDisabled())
    {
      if (ImGui::BeginDragDropTarget())
      {
        auto data = ImGui::AcceptDragDropPayload("asset_payload");
        
        if (data)
        {
          AssetHandle assetHandle = *(AssetHandle*)data->Data;
          s_propertyAssetReferenceAssetHandle = assetHandle;
          Ref<Asset> asset = AssetManager::GetAsset<Asset>(assetHandle);
          if (asset && asset->GetAssetType() == T::GetStaticType())
          {
            targetFunc(std::dynamic_pointer_cast<T>(asset));
            modified = true;
          }
        }
      }
      
      DrawItemActivityOutline(2.0f, true, UI::Color::Accent);
    }
    
    ImGui::PopItemWidth();
    if (settings.advanceToNextColumn)
    {
      ImGui::NextColumn();
    }
    if (settings.noItemSpacing)
    {
      ImGui::PopStyleVar();
    }
    return modified;
  }
} // namespace Kreator::UI
