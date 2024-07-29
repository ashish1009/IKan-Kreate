//
//  ImGuiPopup.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

namespace Kreator::UI
{
  /// This class Begins the ImGui popup inside scope based on flag and size
  class Popup
  {
  public:
    /// This constructor creates the popup instance for scope
    /// - Parameters:
    ///   - title: title of popup
    ///   - openFlag: popup open flag as default
    ///   - width: width of popup
    ///   - height: height of popup
    ///   - center: is popup at center
    Popup(std::string_view title, bool openFlag = false, float width = 900 , float height = 450, bool center = true);
    
    /// This function shows the popup inside imGui loop
    /// - Parameters:
    ///   - popupFlags: popup flags
    ///   - func: function inside popup
    template<typename FuncT> void Show(ImGuiWindowFlags popupFlags, FuncT&& func)
    {
      if (m_openFlag)
      {
        ImGui::OpenPopup(m_title.data());
        m_openFlag = false;
      }

      // Keep the window at center
      if (m_center)
      {
        UI::SetNextWindowAtCenter();
      }
      UI::SetNextWindowSize({m_width, m_height});

      if (ImGui::BeginPopupModal(m_title.data(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | popupFlags))
      {
        UI::PushID();
        func();
        UI::PopID();
        
        ImGui::EndPopup();
      } 
    }
    
  private:
    std::string m_title {""};
    bool m_openFlag {false};
    bool m_center {true};
    float m_width, m_height;
  };
} // namespace Kreator::UI
