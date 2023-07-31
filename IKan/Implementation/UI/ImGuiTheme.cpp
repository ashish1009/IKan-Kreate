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
  void Theme::ChangeFont(const std::vector<UI::Font> otherFonts)
  {
    auto& imguiLayer = Application::Get().GetImGuiLayer();
    imguiLayer.OnDetach();
    
    imguiLayer.OnAttach();
    imguiLayer.ReloadIni();
    imguiLayer.SetFont(otherFonts);
  }

} // namespace IKan::UI
