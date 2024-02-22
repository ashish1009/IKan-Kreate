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
      
      ImRect windowRect = { ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax() };
      if (ImGui::BeginDragDropTargetCustom(UI::RectOffset(windowRect, ImGui::GetWindowPos()) , ImGui::GetCurrentWindow()->ID))
      {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
        
        if (payload)
        {
          Entity& entity = *(Entity*)payload->Data;
          m_context->UnparentEntity(entity);
        }
        
        ImGui::EndDragDropTarget();
      }
      
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
      UI::ScopedColor bgCol(ImGuiCol_ChildBg,  UI::ColorWithMultipliedValue(UI::Color::Titlebar, 0.8));
      ImGuiTableFlags tableFlags = ImGuiTableFlags_NoPadInnerX
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
      } // Begin table
    }  // Table Scope
  }
  
  void SceneHierarchyPanel::DrawComponents(Entity entity)
  {
    IK_PERFORMANCE("SceneHierarchyPanel::DrawComponents");
    
    static char searchedString[128];
    DrawTitleBar(searchedString);
  }
  
  void SceneHierarchyPanel::DrawEntityNode(Entity entity, const std::string &searchFilter)
  {
    const char* name = entity.GetComponent<TagComponent>().tag.c_str();
    constexpr uint32_t maxSearchDepth = 10;
    bool hasChildMatchingSearch = SearchEntityRecursive(entity, searchFilter, maxSearchDepth);
    
    RETURN_IF(!Kreator::UI::IsMatchingSearch(name, searchFilter) and !hasChildMatchingSearch)

    const float rowHeight = 21.0f;
    
    // ImGui item height tweaks
    auto* window = ImGui::GetCurrentWindow();
    window->DC.CurrLineSize.y = rowHeight;
    ImGui::TableNextRow(0, rowHeight);
    
    // Label column -------------------------------------------------------------------------------------------------
    ImGui::TableNextColumn();
    window->DC.CurrLineTextBaseOffset = 3.0f;
    
    const ImVec2 rowAreaMin = ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), 0).Min;
    const ImVec2 rowAreaMax =
    {
      ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), ImGui::TableGetColumnCount() - 1).Max.x,
      rowAreaMin.y + rowHeight
    };

    // Setup flags ---------------
    const bool isSelected = m_selectionContext.Find(entity);
    ImGuiTreeNodeFlags flags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow| ImGuiTreeNodeFlags_SpanAvailWidth;
    if (hasChildMatchingSearch)
    {
      flags |= ImGuiTreeNodeFlags_DefaultOpen;
    }
    if (entity.Children().empty())
    {
      flags |= ImGuiTreeNodeFlags_Leaf;
    }
    if (isSelected)
    {
      if (entity.Children().size() > 0)
      {
        flags |= ImGuiTreeNodeFlags_DefaultOpen;
      }
    }
    if (IsChildSelected(entity))
    {
      flags |= ImGuiTreeNodeFlags_DefaultOpen;
    }
    
    const std::string strID = std::string(name) + std::to_string((uint32_t)entity);
    ImGui::PushClipRect(rowAreaMin, rowAreaMax, false);
    bool isRowHovered, held;
    bool isRowClicked = ImGui::ButtonBehavior(ImRect(rowAreaMin, rowAreaMax), ImGui::GetID(strID.c_str()), &isRowHovered, &held, ImGuiButtonFlags_AllowItemOverlap);
    ImGui::SetItemAllowOverlap();
    ImGui::PopClipRect();

    const bool isWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

    // Row Coloring ------------------
    // Fill with light selection Color if any of the child entities selected
    auto isAnyDescendantSelected = [&](Entity ent, auto isAnyDescendantSelected) -> bool
    {
      if (m_selectionContext.Find(ent))
      {
        return true;
      }
      
      if (!ent.Children().empty())
      {
        for (auto& childEntityID : ent.Children())
        {
          Entity childEntity = m_context->GetEntityWithUUID(childEntityID);
          if (isAnyDescendantSelected(childEntity, isAnyDescendantSelected))
          {
            return true;
          }
        }
      }
      return false;
    };
    
    auto fillRowWithColor = [] (const ImColor& Color)
    {
      for (int column = 0; column < ImGui::TableGetColumnCount(); column++)
      {
        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, Color, column);
      }
    };

    if (isSelected)
    {
      if (isWindowFocused || UI::NavigatedTo())
      {
        fillRowWithColor(UI::Color::Selection);
      }
      else
      {
        const ImColor col = UI::ColorWithMultipliedValue(UI::Color::Selection, 0.9f);
        fillRowWithColor(UI::ColorWithMultipliedSaturation(col, 0.7f));
      }
    }
    else if (isRowHovered)
    {
      fillRowWithColor(UI::Color::BackgroundDark);
    }
    else if (isAnyDescendantSelected(entity, isAnyDescendantSelected))
    {
      fillRowWithColor(UI::Color::SelectionMuted);
    }

    // Text Coloring ------------------
    if (isSelected)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, UI::Color::BackgroundDark);
    }

    // Tree node ----------------------
    ImGuiContext& g = *GImGui;
    auto& style = ImGui::GetStyle();
    
    const ImVec2 padding = ((flags & ImGuiTreeNodeFlags_FramePadding)) ?
    style.FramePadding :
    ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));
    
    // Collapser arrow width + Spacing
    const float textOffsetX = g.FontSize + padding.x * 2;
    
    // Latch before ItemSize changes it
    const float textOffsetY = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);
    
    ImVec2 textPos(window->DC.CursorPos.x + textOffsetX, window->DC.CursorPos.y + textOffsetY);
    
    const float arrowHitX1 = (textPos.x - textOffsetX) - style.TouchExtraPadding.x;
    const float arrowHitX2 = (textPos.x - textOffsetX) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
    const bool isMouseXOverSrrow = (g.IO.MousePos.x >= arrowHitX1 && g.IO.MousePos.x < arrowHitX2);
    
    bool previousState = ImGui::TreeNodeBehaviorIsOpen(ImGui::GetID(strID.c_str()));
    if (isMouseXOverSrrow and isRowClicked)
    {
      ImGui::SetNextItemOpen(!previousState);
    }
    
    const bool opened = UI::TreeNodeWithIcon(nullptr, ImGui::GetID(strID.c_str()), flags, name, nullptr);
    bool entityDeleted = false;
    
    if (ImGui::BeginPopupContextItem())
    {
      {
        UI::ScopedColor colText(ImGuiCol_Text, UI::Color::Text);
        UI::ScopedColorStack entitySelection(ImGuiCol_Header, UI::Color::GroupHeader, ImGuiCol_HeaderHovered, UI::Color::GroupHeader, ImGuiCol_HeaderActive, UI::Color::GroupHeader);
        
        // Select current entity
        SetSelectedEntity(entity);
        
        // Empty Space Right click menu
        DrawCreateEntityMenu(entity);
        
        // Selected Entity Right click
        if (m_selectionContext.Size() == 1)
        {
          ImGui::Separator();
          if (m_selectionContext.At(0).GetParent())
          {
            if (ImGui::MenuItem("Unparent"))
            {
              m_context->UnparentEntity(m_selectionContext.At(0));
              ImGui::Separator();
            }
          }
          
          if (ImGui::MenuItem("Delete"))
          {
            entityDeleted = true;
          }
          if (ImGui::MenuItem("Duplicate"))
          {
            SetSelectedEntity(m_context->DuplicateEntity(m_selectionContext.At(0)));
          }
        }
      }
      ImGui::EndPopup();
    }

    // <> column 2 -------------------------------------------------------------------------------------------------
    ImGui::TableNextColumn();
    
    if (isRowClicked)
    {
      bool multipleSelection = Input::IsKeyPressed(IKan::Key::LeftControl);
      SetSelectedEntity(entity, multipleSelection);
      ImGui::FocusWindow(ImGui::GetCurrentWindow());
    }
    
    if (isSelected)
    {
      ImGui::PopStyleColor();
    }
    
    // Drag & Drop -------------------------
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
    {
      ImGui::Text(entity.GetComponent<TagComponent>().tag.c_str());
      ImGui::SetDragDropPayload("scene_entity_hierarchy", &entity, sizeof(Entity));
      ImGui::EndDragDropSource();
    }
    
    if (ImGui::BeginDragDropTarget())
    {
      const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
      
      if (payload)
      {
        Entity& droppedEntity = *(Entity*)payload->Data;
        m_context->ParentEntity(droppedEntity, entity);
      }
      
      ImGui::EndDragDropTarget();
    }

    // Draw children ------------------------
    if (opened)
    {
      for (auto child : entity.Children())
      {
        DrawEntityNode(m_context->GetEntityWithUUID(child), searchFilter);
      }
      ImGui::TreePop();
    }
    
    // Defer deletion until end of node UI
    if (entityDeleted)
    {
      OnEntityDestroyed(entity);
    }
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
  
  bool SceneHierarchyPanel::IsChildSelected(Entity entity)
  {
    for (const auto& e : entity.Children())
    {
      Entity childEntity = m_context->TryGetEntityWithUUID(e);
      if (m_selectionContext.Find(childEntity))
      {
        return true;
      }
      
      if (IsChildSelected(childEntity))
      {
        return true;
      }
    }
    return false;
  }

} // namespace Kreator
