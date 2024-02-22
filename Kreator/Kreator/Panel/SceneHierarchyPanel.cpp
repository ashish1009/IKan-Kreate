//
//  SceneHierarchyPanel.cpp
//  Kreator
//
//  Created by Ashish . on 22/02/24.
//

#include "SceneHierarchyPanel.hpp"
#include "Layer/KreatorLayer.hpp"

namespace Kreator
{
  static void DrawSearchBar(char* searchBuffer)
  {
    const float contentRegionAvail = ImGui::GetContentRegionAvail().x;
    UI::ShiftCursor(contentRegionAvail / 3, 5);
    ImGui::SetNextItemWidth(contentRegionAvail / 3);
    Kreator::UI::Widgets::SearchWidget(searchBuffer);
  }
  
  static void DrawTitleBar(char* searchBuffer)
  {
    static const float height = 30.0f;
    const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;
    
    // Draw the title Bar rectangle ---------------------------------------------------
    const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
    const ImVec2 titlebarMax =
    {
      ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth(),
      ImGui::GetCursorScreenPos().y + height
    };
    
    UI::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y));
    auto* drawList = ImGui::GetWindowDrawList();
    if (ImGui::IsWindowHovered())
    {
      drawList->AddRectFilled(titlebarMin, titlebarMax, UI::Color::Background);
    }
    else
    {
      drawList->AddRectFilled(titlebarMin, titlebarMax, UI::Color::Titlebar);
    }
    
    // Search box
    if (searchBuffer)
    {
      DrawSearchBar(searchBuffer);
    }
  }
  
  void SceneHierarchyPanel::Initialize()
  {
    IK_PROFILE();
    IK_LOG_TRACE("SceneHierarchyPanel", "Initializing the Scene Hierarchy Panel");
    s_pencilIcon = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/Pencil.png");
    s_plusIcon = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/Plus.png");
    s_gearIcon = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/Gear.png");
    s_reloadIcon = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/Rotate.png");
  }
  void SceneHierarchyPanel::Shutdown()
  {
    IK_PROFILE();
    s_pencilIcon.reset();
    s_plusIcon.reset();
    s_gearIcon.reset();
    s_reloadIcon.reset();
    IK_LOG_WARN("SceneHierarchyPanel", "Shutting down the Scene Hierarchy Panel");
  }

  SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
  : m_context(context)
  {
    IK_PROFILE();
    IK_LOG_INFO("Scene Hierarchy Panel", "Creating Scene Hierarchy Panel instance");
    if(m_context)
    {
      m_context->SetEntityDestroyedCallback([this](const Entity& entity) { OnExternalEntityDestroyed(entity); });
    }
  }
  
  void SceneHierarchyPanel::SetSceneContext(const Ref<Scene>& scene)
  {
    IK_PROFILE();
    m_context = scene;
    if(m_context)
    {
      m_context->SetEntityDestroyedCallback([this](const Entity& entity) { OnExternalEntityDestroyed(entity); });
    }
  }

  void SceneHierarchyPanel::OnProjectChanged(const Ref<Project>& project)
  {
    IK_PROFILE();
  }
  
  void SceneHierarchyPanel::OnImGuiRender(bool& isOpen)
  {
    IK_PERFORMANCE("SceneHierarchyPanel::OnImGuiRender");
    {
      UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
      ImGui::Begin("Scene Hierarchy", &isOpen);
    }
    
    if (m_context)
    {
      RenderHierarchy();
      ImGui::End();
      
      {
        UI::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(2.0, 4.0f));
        ImGui::Begin("Properties");
      }
      
      if (m_selectionContext.Size())
      {
        if (m_selectionContext.Size() == 1)
        {
          DrawComponents(m_selectionContext.At(0));
        }
        else if (m_lastSelectedEntity)
        {
          DrawComponents(m_lastSelectedEntity);
        }
        else
        {
          
        }
      }
    }
    
    {
      ImGui::End();
    }
  }
  
  void SceneHierarchyPanel::RenderHierarchy()
  {
    IK_PERFORMANCE("SceneHierarchyPanel::RenderHierarchy");
    
    static char searchedString[128];
    DrawTitleBar(searchedString);
    
    ImGui::Spacing();
  }
  
  void SceneHierarchyPanel::DrawComponents(Entity entity)
  {
    IK_PERFORMANCE("SceneHierarchyPanel::DrawComponents");
    
    static char searchedString[128];
    DrawTitleBar(searchedString);
  }
  
  void SceneHierarchyPanel::SetSelectionChangedCallback(const std::function<void(SelectionContext)>& func)
  {
    m_selectionChangedCallback = func;
  }
  void SceneHierarchyPanel::SetEntityDeletedCallback(const std::function<void(SelectionContext)>& func)
  {
    m_entityDeletedCallback = func;
  }
  void SceneHierarchyPanel::SetSelectedEntity(const Entity& entity, bool multipleSelection)
  {
    IK_PROFILE();
    if (!multipleSelection)
    {
      m_selectionContext.Clear();
    }
    
    RETURN_IF (!entity)
    
    m_selectionContext.PushBack(entity);
    if (m_selectionChangedCallback)
    {
      m_selectionChangedCallback(m_selectionContext);
    }
  }

  void SceneHierarchyPanel::OnExternalEntityDestroyed(const Entity& entity)
  {
    IK_PROFILE();
    if (m_selectionContext.Find(entity))
    {
      m_selectionContext.Erase(entity);
    }
  }
  void SceneHierarchyPanel::OnEntityDestroyed(Entity entity)
  {
    IK_PROFILE();
    m_entityDeletedCallback(m_selectionContext);
    
    if (m_selectionContext.Find(entity))
    {
      m_selectionContext.Erase(entity);
    }
    
    m_context->DestroyEntity(entity);
  }
} // namespace Kreator
