//
//  AssetPanel.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "AssetPanel.hpp"

namespace Kreator
{
  void AssetPanel::OnImGuiRender(bool& isOpen)
  {
    IK_PERFORMANCE("ProjectSettingsPanel::OnImGuiRender");
    if (!isOpen)
    {
      return;
    }
    
    UI::ScopedStyle frameRound(ImGuiStyleVar_FrameRounding, 5);
    
    ImGui::Begin("Asset Manager", &isOpen);
    if (UI::PropertyGridHeader("Registry", true, 4))
    {
      static char searchBuffer[256];
      ImGui::SetNextItemWidth(-1);
      ImGui::InputTextWithHint("##regsearch", "Search ...", searchBuffer, 256);
      
      UI::BeginPropertyGrid(2, 1, 5);
      
      for (const auto& [path, metadata] : AssetManager::GetAssetRegistry())
      {
        std::string handle = fmt::format("{0}", (uint64_t)metadata.handle);
        std::string filepath = metadata.filePath.string();
        std::string type = AssetUtils::AssetTypeToString(metadata.type);
        if (searchBuffer[0] != 0)
        {
          std::string searchString = Utils::String::ToLower(searchBuffer);
          if (Utils::String::ToLowerCopy(handle).find(searchString) != std::string::npos
              or Utils::String::ToLowerCopy(filepath).find(searchString) != std::string::npos
              or Utils::String::ToLowerCopy(type).find(searchString) != std::string::npos)
          {
            ImGui::Separator();
            UI::Property("Handle", (const std::string&)handle);
            UI::Property("File Path", (const std::string&)filepath);
            UI::Property("Type", (const std::string&)type);
          }
        }
        else
        {
          ImGui::Separator();
          UI::Property("Handle", (const std::string&)fmt::format("{0}", (uint64_t)metadata.handle));
          UI::Property("File Path", (const std::string&)metadata.filePath.string());
          UI::Property("Type", (const std::string&)AssetUtils::AssetTypeToString(metadata.type));
        }
      }
      
      UI::EndPropertyGrid();
      UI::PropertyGridHeaderEnd();
    }
    ImGui::End();
  }
} // namespace Kreator
