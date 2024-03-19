//
//  ImGuiTheme.cpp
//  Kreator
//
//  Created by Ashish . on 19/03/24.
//

#include "ImGuiTheme.hpp"

namespace Kreator::UI
{
  void LoadFonts(const std::vector<IKan::UI::ImGuiFont>& otherFonts)
  {
    IK_PROFILE();
    auto& imguiLayer = Application::Get().GetImGuiLayer();
    imguiLayer.OnDetach();
    
    imguiLayer.OnAttach();
    imguiLayer.LoadFonts(otherFonts);
  }
} // namespace Kreator::UI
