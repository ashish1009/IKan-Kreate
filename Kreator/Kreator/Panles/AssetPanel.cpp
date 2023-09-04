//
//  AssetPanel.cpp
//  Kreator
//
//  Created by Ashish . on 04/08/23.
//

#include "AssetPanel.hpp"

namespace Kreator
{
  void AssetPanel::OnImGuiRender(bool& isOpen)
  {
    if (!isOpen)
    {
      return;
    }
    
    UI::ScopedStyle frameRound(ImGuiStyleVar_FrameRounding, 20);
    ImGui::Begin("Asset Manager", &isOpen);
    if (UI::BeginTreeNode("Registry"))
    {
      static char searchBuffer[256];
      ImGui::SetNextItemWidth(-1);
      ImGui::InputTextWithHint("##regsearch", "Search ...", searchBuffer, 256);
      
      Kreator_UI::BeginPropertyGrid(2, 1);
      
      for (const auto& [path, metadata] : AssetManager::GetAssetRegistry())
      {
        std::string handle = fmt::format("{0}", (uint64_t)metadata.handle);
        std::string filepath = metadata.filePath.string();
        std::string type = AssetUtils::AssetTypeToString(metadata.type);
        if (searchBuffer[0] != 0)
        {
          std::string searchString = searchBuffer;
          Utils::String::ToLower(searchString);
          if (Utils::String::ToLowerCopy(handle).find(searchString) != std::string::npos
              or Utils::String::ToLowerCopy(filepath).find(searchString) != std::string::npos
              or Utils::String::ToLowerCopy(type).find(searchString) != std::string::npos)
          {
            UI::Separator();
            Kreator_UI::Property("Handle", (const std::string&)handle);
            Kreator_UI::Property("File Path", (const std::string&)filepath);
            Kreator_UI::Property("Type", (const std::string&)type);
          }
        }
        else
        {
          UI::Separator();
          Kreator_UI::Property("Handle", (const std::string&)fmt::format("{0}", (uint64_t)metadata.handle));
          Kreator_UI::Property("File Path", (const std::string&)metadata.filePath.string());
          Kreator_UI::Property("Type", (const std::string&)AssetUtils::AssetTypeToString(metadata.type));
        }
      }
      
      Kreator_UI::EndPropertyGrid();
      UI::EndTreeNode();
    }
    ImGui::End();
  }
} // namespace Kreator
