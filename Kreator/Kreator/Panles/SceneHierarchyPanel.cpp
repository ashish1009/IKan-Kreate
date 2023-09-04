//
//  SceneHierarchyPanel.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#include "SceneHierarchyPanel.hpp"
#include "RendererLayer.hpp"

namespace Kreator
{
  void SceneHierarchyPanel::Initialize()
  {
    s_pencilIcon = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Pencil.png");
    s_plusIcon = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Plus.png");
    s_gearIcon = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Gear.png");
  }
  void SceneHierarchyPanel::Shutdown()
  {
    s_pencilIcon.reset();
    s_plusIcon.reset();
    s_gearIcon.reset();
  }

  SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context, bool isWindow)
  : m_context(context), m_isWindow(isWindow)
  {

  }
  
  void SceneHierarchyPanel::SetSceneContext(const Ref<Scene>& scene)
  {
    m_context = scene;
  }
  
  void SceneHierarchyPanel::OnImGuiRender(bool& isOpen)
  {
    if (m_isWindow)
    {
      UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
      ImGui::Begin("Scene Hierarchy", &isOpen);
    }
    
    if (m_context)
    {
      ImRect windowRect = { ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax() };
      RenderHierarchy();
      ImGui::End();
      
      {
        UI::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(2.0, 4.0f));
        ImGui::Begin("Properties");
      }
      
      if (m_selectionContext)
      {
        DrawComponents(m_selectionContext);
      }
    }
    
    if (m_isWindow)
    {
      ImGui::End();
    }
  }
  
  void SceneHierarchyPanel::RenderHierarchy()
  {
    const float edgeOffset = 4.0f;
    UI::ShiftCursorX(edgeOffset * 3.0f);
    UI::ShiftCursorY(edgeOffset * 2.0f);
    
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - edgeOffset * 3.0f);
    
    static std::string searchedString;
    Kreator_UI::Widgets::SearchWidget(searchedString.data());
    
    ImGui::Spacing();

    // Entity list
    //------------
    UI::ScopedStyle cellPadding(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 0.0f));
    
    // Alt row Color
    const ImU32 colRowAlt = UI::ColorWithMultipliedValue(UI::Theme::Color::BackgroundDark, 1.3f);
    UI::ScopedColor tableBGAlt(ImGuiCol_TableRowBgAlt, colRowAlt);

    // Table
    {
      // Scrollable Table uses child window internally
      UI::ScopedColor bgCol(ImGuiCol_ChildBg,  UI::ColorWithMultipliedValue(UI::Theme::Color::Titlebar, 0.8));
      ImGuiTableFlags tableFlags = ImGuiTableFlags_NoPadInnerX
      | ImGuiTableFlags_Resizable
      | ImGuiTableFlags_Reorderable
      | ImGuiTableFlags_ScrollY
      | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable;
      
      const int numColumns = 3;
      if (ImGui::BeginTable("##SceneHierarchy-Table", numColumns, tableFlags, ImVec2(ImGui::GetContentRegionAvail())))
      {
        ImGui::TableSetupColumn("Label");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Visibility");
        
        // Headers
        {
          const ImU32 colActive = UI::ColorWithMultipliedValue(UI::Theme::Color::GroupHeader, 1.2f);
          UI::ScopedColorStack headerColor(ImGuiCol_HeaderHovered, colActive,
                                           ImGuiCol_HeaderActive, colActive);
          
          ImGui::TableSetupScrollFreeze(ImGui::TableGetColumnCount(), 1);
          
          ImGui::TableNextRow(ImGuiTableRowFlags_Headers, 22.0f);
          for (int column = 0; column < ImGui::TableGetColumnCount(); column++)
          {
            ImGui::TableSetColumnIndex(column);
            const char* columnName = ImGui::TableGetColumnName(column);
            UI::ScopedID columnID(column);
            
            UI::ShiftCursor(edgeOffset * 3.0f, edgeOffset * 2.0f);
            ImGui::TableHeader(columnName);
            UI::ShiftCursor(-edgeOffset * 3.0f, -edgeOffset * 2.0f);
          }
          ImGui::SetCursorPosX(ImGui::GetCurrentTable()->OuterRect.Min.x);
          UI::DrawUnderline(true, 0.0f, 5.0f);
        }
        
        // List
        {
          // We draw selection and hover for table rows manually
          UI::ScopedColorStack entitySelection(ImGuiCol_Header, IM_COL32_DISABLE,
                                               ImGuiCol_HeaderHovered, IM_COL32_DISABLE,
                                               ImGuiCol_HeaderActive, IM_COL32_DISABLE);
          
          for (auto entity : m_context->GetRegistry().view<IDComponent, RelationshipComponent>())
          {
            Entity e(entity, m_context.get());
            if (e.GetParentUUID() == 0)
            {
              DrawEntityNode({ entity, m_context.get() }, searchedString);
            }
          }
        }
        
        if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
        {
          DrawCreateEntityMenu({});
          ImGui::EndPopup();
        }
        
        ImGui::EndTable();
      }
    }
  }
  
  void SceneHierarchyPanel::DrawComponents(Entity entity)
  {
    
  }
  
  void SceneHierarchyPanel::DrawEntityNode(Entity entity, const std::string& searchFilter)
  {
  }
  
  void SceneHierarchyPanel::DrawCreateEntityMenu(Entity parent)
  {
  }
} // namespace Kreator
