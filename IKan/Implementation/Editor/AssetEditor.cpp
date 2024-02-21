//
//  AssetEditor.cpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#include "AssetEditor.hpp"
#include "Asset/AssetManager.hpp"

namespace IKan
{
  AssetEditor::AssetEditor(const char* title)
  : m_title(title), m_minSize(200, 400), m_maxSize(2000, 2000)
  {
  }
  
  void AssetEditor::OnImGuiRender()
  {
    if (!m_isOpen)
    {
      return;
    }
    
    bool wasPpen = m_isOpen;
    ImGui::SetNextWindowSizeConstraints(m_minSize, m_maxSize);
    ImGui::Begin(m_title.c_str(), &m_isOpen, GetWindowFlags());
    Render();
    ImGui::End();
    
    if (wasPpen && !m_isOpen)
    {
      OnClose();
    }
  }
  
  void AssetEditor::SetOpen(bool isOpen)
  {
    m_isOpen = isOpen;
    if (!m_isOpen)
    {
      OnClose();
    }
  }
  
  void AssetEditor::SetMinSize(uint32_t width, uint32_t height)
  {
    if (width <= 0) width = 200;
    if (height <= 0) height = 400;
    
    m_minSize = ImVec2(float(width), float(height));
  }
  
  void AssetEditor::SetMaxSize(uint32_t width, uint32_t height)
  {
    if (width <= 0) width = 2000;
    if (height <= 0) height = 2000;
    if (float(width) <= m_minSize.x) width = (uint32_t)(m_minSize.x * 2.f);
    if (float(height) <= m_minSize.y) height = (uint32_t)(m_minSize.y * 2.f);
    
    m_maxSize = ImVec2(float(width), float(height));
  }
  
  void AssetEditor::SetTitle(const std::string& newTitle)
  {
    m_title = newTitle;
  }
  
  const std::string& AssetEditor::GetTitle() const
  {
    return m_title;
  }
  
  ImGuiWindowFlags AssetEditor::GetWindowFlags() const
  {
    return m_flags;
  }
} // namespace IKan
