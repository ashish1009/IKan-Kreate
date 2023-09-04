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
    if(m_context)
    {
      m_context->SetEntityDestroyedCallback([this](Entity entity) { OnExternalEntityDestroyed(entity); });
    }
  }
  
  void SceneHierarchyPanel::SetSceneContext(const Ref<Scene>& scene)
  {
    m_context = scene;
    m_selectionContext = {};
    if(m_context)
    {
      m_context->SetEntityDestroyedCallback([this](Entity entity) { OnExternalEntityDestroyed(entity); });
    }
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
      if (ImGui::BeginDragDropTargetCustom(windowRect, ImGui::GetCurrentWindow()->ID))
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
    const char* name = entity.GetComponent<TagComponent>().tag.c_str();
    const uint32_t maxSearchDepth = 10;
    bool hasChildMatchingSearch = TagSearchRecursive(entity, searchFilter, maxSearchDepth);
    
    if (!Kreator_UI::IsMatchingSearch(name, searchFilter) and !hasChildMatchingSearch)
    {
      return;
    }

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
    
    const bool isSelected = entity == m_selectionContext;
    ImGuiTreeNodeFlags flags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    
    if (hasChildMatchingSearch)
    {
      flags |= ImGuiTreeNodeFlags_DefaultOpen;
    }
    
    if (entity.Children().empty())
    {
      flags |= ImGuiTreeNodeFlags_Leaf;
    }

    const std::string strID = std::string(name) + std::to_string((uint32_t)entity);

    ImGui::PushClipRect(rowAreaMin, rowAreaMax, false);
    bool isRowHovered, held;
    bool isRowClicked = ImGui::ButtonBehavior(ImRect(rowAreaMin, rowAreaMax), ImGui::GetID(strID.c_str()),
                                              &isRowHovered, &held, ImGuiButtonFlags_AllowItemOverlap);
    ImGui::SetItemAllowOverlap();
    ImGui::PopClipRect();
    
    const bool isWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

    // Row Coloring
    //--------------
    // Fill with light selection Color if any of the child entities selected
    auto isAnyDescendantSelected = [&](Entity ent, auto isAnyDescendantSelected) -> bool
    {
      if (ent == m_selectionContext)
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
        fillRowWithColor(Kreator_UI::Color::Selection);
      }
      else
      {
        const ImColor col = UI::ColorWithMultipliedValue(Kreator_UI::Color::Selection, 0.9f);
        fillRowWithColor(UI::ColorWithMultipliedSaturation(col, 0.7f));
      }
    }
    else if (isRowHovered)
    {
      fillRowWithColor(UI::Theme::Color::BackgroundPopup);
    }
    else if (isAnyDescendantSelected(entity, isAnyDescendantSelected))
    {
      fillRowWithColor(Kreator_UI::Color::SelectionMuted);
    }
    
    // Text Coloring
    //---------------
    if (isSelected)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, UI::Theme::Color::BackgroundDark);
    }

    // Tree node
    //----------
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
        UI::ScopedColor colText(ImGuiCol_Text, UI::Theme::Color::Text);
        UI::ScopedColorStack entitySelection(ImGuiCol_Header, UI::Theme::Color::GroupHeader,
                                             ImGuiCol_HeaderHovered, UI::Theme::Color::GroupHeader,
                                             ImGuiCol_HeaderActive, UI::Theme::Color::GroupHeader);
        DrawCreateEntityMenu(entity);
        
        if (ImGui::MenuItem("Delete"))
        {
          entityDeleted = true;
        }
      }
      ImGui::EndPopup();
    }
    
    // Type column
    //------------
    ImGui::TableNextColumn();
    if (isRowClicked)
    {
      SetSelectedEntity(entity);
      ImGui::FocusWindow(ImGui::GetCurrentWindow());
    }
    
    if (isSelected)
    {
      ImGui::PopStyleColor();
    }
    
    // Drag & Drop
    //------------
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
    
    ImGui::TableNextColumn();

    // Draw children
    //--------------
    if (opened)
    {
      for (auto child : entity.Children())
      {
        DrawEntityNode(m_context->GetEntityWithUUID(child), searchFilter);
      }
      ImGui::TreePop();
    }
  }
  
  void SceneHierarchyPanel::DrawCreateEntityMenu(Entity parent)
  {
    Entity newEntity;
    if (ImGui::MenuItem("Empty Entity"))
    {
      newEntity = m_context->CreateEntity("Empty Entity");
    }
    
    
    if (ImGui::MenuItem("Camera"))
    {
      newEntity = m_context->CreateEntity("Camera");
      newEntity.AddComponent<CameraComponent>();
    }
    
    if (ImGui::MenuItem("Text"))
    {
      newEntity = m_context->CreateEntity("Text");
      newEntity.AddComponent<TextComponent>();
    }
    
    if (ImGui::BeginMenu("2D"))
    {
      if (ImGui::MenuItem("Quad"))
      {
        newEntity = m_context->CreateEntity("Quad");
        newEntity.AddComponent<QuadComponent>();
      }
      
      if (ImGui::MenuItem("Circle"))
      {
        newEntity = m_context->CreateEntity("Circle");
        newEntity.AddComponent<CircleComponent>();
      }
      
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("3D"))
    {
      auto menuForDefaultMesh = [this](Entity& newEntity, const std::string& name) {
        if (ImGui::MenuItem(name.c_str()))
        {
          newEntity = m_context->CreateEntity(name);
          std::string file = Project::GetActive()->GetMeshPath("Default/");
          file += name;
          file += ".fbx";
          auto meshSourceHandle = AssetManager::CreateMemoryOnlyAssetWithFile<MeshSource>(file, file);
          newEntity.AddComponent<StaticMeshComponent>(meshSourceHandle);
        }
      };
      
      if (ImGui::MenuItem("Empty Mesh"))
      {
        newEntity = m_context->CreateEntity("Mesh");
        newEntity.AddComponent<StaticMeshComponent>();
      }
      
      menuForDefaultMesh(newEntity, "Cube");
      menuForDefaultMesh(newEntity, "Cone");
      menuForDefaultMesh(newEntity, "Capsule");
      menuForDefaultMesh(newEntity, "Cylinder");
      menuForDefaultMesh(newEntity, "Plane");
      menuForDefaultMesh(newEntity, "Sphere");
      menuForDefaultMesh(newEntity, "Torus");
      
      ImGui::EndMenu();
    }
    
    if (newEntity and parent)
    {
      m_context->ParentEntity(newEntity, parent);
    }
    
    if (newEntity)
    {
      SetSelectedEntity(newEntity);
    }
  }
  
  bool SceneHierarchyPanel::TagSearchRecursive(Entity entity, std::string_view searchFilter, uint32_t maxSearchDepth,
                                               uint32_t currentDepth)
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
        if (Kreator_UI::IsMatchingSearch(e.GetComponent<TagComponent>().tag, searchFilter))
        {
          return true;
        }
      }
      
      bool found = TagSearchRecursive(e, searchFilter, maxSearchDepth, currentDepth + 1);
      if (found)
      {
        return true;
      }
    }
    return false;
  }
  
  void SceneHierarchyPanel::OnExternalEntityDestroyed(Entity entity)
  {
    if (entity == m_selectionContext)
    {
      m_selectionContext = {};
    }
  }

  void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
  {
    m_selectionContext = entity;
    
    if (m_selectionChangedCallback)
    {
      m_selectionChangedCallback(m_selectionContext);
    }
  }

  void SceneHierarchyPanel::SetSelectionChangedCallback(const std::function<void(Entity)>& func)
  {
    m_selectionChangedCallback = func;
  }
  void SceneHierarchyPanel::SetEntityDeletedCallback(const std::function<void(Entity)>& func)
  {
    m_entityDeletedCallback = func;
  }

} // namespace Kreator
