//
//  ContentBrowserPanel.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//


#include "ContentBrowserPanel.hpp"
#include "RendererLayer.hpp"
#include "Widget.hpp"
#include "ApplicationSettings.hpp"

namespace Kreator
{
  // Content Browser Panel -------------------------------------------------------------------------------------------
  namespace CBP_Utils
  {
    /// Get Full Content browser panel path
    /// - Parameter file_name: file name
    Ref<Image> AssetPath(const std::string& fileName)
    {
      return Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/CBP" / fileName);
    }
  } // namespace CBP_Utils
  
  ContentBrowserPanel::ContentBrowserPanel()
  {
    IK_LOG_TRACE("ContentBrowserPanel", "Initialising Content Browser Panel Textures");
  }
  
  void ContentBrowserPanel::OnImGuiRender(bool &isOpen)
  {
    ImGui::Begin("Content Browser", &isOpen, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
    ImGui::End();
  }
  void ContentBrowserPanel::OnProjectChanged(const Ref<Project> &project)
  {
  }
  
  void ContentBrowserPanel::OnEvent(Event& e)
  {
    
  }
} // namespace Kreator
