//
//  AssetEdditor.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "AssetEdditor.hpp"

#include "Assets/AssetManager.hpp"

namespace IKan
{
  AssetEditor::AssetEditor(const char* title)
  : m_title(title), m_minSize(200, 400), m_maxSize(2000, 2000)
  {
  }
  
  void AssetEditor::OnImGuiRender()
  {
    IK_PERFORMANCE("AssetEditor::OnImGuiRender");
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
  
  void AssetEditorManager::UnregisterAllEditors()
  {
    s_editors.clear();
  }
  
  void AssetEditorManager::OnUpdate(TimeStep ts)
  {
    for (auto& kv : s_editors)
    {
      kv.second->OnUpdate(ts);
    }
  }
  
  void AssetEditorManager::OnEvent(Event& e)
  {
    for (auto& kv : s_editors)
    {
      kv.second->OnEvent(e);
    }
  }
  
  void AssetEditorManager::OnImGuiRender()
  {
    for (auto& kv : s_editors)
    {
      kv.second->OnImGuiRender();
    }
  }
  
  void AssetEditorManager::OpenEditor(const Ref<Asset>& asset)
  {
    if (s_editors.find(asset->GetAssetType()) == s_editors.end())
    {
      return;
    }
    
    s_editors[asset->GetAssetType()]->SetOpen(true);
    s_editors[asset->GetAssetType()]->SetAsset(AssetManager::GetAsset<Asset>(asset->handle));
  }
  
  void AssetEditorManager::CloseEditor(const Ref<Asset>& asset)
  {
    if (s_editors.find(asset->GetAssetType()) == s_editors.end())
    {
      return;
    }
    s_editors[asset->GetAssetType()]->SetOpen(false);
  }
  
  void AssetEditorManager::Clear()
  {
    s_editors.clear();
  }
  
  std::unordered_map<AssetType, Scope<AssetEditor>> AssetEditorManager::s_editors;
} // namespace IKan
