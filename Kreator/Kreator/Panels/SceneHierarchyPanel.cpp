//
//  SceneHierarchyPanel.cpp
//  Kreator
//
//  Created by Ashish . on 02/08/24.
//

#include "SceneHierarchyPanel.hpp"

#include "Layers/RendererLayer.hpp"
#include "Editor/EntityUtils.hpp"

namespace Kreator
{
  static void DrawSearchBar(char* searchBuffer)
  {
    const float contentRegionAvail = ImGui::GetContentRegionAvail().x;
    UI::ShiftCursor(contentRegionAvail / 3, 5);
    ImGui::SetNextItemWidth(contentRegionAvail / 3);
    Kreator::UI::Widgets::SearchWidget(searchBuffer);
    ImGui::Spacing();
  }

  void SceneHierarchyPanel::Initialize()
  {
    IK_PROFILE();
    IK_LOG_TRACE("SceneHierarchyPanel", "Initializing the Scene Hierarchy Panel");
    s_pencilIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Pencil.png");
    s_plusIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Plus.png");
    s_gearIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Gear.png");
    s_reloadIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Rotate.png");
    s_closeEyeIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/CloseEye.png");
    s_EyeIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Eye.png");
  }
  void SceneHierarchyPanel::Shutdown()
  {
    IK_PROFILE();
    s_pencilIcon.reset();
    s_plusIcon.reset();
    s_gearIcon.reset();
    s_reloadIcon.reset();
    IK_LOG_INFO("SceneHierarchyPanel", "Shutting down the Scene Hierarchy Panel");
  }
  
  SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context, bool isWindow)
  : m_context(context), m_isWindow(isWindow)
  {
    IK_PROFILE();
    IK_LOG_INFO("Scene Hierarchy Panel", "Creating Scene Hierarchy Panel instance");
  }
  
  void SceneHierarchyPanel::SetSceneContext(const Ref<Scene>& scene)
  {
    IK_PROFILE();
    m_context = scene;
  }
  
  void SceneHierarchyPanel::OnProjectChanged([[maybe_unused]] const Ref<Project>& project)
  {
    IK_PROFILE();
  }
  
  void SceneHierarchyPanel::OnImGuiRender(bool& isOpen)
  {
    IK_PERFORMANCE("SceneHierarchyPanel::OnImGuiRender");
    if (m_isWindow)
    {
      UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
      ImGui::Begin("Scene Hierarchy", &isOpen);
    }

    // Draw scene hierarchy panel
    if (m_context)
    {
      RenderHierarchy();

      ImGui::End();

      // Draw property panel
      ImGui::Begin("Properties");
    }
    
    if (m_isWindow)
    {
      ImGui::End();
    }
  }
  
  void SceneHierarchyPanel::RenderHierarchy()
  {
    IK_PERFORMANCE("SceneHierarchyPanel::RenderHierarchy");

    static const float edgeOffset = 4.0f;
    UI::ShiftCursorX(edgeOffset * 3.0f);
    UI::ShiftCursorY(edgeOffset * 2.0f);

    static char searchedString[128];
    DrawSearchBar(searchedString);
    
    // Entity list ------------------------------------------------------------------------------------------------
    UI::ScopedStyle cellPadding(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 0.0f));

    // Alternate row Color
    const ImU32 colRowAlt = UI::ColorWithMultipliedValue(UI::Color::BackgroundDark, 1.3f);
    UI::ScopedColor tableBGAlt(ImGuiCol_TableRowBgAlt, colRowAlt);

    // Table
    {
      // Scrollable Table uses child window internally
      UI::ScopedColor bgCol(ImGuiCol_ChildBg,  UI::ColorWithMultipliedValue(UI::Color::TitleBar, 0.8));
      ImGuiTableFlags tableFlags = ImGuiTableFlags_NoPadInnerX
      | ImGuiTableFlags_Resizable
      | ImGuiTableFlags_Reorderable
      | ImGuiTableFlags_ScrollY
      | ImGuiTableFlags_RowBg
      | ImGuiTableFlags_Sortable;
      
      const int numColumns = 3;
      if (ImGui::BeginTable("##SceneHierarchy-Table", numColumns, tableFlags, ImVec2(ImGui::GetContentRegionAvail())))
      {
        ImGui::TableSetupColumn("Label");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Visibility");

        // Headers
        {
          const ImU32 colActive = UI::ColorWithMultipliedValue(UI::Color::GroupHeader, 1.2f);
          UI::ScopedColorStack headerColor(ImGuiCol_HeaderHovered, colActive, ImGuiCol_HeaderActive, colActive);
          
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
          UI::ScopedColorStack entitySelection(ImGuiCol_Header, IM_COL32_DISABLE, ImGuiCol_HeaderHovered, IM_COL32_DISABLE, ImGuiCol_HeaderActive, IM_COL32_DISABLE);
          
          for (auto entity : m_context->GetRegistry().view<IDComponent, RelationshipComponent>())
          {
            Entity e(entity, m_context.get());
            
            // Render those entity who do not have parent
            if (e.GetParentUUID() == 0)
            {
              DrawEntityNode({ entity, m_context.get() }, searchedString);
            }
          }
        }

        // Right click option
        if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
        {
          Entity newEntity = ECS_Utils::DrawCreateEntityMenu(m_context, {});
          
          ImGui::EndPopup();
        }

        ImGui::EndTable();
      } // Begin table
    }  // Table Scope
  }
  
  void SceneHierarchyPanel::DrawEntityNode(Entity entity, const std::string &searchFilter)
  {
    static const float edgeOffset = 4.0f;
    const char* name = entity.GetComponent<TagComponent>().tag.c_str();
    constexpr uint32_t maxSearchDepth = 20;
    
    // Search entity with filter string
    bool hasChildMatchingSearch = SearchEntityRecursive(entity, searchFilter, maxSearchDepth);
    if (!Kreator::UI::IsMatchingSearch(name, searchFilter) and !hasChildMatchingSearch)
    {
      return;
    }

  }
  
  bool SceneHierarchyPanel::SearchEntityRecursive(Entity entity, const std::string_view &searchFilter, const uint32_t maxSearchDepth, uint32_t currentDepth)
  {
    // Return if search string is empty
    if (searchFilter.empty())
    {
      return false;
    }
    
    // Return if reached max depth
    if (currentDepth > maxSearchDepth)
    {
      return false;
    }
    
    // For each child
    for (const auto& child : entity.Children())
    {
      Entity e = m_context->GetEntityWithUUID(child);
      if (e.HasComponent<TagComponent>())
      {
        if (Kreator::UI::IsMatchingSearch(e.GetComponent<TagComponent>().tag, searchFilter))
        {
          return true;
        }
      }

      bool found = SearchEntityRecursive(e, searchFilter, maxSearchDepth, currentDepth + 1);
      if (found)
      {
        return true;
      }
    }
    return false;
  }
} //  namesapce Kreator
