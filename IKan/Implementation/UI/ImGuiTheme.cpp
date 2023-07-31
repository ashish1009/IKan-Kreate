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
} // namespace IKan::UI
