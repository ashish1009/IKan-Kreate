//
//  ImGuiTheme.cpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#include "ImGuiTheme.hpp"
#include "Core/Application.hpp"

namespace IKan::UI
{
  void Theme::ChangeFont(const UI::Font &defaultFont, const UI::Font &boldFont, const std::vector<UI::Font> otherFonts)
  {
    auto& imguiLayer = Application::Get().GetImGuiLayer();
    imguiLayer.OnDetach();
    
    imguiLayer.OnAttach();
    imguiLayer.ReloadIni();
    imguiLayer.SetFont(defaultFont, boldFont, otherFonts);
  }
  
  ImFont* Theme::GetRegularFont()
  {
    return ImGui::GetIO().Fonts->Fonts[0];
  }
  ImFont* Theme::GetBoldFont()
  {
    return ImGui::GetIO().Fonts->Fonts.Size > 1 ? ImGui::GetIO().Fonts->Fonts[1] : ImGui::GetIO().Fonts->Fonts[0];
  }

} // namespace IKan::UI
