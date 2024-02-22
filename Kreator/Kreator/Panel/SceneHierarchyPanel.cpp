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
    ImGui::Spacing();
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

    UI::ScopedStyle cellPadding(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 0.0f));

    // Alternate row Color
    static const ImU32 colRowAlt = UI::ColorWithMultipliedValue(UI::Color::BackgroundDark, 1.3f);
    UI::ScopedColor tableBGAlt(ImGuiCol_TableRowBgAlt, colRowAlt);

    // Scrollable Table uses child window internally
    UI::ScopedColor bgCol(ImGuiCol_ChildBg,  UI::ColorWithMultipliedValue(UI::Color::Titlebar, 0.8));
    static ImGuiTableFlags tableFlags = ImGuiTableFlags_NoPadInnerX
    | ImGuiTableFlags_Resizable
    | ImGuiTableFlags_Reorderable
    | ImGuiTableFlags_ScrollY
    | ImGuiTableFlags_RowBg
    | ImGuiTableFlags_Sortable;
    
    const int numColumns = 2;
    if (ImGui::BeginTable("##SceneHierarchy-Table", numColumns, tableFlags, ImVec2(ImGui::GetContentRegionAvail())))
    {
      ImGui::TableSetupColumn("Label");
      ImGui::TableSetupColumn("Visibility");

      // Headers
      {
        static constexpr float edgeOffset = 4.0f;
        static const ImU32 colActive = UI::ColorWithMultipliedValue(UI::Color::GroupHeader, 1.2f);
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
      } // Headers
      
      // List
      {
        // We draw selection and hover for table rows manually
        UI::ScopedColorStack entitySelection(ImGuiCol_Header, IM_COL32_DISABLE, ImGuiCol_HeaderHovered, IM_COL32_DISABLE, ImGuiCol_HeaderActive, IM_COL32_DISABLE);
        
        for (const auto& entity : m_context->GetRegistry().view<IDComponent, RelationshipComponent>())
        {
          Entity e(entity, m_context.get());
          if (e.GetParentUUID() == 0)
          {
            DrawEntityNode({ entity, m_context.get() }, searchedString);
          }
        }
      }
      
      // Right click option
      if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
      {
        DrawCreateEntityMenu({});
        ImGui::EndPopup();
      }

      ImGui::EndTable();
    } // Begin Table
  }
  
  void SceneHierarchyPanel::DrawComponents(Entity entity)
  {
    IK_PERFORMANCE("SceneHierarchyPanel::DrawComponents");
    
    static char searchedString[128];
    DrawTitleBar(searchedString);
  }
  
  void SceneHierarchyPanel::DrawEntityNode(Entity entity, const std::string &searchFilter)
  {
    
  }
  
  void SceneHierarchyPanel::DrawCreateEntityMenu(Entity parent)
  {
    Entity newEntity;
    if (ImGui::MenuItem("Empty Entity"))
    {
      newEntity = m_context->CreateEntity("Empty Entity");
    }
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
  
  bool SceneHierarchyPanel::SearchEntityRecursive(Entity entity, const std::string_view& searchFilter, const uint32_t maxSearchDepth, uint32_t currentDepth)
  {
    if (searchFilter.empty())
    {
      return false;
    }
    
    for (auto child : entity.Children())
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
} // namespace Kreator
