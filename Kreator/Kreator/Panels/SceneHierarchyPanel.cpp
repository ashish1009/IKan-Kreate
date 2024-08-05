//
//  SceneHierarchyPanel.cpp
//  Kreator
//
//  Created by Ashish . on 02/08/24.
//

#include "SceneHierarchyPanel.hpp"

#include "Editor/RendererLayer.hpp"
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

  static void DrawTitleBar(char* searchBuffer)
  {
    static const float height = 30.0f;
    
    // Draw the title Bar rectangle ---------------------------------------------------
    const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
    const ImVec2 titlebarMax =
    {
      ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth(),
      ImGui::GetCursorScreenPos().y + height
    };
    
    auto* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(titlebarMin, titlebarMax, UI::Color::TitleBar);
    
    // Search box
    if (searchBuffer)
    {
      DrawSearchBar(searchBuffer);
    }
  }

  template<typename T, typename UIFunction>
  static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, const Ref<Image>& settingsIcon, char* searchBuffer, bool defaultOpen = false, bool canBeRemoved = true)
  {
    if (!Kreator::UI::IsMatchingSearch(name, searchBuffer))
    {
      return;
    }
    
    if (entity.HasComponent<T>())
    {
      //  This fixes an issue where the first "+" button would display the "Remove" buttons for ALL components on an Entity.
      //  This is due to ImGui::TreeNodeEx only pushing the id for it's children if it's actually open
      ImGui::PushID((void*)typeid(T).hash_code());
      auto& component = entity.GetComponent<T>();
      ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
      
      bool open = UI::PropertyGridHeader(name, defaultOpen, 5, 5);
      bool rightClicked  = ImGui::IsItemClicked(ImGuiMouseButton_Right);
      float lineHeight  = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;
      
      bool resetValues = false;
      bool removeComponent = false;
      
      ImGui::SameLine(contentRegionAvailable.x - lineHeight);
      if (ImGui::InvisibleButton("##options", ImVec2{ lineHeight, lineHeight }) or rightClicked)
      {
        ImGui::OpenPopup("ComponentSettings");
      }
      UI::DrawButtonImage(settingsIcon, UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f), IM_COL32(160, 160, 160, 200), IM_COL32(160, 160, 160, 255), IM_COL32(160, 160, 160, 150));
      
      if (UI::BeginPopup("ComponentSettings"))
      {
        if (ImGui::MenuItem("Reset"))
        {
          resetValues = true;
        }
        
        if (canBeRemoved)
        {
          if (ImGui::MenuItem("Remove component"))
          {
            removeComponent = true;
          }
        }
        
        UI::EndPopup();
      }
      
      if (open)
      {
        uiFunction(component);
        UI::PropertyGridHeaderEnd();
      }
      
      if (removeComponent or resetValues)
      {
        entity.RemoveComponent<T>();
      }
      
      if (resetValues)
      {
        entity.AddComponent<T>();
      }
      
      if(!open)
      {
        UI::ShiftCursorY(-(ImGui::GetStyle().ItemSpacing.y + 1.0f));
      }
      ImGui::PopID();
    }
  }
  
  static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float minValue = 0.0f, float maxValue = 0.0f, [[maybe_unused]] float columnWidth = 100.0f)
  {
    bool modified = false;
    
    UI::ScopedStyle framePdding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2));
    UI::ScopedStyle framerounding(ImGuiStyleVar_FrameRounding, 0);
    
    UI::PushID();
    ImGui::TableSetColumnIndex(0);
    UI::ShiftCursor(0.0f, 7.0f);
    
    ImGui::Text(label.c_str());
    UI::DrawUnderline(false, 0.0f, 2.0f);
    
    ImGui::TableSetColumnIndex(1);
    UI::ShiftCursor(7.0f, 0.0f);
    
    {
      const float spacingX = 8.0f;
      UI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2{ spacingX, 0.0f });
      UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });
      
      {
        // Begin XYZ area
        UI::ScopedColor padding(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
        UI::ScopedStyle frame(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
        
        ImGui::BeginChild(ImGui::GetID((label + "fr").c_str()),
                          ImVec2(ImGui::GetContentRegionAvail().x - spacingX, ImGui::GetFrameHeightWithSpacing() + 10.0f),
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
      }
      const float framePadding = 2.0f;
      const float outlineSpacing = 1.0f;
      const float lineHeight = GImGui->Font->FontSize + framePadding * 2.0f;
      const ImVec2 buttonSize = { lineHeight + 2.0f, lineHeight };
      const float inputItemWidth = (ImGui::GetContentRegionAvail().x - spacingX) / 3.0f - buttonSize.x;
      
      auto drawControl = [&] (const std::string& label, float& value, const ImVec4& colourN, const ImVec4& colourH, const ImVec4& colourP) {
        {
          UI::ScopedStyle buttonFrame(ImGuiStyleVar_FramePadding, ImVec2(framePadding, 0.0f));
          UI::ScopedStyle buttonRounding(ImGuiStyleVar_FrameRounding, 1.0f);
          UI::ScopedColorStack buttonColours(ImGuiCol_Button, colourN,
                                             ImGuiCol_ButtonHovered, colourH,
                                             ImGuiCol_ButtonActive, colourP);
          
          UI::ScopedFont boldFont(UI::Font::Get(UI::FontType::Bold));
          
          UI::ShiftCursorY(2.0f);
          if (ImGui::Button(label.c_str(), buttonSize))
          {
            value = resetValue;
            modified = true;
          }
        }
        
        ImGui::SameLine(0.0f, outlineSpacing);
        ImGui::SetNextItemWidth(inputItemWidth);
        UI::ShiftCursorY(-2.0f);
        modified |= ImGui::DragFloat(("##" + label).c_str(), &value, 0.1f, minValue, maxValue, "%.2f");
        
        if (!UI::IsItemDisabled())
        {
          UI::DrawItemActivityOutline(2.0f, true, UI::Color::Accent);
        }
      };
      
      drawControl("X", values.x, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f }, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f }, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
      
      ImGui::SameLine(0.0f, outlineSpacing);
      drawControl("Y", values.y, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f }, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f }, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
      
      ImGui::SameLine(0.0f, outlineSpacing);
      drawControl("Z", values.z, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f }, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f }, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
      
      ImGui::EndChild();
    }
    UI::PopID();
    
    return modified;
  }
  
  struct PropertyFloatData
  {
    std::string title;
    float resetValue = 0.0f;
    float min = 0.0f, max = 0.0f;
  };
  
  static bool DrawFloat2Control(const std::string& label, float& value1, float& value2, const PropertyFloatData& data1,
                                const PropertyFloatData& data2, [[maybe_unused]] float columnWidth = 100.0f)
  {
    bool modified = false;
    
    UI::ScopedStyle framePdding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2));
    UI::ScopedStyle framerounding(ImGuiStyleVar_FrameRounding, 0);
    
    UI::PushID();
    ImGui::TableSetColumnIndex(0);
    UI::ShiftCursor(0.0f, 7.0f);
    
    ImGui::Text(label.c_str());
    UI::DrawUnderline(false, 0.0f, 2.0f);
    
    ImGui::TableSetColumnIndex(1);
    UI::ShiftCursor(7.0f, 0.0f);
    
    {
      const float spacingX = 5.0f;
      UI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2{ spacingX, 0.0f });
      UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });
      
      {
        // Begin XYZ area
        UI::ScopedColor padding(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
        UI::ScopedStyle frame(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
        
        ImGui::BeginChild(ImGui::GetID((label + "fr").c_str()),
                          ImVec2(ImGui::GetContentRegionAvail().x - spacingX, ImGui::GetFrameHeightWithSpacing() + 10.0f),
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
      }
      const float framePadding = 2.0f;
      const float outlineSpacing = 1.0f;
      const float lineHeight = GImGui->Font->FontSize + framePadding * 2.0f;
      const ImVec2 buttonSize = { 50, lineHeight };
      const float inputItemWidth = (ImGui::GetContentRegionAvail().x - spacingX) / 2.0f - buttonSize.x;
      
      auto drawControl = [&] (const std::string& label, float& value, float resetValue, float min, float max, const ImVec4& colourN,
                              const ImVec4& colourH, const ImVec4& colourP) {
        {
          UI::ScopedStyle buttonFrame(ImGuiStyleVar_FramePadding, ImVec2(framePadding, 0.0f));
          UI::ScopedStyle buttonRounding(ImGuiStyleVar_FrameRounding, 1.0f);
          UI::ScopedColorStack buttonColours(ImGuiCol_Button, colourN,
                                             ImGuiCol_ButtonHovered, colourH,
                                             ImGuiCol_ButtonActive, colourP);
          
          UI::ScopedFont boldFont(UI::Font::Get(UI::FontType::Bold));

          UI::ShiftCursorY(2.0f);
          if (ImGui::Button(label.c_str(), buttonSize))
          {
            value = resetValue;
            modified = true;
          }
        }
        
        ImGui::SameLine(0.0f, outlineSpacing);
        ImGui::SetNextItemWidth(inputItemWidth);
        UI::ShiftCursorY(-2.0f);
        modified |= ImGui::DragFloat(("##" + label).c_str(), &value, 0.1f, min, max);
        
        if (!UI::IsItemDisabled())
        {
          UI::DrawItemActivityOutline(2.0f, true, UI::Color::Accent);
        }
      };
      
      drawControl(data1.title, value1, data1.resetValue, data1.min, data1.max, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f },
                  ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f }, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });
      
      ImGui::SameLine(0.0f, outlineSpacing);
      drawControl(data2.title, value2, data2.resetValue, data2.min, data2.max, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f },
                  ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f }, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });
      
      ImGui::EndChild();
    }
    UI::PopID();
    
    return modified;
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
    s_EyeIcon.reset();
    s_closeEyeIcon.reset();
    IK_LOG_INFO("SceneHierarchyPanel", "Shutting down the Scene Hierarchy Panel");
  }
  
  SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context, bool isWindow)
  : m_context(context), m_isWindow(isWindow)
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

      ImRect windowRect = { ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax() };
      if (ImGui::BeginDragDropTargetCustom(UI::RectOffset(windowRect, ImGui::GetWindowPos()) , ImGui::GetCurrentWindow()->ID))
      {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("scene_entity_hierarchy", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
        
        if (payload)
        {
          IK_ASSERT(false);
        }
        
        ImGui::EndDragDropTarget();
      }
      
      if (m_selectedMeshMaterialAsset)
      {
        if (m_showMaterialEditor)
        {
          AssetEditorManager::OpenEditor(m_selectedMeshMaterialAsset);
        }
        else
        {
          AssetEditorManager::CloseEditor(m_selectedMeshMaterialAsset);
        }
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
        else
        {
          
        }
      }
    }
    
    if (m_isWindow)
    {
      ImGui::End();
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
    m_showMaterialEditor = false;

    if (!multipleSelection)
    {
      m_selectionContext.Clear();
    }
    
    if (!entity)
    {
      return;
    }
    
    if (m_selectionContext.Find(entity))
    {
      m_selectionContext.Erase(entity);
    }
    else 
    {
      m_selectionContext.PushBack(entity);
    }
    
    if (m_selectionChangedCallback)
    {
      m_selectionChangedCallback(m_selectionContext);
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
    const ImU32 colRowAlt = UI::ColorWithMultipliedValue(UI::Color::PopupBackground, 1.3f);
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
      /*| ImGuiTableFlags_Sortable*/;
      
      const int numColumns = 3;
      if (ImGui::BeginTable("##SceneHierarchy-Table", numColumns, tableFlags, ImVec2(ImGui::GetContentRegionAvail())))
      {
        ImGui::TableSetupColumn("Label");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Visibility");
        
        // Headers
        {
          const ImU32 colActive = UI::ColorWithMultipliedValue(UI::Color::TableHeaderBg, 1.2f);
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
          for (const auto& entity : m_context->GetRegistry().view<IDComponent, RelationshipComponent>())
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
          if (newEntity)
          {
            SetSelectedEntity(newEntity);
          }

          ImGui::EndPopup();
        }

        ImGui::EndTable();
      } // Begin table
    }  // Table Scope
  }
  
  void SceneHierarchyPanel::DrawEntityNode(Entity entity, const std::string &searchFilter)
  {
    const char* name = entity.GetComponent<TagComponent>().tag.c_str();
    constexpr uint32_t maxSearchDepth = 20;
    
    // Search entity with filter string
    bool hasChildMatchingSearch = SearchEntityRecursive(entity, searchFilter, maxSearchDepth);
    if (!Kreator::UI::IsMatchingSearch(name, searchFilter) and !hasChildMatchingSearch)
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
      ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), ImGui::TableGetColumnCount() - 2).Max.x,
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
      if (isWindowFocused or UI::NavigatedTo())
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
    bool missingMesh = entity.HasComponent<MeshComponent>() and (AssetManager::IsAssetHandleValid(entity.GetComponent<MeshComponent>().mesh)
                                                                 and AssetManager::GetAsset<Mesh>(entity.GetComponent<MeshComponent>().mesh)
                                                                 and AssetManager::GetAsset<Mesh>(entity.GetComponent<MeshComponent>().mesh)->IsFlagSet(AssetFlag::Missing));
    if (missingMesh)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.4f, 0.3f, 1.0f));
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
    const bool isMouseXOverSrrow = (g.IO.MousePos.x >= arrowHitX1 and g.IO.MousePos.x < arrowHitX2);
    
    bool previousState = ImGui::TreeNodeBehaviorIsOpen(ImGui::GetID(strID.c_str()));
    if (isMouseXOverSrrow and isRowClicked)
    {
      ImGui::SetNextItemOpen(!previousState);
    }

    const bool opened = UI::TreeNodeWithIcon(nullptr, ImGui::GetID(strID.c_str()), flags, name, nullptr);
    bool entityDeleted = false;
    bool entityDuplicated = false;

    if (ImGui::BeginPopupContextItem())
    {
      {
        UI::ScopedColor colText(ImGuiCol_Text, UI::Color::Text);
        UI::ScopedColorStack entitySelection(ImGuiCol_Header, UI::Color::HoveredItem, ImGuiCol_HeaderHovered, UI::Color::HoveredItem, ImGuiCol_HeaderActive, UI::Color::HoveredItem);
        
        // Select current entity
        SetSelectedEntity(entity);

        // Empty Space Right click menu
        Entity newEntity = ECS_Utils::DrawCreateEntityMenu(m_context, entity);
        if (newEntity)
        {
          SetSelectedEntity(newEntity);
        }

        // Selected Entity Right click
        if (m_selectionContext.Size() == 1)
        {
          ImGui::Separator();
          if (m_selectionContext.At(0).GetParent())
          {
            if (ImGui::MenuItem("Unparent", nullptr, false, false))
            {
              IK_ASSERT(false, "Implement later")
              ImGui::Separator();
            }
          }
          
          if (ImGui::MenuItem("Delete"))
          {
            entityDeleted = true;
          }
          if (ImGui::MenuItem("Duplicate"))
          {
            entityDuplicated = true;
          }
        }
      }
      ImGui::EndPopup();
    }
    
    if (isRowClicked)
    {
      bool multipleSelection = Input::IsKeyPressed(IKan::Key::LeftSuper);
      SetSelectedEntity(entity, multipleSelection);
      ImGui::FocusWindow(ImGui::GetCurrentWindow());
    }

    if (isSelected)
    {
      ImGui::PopStyleColor();
    }
    if (missingMesh)
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
        IK_ASSERT(false, "Implement later");
      }
      
      ImGui::EndDragDropTarget();
    }

    // <> column 2 -------------------------------------------------------------------------------------------------
    ImGui::TableNextColumn();

    // <> column 3 -------------------------------------------------------------------------------------------------
    ImGui::TableNextColumn();
    ImGui::PushID(entity);
    const auto& visibility = entity.GetComponent<VisibilityComponent>().isVisible;
    
    static float lineHeight  = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;
    if (ImGui::InvisibleButton("##Visibility", ImVec2{ 1.3f * lineHeight, 1.3f * lineHeight }))
    {
      auto changeChildVisibility = [&](Entity e, auto&& changeChildVisibility) -> void {
        auto& v = e.GetComponent<VisibilityComponent>().isVisible;
        v = v ? false : true;
        for (const auto& c : e.Children())
        {
          Entity child = m_context->GetEntityWithUUID(c); // Has to be present
          changeChildVisibility(child, changeChildVisibility);
        }
      };
      
      changeChildVisibility(entity, changeChildVisibility);
    }
    
    ImU32 normalColor = IM_COL32(160, 160, 160, 200);
    ImU32 hoveredColor = IM_COL32(160, 160, 160, 255);
    ImU32 pressedColor = IM_COL32(160, 160, 160, 150);
    if (isSelected)
    {
      normalColor = IM_COL32(10, 10, 10, 200);
      hoveredColor = IM_COL32(10, 10, 10, 255);
      pressedColor = IM_COL32(10, 10, 10, 150);
    }
    UI::DrawButtonImage(visibility ? s_EyeIcon : s_closeEyeIcon, UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f), normalColor, hoveredColor, pressedColor);
    
    ImGui::PopID();
    
    // Draw children ------------------------
    if (opened)
    {
      for (auto child : entity.Children())
      {
        if (Entity e = m_context->TryGetEntityWithUUID(child); e)
        {
          DrawEntityNode(e, searchFilter);
        }
      }
      ImGui::TreePop();
    }
    
    // Defer deletion until end of node UI
    if (entityDeleted)
    {
      OnEntityDestroyed(entity);
    }
    
    if (entityDuplicated)
    {
      SetSelectedEntity(m_context->DuplicateEntity(entity));
    }
  }
  
  void SceneHierarchyPanel::DrawComponents(Entity entity)
  {
    IK_PERFORMANCE("SceneHierarchyPanel::DrawComponents");
    
    static constexpr float roundingVal = 5.0f;
    
    static char searchedString[128];
    DrawTitleBar(searchedString);

    ImGui::AlignTextToFramePadding();
    auto ID = entity.GetComponent<IDComponent>().ID;
    ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
    UI::ShiftCursor(4.0f, 4.0f);
    
    // + Button Data for Input Text Size and position
    bool isHoveringID = false;
    static const std::string buttonString = " ADD        ";
    ImVec2 textSize = ImGui::CalcTextSize(buttonString.c_str());
    
    static const float pad = 4.0f;
    const float iconWidth = ImGui::GetFrameHeight() - pad * 2.0f;
    const float iconHeight = iconWidth;
    
    // Tag --------------------------------------
    if (entity.HasComponent<TagComponent>())
    {
      UI::ScopedStyle rounding (ImGuiStyleVar_FrameRounding, roundingVal);
      auto& tag = entity.GetComponent<TagComponent>().tag;
      char buffer[256];
      memset(buffer, 0, 256);
      memcpy(buffer, tag.c_str(), tag.length());
      ImGui::PushItemWidth(contentRegionAvailable.x - textSize.x - iconWidth - pad * 2.0f);
      
      UI::ScopedStyle frameBorder(ImGuiStyleVar_FrameBorderSize, 0.0f);
      UI::ScopedColor frameColour(ImGuiCol_FrameBg, UI::Color::Background);
      
      if (ImGui::InputText("##Tag", buffer, 256))
      {
        tag = std::string(buffer);
      }
      UI::DrawItemActivityOutline(roundingVal, false, UI::Color::Accent);
      
      isHoveringID = ImGui::IsItemHovered();
      
      ImGui::PopItemWidth();
    }
    
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    textSize.x += GImGui->Style.FramePadding.x * 2.0f;
    {
      UI::ScopedColorStack addCompButtonColours(ImGuiCol_Button, IM_COL32(70, 70, 70, 200),
                                                ImGuiCol_ButtonHovered, IM_COL32(70, 70, 70, 255),
                                                ImGuiCol_ButtonActive, IM_COL32(70, 70, 70, 150));
      
      ImGui::SameLine(contentRegionAvailable.x - (textSize.x + GImGui->Style.FramePadding.x));
      if (ImGui::Button(buttonString.c_str(), ImVec2(textSize.x + 4.0f, lineHeight + 2.0f)))
      {
        ImGui::OpenPopup("AddComponentPanel");
      }
      
      ImVec2 iconPos = ImGui::GetItemRectMax();
      iconPos.x -= iconWidth + pad;
      iconPos.y -= iconHeight + pad;
      ImGui::SetCursorScreenPos(iconPos);
      UI::ShiftCursor(-5.0f, -1.0f);
      
      UI::Image(s_plusIcon, ImVec2(iconWidth, iconHeight));
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    
    AddComponentPopup();
    ImGui::Separator();

    // For Asset Selector
    static UI::PropertyAssetReferenceSettings settings;
    
    DrawComponent<TransformComponent>("Transform", entity, [this, &entity](TransformComponent& component)
                                      {
      UI::ScopedStyle spacing (ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
      UI::ScopedStyle padding (ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
      
      ImGui::BeginTable("transformComponent", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoClip);
      ImGui::TableSetupColumn("label_column", 0, 100.0f);
      ImGui::TableSetupColumn("value_column", ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_NoClip,
                              ImGui::GetContentRegionAvail().x - 100.0f);
      
      ImGui::TableNextRow();
      auto position = component.Position();
      DrawVec3Control("Translation", position);
      component.UpdatePosition(position);
      
      ImGui::TableNextRow();
      auto rotation = glm::degrees(component.Rotation());
      DrawVec3Control("Rotation", rotation);
      component.UpdateRotation(glm::radians(rotation));
      
      ImGui::TableNextRow();
      auto scale = component.Scale();
      DrawVec3Control("Scale", scale, 1.0f, 0.1f);
      component.UpdateScale(scale);
      ImGui::EndTable();
      
      UI::ShiftCursorY(-8.0f);
      UI::ShiftCursorY(18.0f);
    }, s_gearIcon, searchedString, true, false);
    
    DrawComponent<RelationshipComponent>("Relation", entity, [&](RelationshipComponent& rc)
                                         {
      UI::ScopedStyle headerRounding(ImGuiStyleVar_FrameRounding, roundingVal);
      UI::BeginPropertyGrid();
      
      UI::PropertyAssetReferenceSettings settings;
      settings.advanceToNextColumn = false;
      settings.widthOffset = 40;
      
      UUID curretnParent = 0;
      DrawEntitySelector("Parent", rc.parentHandle, curretnParent, settings);
      Entity parentEntity = m_context->TryGetEntityWithUUID(curretnParent);
      if (parentEntity)
      {
        IKan::ECS_Utils::ParentEntity(m_context, entity, parentEntity);
      }
      
      ImGui::SameLine();
      float prevItemHeight = ImGui::GetItemRectSize().y;
      if (UI::DrawButton("X", UI::FontType::Regular, UI::Color::Text, UI::Color::Muted, 5.0f, {prevItemHeight, prevItemHeight}))
      {
        IKan::ECS_Utils::UnparentEntity(m_context, entity);
      }
      ImGui::NextColumn();
      UI::EndPropertyGrid();
      
      if (rc.children.size())
      {
        UI::ScopedColor header(ImGuiCol_Header, UI::Color::PopupBackground);
        if (UI::PropertyGridHeader("Children", false, 2, roundingVal))
        {
          UI::BeginPropertyGrid();
          {
            UI::ScopedDisable disable;
            for (const auto& c : rc.children)
            {
              Entity e = m_context->TryGetEntityWithUUID(c);
              if (e)
              {
                UI::Property(e.GetName().c_str(), std::to_string(e.GetUUID()));
              }
            }
          }
          UI::EndPropertyGrid();
          UI::PropertyGridHeaderEnd();
        }
      }
      
    }, s_gearIcon, searchedString, true, false);
    
    DrawComponent<CameraComponent>("Camera", entity, [this](CameraComponent& cc)
                                   {
      UI::BeginPropertyGrid();
      UI::ScopedStyle framePdding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2));
      
      UI::Property("Main Camera", cc.primary);
      
      // Projection Type
      const char* projTypeStrings[] = { "Perspective", "Orthographic" };
      int currentProj = (int)cc.camera.GetProjectionType();
      if (UI::PropertyDropdown("Projection", projTypeStrings, 2, &currentProj))
      {
        cc.camera.SetProjectionType((SceneCamera::ProjectionType)currentProj);
      }
      ImGui::Separator();
      
      // Perspective parameters
      if (cc.camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
      {
        float verticalFOV = cc.camera.GetDegPerspectiveVerticalFOV();
        if (UI::Property("Vertical FOV", verticalFOV))
        {
          cc.camera.SetDegPerspectiveVerticalFOV(verticalFOV);
        }
        float nearClip = cc.camera.GetPerspectiveNearClip();
        if (UI::Property("Near Clip", nearClip))
        {
          cc.camera.SetPerspectiveNearClip(nearClip);
        }
        float farClip = cc.camera.GetPerspectiveFarClip();
        if (UI::Property("Far Clip", farClip))
        {
          cc.camera.SetPerspectiveFarClip(farClip);
        }
      }
      
      // Orthographic parameters
      else if (cc.camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
      {
        float orthoSize = cc.camera.GetOrthographicSize();
        if (UI::Property("Size", orthoSize, 1.0f))
        {
          cc.camera.SetOrthographicSize(orthoSize);
        }
        float nearClip = cc.camera.GetOrthographicNearClip();
        if (UI::Property("Near Clip", nearClip, 0.1f, -1.0f, 0.0f))
        {
          cc.camera.SetOrthographicNearClip(nearClip);
        }
        float farClip = cc.camera.GetOrthographicFarClip();
        if (UI::Property("Far Clip", farClip, 0.1f, 0.0f, 1.0f))
        {
          cc.camera.SetOrthographicFarClip(farClip);
        }
      }
      UI::EndPropertyGrid();
    }, s_gearIcon, searchedString, true);
    
    DrawComponent<MeshComponent>("Mesh", entity, [&](MeshComponent& smc)
                                 {
      UI::ScopedStyle headerRounding(ImGuiStyleVar_FrameRounding, roundingVal);
      UI::BeginPropertyGrid();
      
      Ref<Mesh> mesh = AssetManager::GetAsset<Mesh>(smc.mesh);
      AssetHandle currentMeshAsset = smc.mesh;
      if (UI::PropertyAssetReference<Mesh>("Mesh", currentMeshAsset, nullptr, settings))
      {
        const auto& metadata = AssetManager::GetMetadata(currentMeshAsset);
        smc.mesh = metadata.handle;
      }
      UI::EndPropertyGrid();
      
      // Materials
      {
        UI::ScopedColor header(ImGuiCol_Header, UI::Color::PopupBackground);
        bool open = UI::PropertyGridHeader("Material", true, 3, 5);
        
        bool rightClicked = ImGui::IsItemClicked(ImGuiMouseButton_Right);
        float lineHeight = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;
        
        ImGui::SameLine(contentRegionAvailable.x - lineHeight);
        if (ImGui::InvisibleButton("##CreateMaterial", ImVec2{ lineHeight, lineHeight }) or rightClicked)
        {
          ImGui::OpenPopup("CreateMaterial");
        }
        UI::DrawButtonImage(s_plusIcon, UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f), IM_COL32(160, 160, 160, 200), IM_COL32(160, 160, 160, 255), IM_COL32(160, 160, 160, 150));
        
        if (UI::BeginPopup("CreateMaterial"))
        {
          char buffer[256];
          memset(buffer, 0, 256);
          if (ImGui::InputText("##Tag", buffer, 256, ImGuiInputTextFlags_EnterReturnsTrue))
          {
            // TODO: Later increase the material for now only 0 is used
            std::string filename = buffer;
            filename += MaterialExtension;
            smc.materialTable->SetMaterial(0, AssetManager::CreateNewAsset<MaterialAsset>(filename, Project::GetActive()->GetMaterialDirectory()));
            m_showMaterialEditor = true;
            ImGui::CloseCurrentPopup();
          }

          UI::EndPopup();
        }
        
        if (open)
        {
          UI::BeginPropertyGrid();
          
          for (size_t i = 0; i < smc.materialTable->GetMaterialCount(); i++)
          {
            bool hasMaterial = smc.materialTable->HasMaterial((uint32_t)i);
            
            std::string label = fmt::format("[Material {0}]", i);

            // Note: Fix for weird ImGui ID bug...
            std::string id = fmt::format("{0}-{1}", label, i);
            ImGui::PushID(id.c_str());

            UI::PropertyAssetReferenceSettings settings;
            if (hasMaterial)
            {
              m_selectedMeshMaterialAsset = smc.materialTable->GetMaterial((uint32_t)i);
              AssetHandle materialAssetHandle = m_selectedMeshMaterialAsset->handle;
              settings.advanceToNextColumn = false;
              settings.widthOffset = 80;
              UI::Property("Tiling Factor", smc.tilingFactor, 1.0f, 1.0f, 1000.0f);
              UI::PropertyAssetReferenceTarget<MaterialAsset>(label.c_str(), nullptr, materialAssetHandle, [smc, i](Ref<MaterialAsset> materialAsset){
                smc.materialTable->SetMaterial((uint32_t)i, materialAsset);
              }, settings);
              
              ImGui::SameLine();
              float prevItemHeight = ImGui::GetItemRectSize().y;
              if (UI::DrawButton("X", UI::FontType::Regular, UI::Color::Text, UI::Color::Muted, 5.0, {prevItemHeight, prevItemHeight}))
              {
                smc.materialTable->ClearMaterial((uint32_t)i);
              }
              
              ImGui::SameLine();
              static float lineHeight  = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;
              if (ImGui::InvisibleButton("##CreateMaterial", ImVec2{ lineHeight, lineHeight }))
              {
                m_showMaterialEditor = m_showMaterialEditor ? false : true;
              }
              UI::DrawButtonImage(m_showMaterialEditor ? s_EyeIcon : s_closeEyeIcon, UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f),
                                  IM_COL32(160, 160, 160, 200), IM_COL32(160, 160, 160, 255), IM_COL32(160, 160, 160, 150));
              
              ImGui::NextColumn();
            }
            else
            {
              AssetHandle materialAssetHandle {};
              UI::PropertyAssetReferenceTarget<MaterialAsset>(label.c_str(), "Empty", materialAssetHandle, [smc, i](Ref<MaterialAsset> materialAsset) {
                smc.materialTable->SetMaterial((uint32_t)i, materialAsset);
              }, settings);
            }
            ImGui::PopID();
          }
          
          UI::EndPropertyGrid();
          UI::PropertyGridHeaderEnd();
        } // property grid header
      }
    }, s_gearIcon, searchedString, true);
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
  
  bool SceneHierarchyPanel::IsChildSelected(Entity entity)
  {
    if (!entity)
    {
      return false;
    }
    
    // For each child
    for (const auto& e : entity.Children())
    {
      Entity childEntity = m_context->TryGetEntityWithUUID(e);
      if (!childEntity)
      {
        return false;
      }
      
      // Search child entity in selection context
      if (m_selectionContext.Find(childEntity))
      {
        return true;
      }
      
      // Recursive search
      if (IsChildSelected(childEntity))
      {
        return true;
      }
    }
    return false;
  }

  void SceneHierarchyPanel::OnExternalEntityDestroyed(const Entity& entity)
  {
    IK_PROFILE();
    m_entityDeletedCallback(m_selectionContext);

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
  
  void SceneHierarchyPanel::DrawEntitySelector(const char* title, UUID entityID, UUID& outID, const UI::PropertyAssetReferenceSettings& settings)
  {
    Entity targetEntity = m_context->TryGetEntityWithUUID(entityID);
    if (UI::PropertyEntityReference(title, targetEntity, settings))
    {
      outID = targetEntity.GetUUID();
    }
    if (ImGui::BeginPopupContextItem())
    {
      {
        UI::ScopedColor colText(ImGuiCol_Text, UI::Color::Text);
        UI::ScopedColorStack entitySelection(ImGuiCol_Header, UI::Color::HoveredItem, ImGuiCol_HeaderHovered,
                                             UI::Color::HoveredItem, ImGuiCol_HeaderActive, UI::Color::HoveredItem);
        
        if (targetEntity)
        {
          if (ImGui::MenuItem("Remove"))
          {
            outID = 1; // Valid UUID is always 64 bit so 1 is also invalid
          }
        }
      }
      ImGui::EndPopup();
    }
  }
  
  void SceneHierarchyPanel::AddComponentPopup()
  {
    if (UI::BeginPopup("AddComponentPanel"))
    { 
      if (!m_selectionContext.At(0).HasComponent<CameraComponent>())
      {
        if (ImGui::MenuItem("Camera"))
        {
          [[maybe_unused]] auto& cameraComp = m_selectionContext.At(0).AddComponent<CameraComponent>();
          ImGui::CloseCurrentPopup();
        }
        ImGui::Separator();
      }
      
      if (!m_selectionContext.At(0).HasComponent<MeshComponent>())
      {
        if (ImGui::MenuItem("Mesh"))
        {
          auto& meshComp = m_selectionContext.At(0).AddComponent<MeshComponent>();
          meshComp.mesh = 0;
          ImGui::CloseCurrentPopup();
        }
        ImGui::Separator();
      }      

      UI::EndPopup();
    }
  }
} // namespace Kreator
