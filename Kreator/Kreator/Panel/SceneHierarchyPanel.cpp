//
//  SceneHierarchyPanel.cpp
//  Kreator
//
//  Created by Ashish . on 22/02/24.
//

#include "SceneHierarchyPanel.hpp"
#include "Layer/KreatorLayer.hpp"
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
  
  template<typename T, typename UIFunction>
  static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, const Ref<Texture>& settingsIcon, char* searchBuffer, bool defaultOpen = false, bool canBeRemoved = true)
  {
    RETURN_IF(!Kreator::UI::IsMatchingSearch(name, searchBuffer));
    
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
      UI::DrawButtonImage(settingsIcon, IM_COL32(160, 160, 160, 200), IM_COL32(160, 160, 160, 255), IM_COL32(160, 160, 160, 150), UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f));
      
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
  
  static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float minValue = 0.0f, float maxValue = 0.0f, float columnWidth = 100.0f)
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
          
          UI::ScopedFont boldFont(UI::GetBoldFont());
          
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
                                const PropertyFloatData& data2, float columnWidth = 100.0f)
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
          
          UI::ScopedFont boldFont(UI::GetBoldFont());
          
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
    drawList->AddRectFilled(titlebarMin, titlebarMax, UI::Color::Titlebar);
    
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
    s_closeEyeIcon = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/CloseEye.png");
    s_EyeIcon = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/Eye.png");
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
    // Should be after loading the asset registry
    s_defaultMaterialAsset = AssetManager::GetAsset<MaterialAsset>("Materials/Default.ikmat")->handle;
  }
  
  void SceneHierarchyPanel::OnImGuiRender(bool& isOpen)
  {
    IK_PERFORMANCE("SceneHierarchyPanel::OnImGuiRender");
    if (m_isWindow)
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
        else if (m_lastSelectedEntity)
        {
          DrawComponents(m_lastSelectedEntity);
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
    static const float edgeOffset = 4.0f;
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
    bool isPrefab = entity.HasComponent<PrefabComponent>();
    if (isPrefab)
    {
      isPrefab = AssetManager::IsAssetHandleValid(entity.GetComponent<PrefabComponent>().PrefabID);
      if (!isPrefab)
      {
        entity.RemoveComponent<PrefabComponent>();
      }
    }
    if (isPrefab && !isSelected)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.32f, 0.7f, 0.87f, 1.0f));
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
    
    if (ImGui::BeginPopupContextItem())
    {
      {
        UI::ScopedColor colText(ImGuiCol_Text, UI::Color::Text);
        UI::ScopedColorStack entitySelection(ImGuiCol_Header, UI::Color::GroupHeader, ImGuiCol_HeaderHovered, UI::Color::GroupHeader, ImGuiCol_HeaderActive, UI::Color::GroupHeader);
        
        // Select current entity
        SetSelectedEntity(entity);
        
        if (isPrefab)
        {
          if (ImGui::MenuItem("Update Prefab"))
          {
            AssetHandle prefabAssetHandle = entity.GetComponent<PrefabComponent>().PrefabID;
            Ref<Prefab> prefab = AssetManager::GetAsset<Prefab>(prefabAssetHandle);
            if (prefab)
            {
              prefab->Create(entity);
            }
            else
            {
              IK_LOG_ERROR("SceneHierarchyPanel", "Prefab has invalid asset handle: {0}", prefabAssetHandle);
            }
          }
        }

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
        Entity& droppedEntity = *(Entity*)payload->Data;
        m_context->ParentEntity(droppedEntity, entity);
      }
      
      ImGui::EndDragDropTarget();
    }

    // <> column 2 -------------------------------------------------------------------------------------------------
    ImGui::TableNextColumn();
    if (isPrefab)
    {
      UI::ShiftCursorX(edgeOffset * 3.0f);
      
      if (isSelected)
      {
        ImGui::PushStyleColor(ImGuiCol_Text, UI::Color::BackgroundDark);
      }
      
      ImGui::TextUnformatted("Prefab");
      ImGui::PopStyleColor();
    }
    
    // <> column 3 -------------------------------------------------------------------------------------------------
    ImGui::TableNextColumn();

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
      
#if 0
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
#else
      ImGui::TableNextRow();
      auto prevPosition = component.Position();
      auto position = component.Position();
      if (DrawVec3Control("Translation", position))
      {
        ECS_Utils::UpdateChildrenTransform(m_context, entity, position - prevPosition, Utils::Math::ZeroVec3, Utils::Math::ZeroVec3, entity);
      }
      
      ImGui::TableNextRow();
      auto rotation = glm::degrees(component.Rotation());
      auto prevRotation = glm::degrees(component.Rotation());
      if (DrawVec3Control("Rotation", rotation))
      {
        ECS_Utils::UpdateChildrenTransform(m_context, entity, Utils::Math::ZeroVec3, Utils::Math::ZeroVec3, glm::radians(rotation) - glm::radians(prevRotation), entity);
      }
      
      ImGui::TableNextRow();
      auto scale = component.Scale();
      auto prevScale = component.Scale();
      if (DrawVec3Control("Scale", scale, 1.0f, 0.01f, 100000.0f))
      {
        ECS_Utils::UpdateChildrenTransform(m_context, entity, Utils::Math::ZeroVec3, scale - prevScale, Utils::Math::ZeroVec3, entity);
      }
      ImGui::EndTable();

      if (entity.Children().size())
      {
        UI::ScopedColor header(ImGuiCol_Header, UI::Color::BackgroundPopup);
        if (UI::PropertyGridHeader("Delta Transform", false, 2, roundingVal))
        {
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

          UI::PropertyGridHeaderEnd();
        }
      }
#endif

      
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
        m_context->ParentEntity(entity, parentEntity);
      }

      ImGui::SameLine();
      float prevItemHeight = ImGui::GetItemRectSize().y;
      if (UI::DrawRoundButton("X", UI::Color::Muted, 20, { prevItemHeight, prevItemHeight }))
      {
        m_context->UnparentEntity(entity);
      }
      ImGui::NextColumn();
      UI::EndPropertyGrid();

      if (rc.children.size())
      {
        UI::ScopedColor header(ImGuiCol_Header, UI::Color::BackgroundPopup);
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
      
    }, s_gearIcon, searchedString, false, false);
    
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
      
      {
        ImGui::PushID("CameraController");
        UI::ScopedColor header(ImGuiCol_Header, UI::Color::BackgroundPopup);
        bool open = UI::PropertyGridHeader("Controller", true, 3, 5);
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
        
        bool rightClicked  = ImGui::IsItemClicked(ImGuiMouseButton_Right);
        float lineHeight  = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;
        
        ImGui::SameLine(contentRegionAvailable.x - 10);
        if (ImGui::InvisibleButton("##options", ImVec2{ lineHeight, lineHeight }) or rightClicked)
        {
          cc.controller.ResetView();
        }
        UI::DrawButtonImage(s_reloadIcon, IM_COL32(160, 160, 160, 200), IM_COL32(160, 160, 160, 255),
                            IM_COL32(160, 160, 160, 150), UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f));
        
        if (open)
        {
          UI::BeginPropertyGrid();
          
          UUID followID = 0;
          DrawEntitySelector("Follow", cc.controller.GetFollowEntity(), followID);
          if (followID > 0)
          {
            cc.controller.SetFollowEntity(followID);
          }
          UI::EndPropertyGrid();
          
          Entity followEntity = m_context->TryGetEntityWithUUID(cc.controller.GetFollowEntity());
          if (followEntity)
          {
            UI::BeginPropertyGrid();
            float sensitivity = cc.controller.GetSensitivity();
            if (UI::Property("Sensitivity", sensitivity))
            {
              cc.controller.SetSensitivity(sensitivity);
            }
            UI::EndPropertyGrid();
            
            ImGui::BeginTable("ControllerOrbits", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoClip);
            ImGui::TableSetupColumn("label_column", 0, 80.0f);
            ImGui::TableSetupColumn("value_column", ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_NoClip,
                                    ImGui::GetContentRegionAvail().x - 100.0f);
            
            CameraController::Orbit topOrbit = cc.controller.GetTopOrbit();
            CameraController::Orbit bottomOrbit = cc.controller.GetBottomOrbit();
            CameraController::Orbit midOrbit = cc.controller.GetMidOrbit();
            
            ImGui::TableNextRow();
            if (DrawFloat2Control("Top Orbit", topOrbit.radius, topOrbit.height, {"Radius", 1, 1, 1000},
                                  {"Height", 1, midOrbit.height, 10000}))
            {
              cc.controller.SetTopOrbit(topOrbit);
            }
            
            ImGui::TableNextRow();
            if (DrawFloat2Control("Mid Orbit", midOrbit.radius, midOrbit.height, {"Radius", 1, 1, 1000},
                                  {"Height", 1, bottomOrbit.height, topOrbit.height}))
            {
              cc.controller.SetMidOrbit(midOrbit);
            }
            
            ImGui::TableNextRow();
            if (DrawFloat2Control("Bottom Orbit", bottomOrbit.radius, bottomOrbit.height, {"Radius", 1, 1, 1000},
                                  {"Height", 0, -10000, midOrbit.height}))
            {
              cc.controller.SetBottomOrbit(bottomOrbit);
            }
            ImGui::EndTable();
          }
          
          UI::PropertyGridHeaderEnd();
        }
        ImGui::PopID();
      } // Scope end
    }, s_gearIcon, searchedString);
    
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
        UI::ScopedColor header(ImGuiCol_Header, UI::Color::BackgroundPopup);
        bool open = UI::PropertyGridHeader("Material", true, 3, 5);

        bool rightClicked = ImGui::IsItemClicked(ImGuiMouseButton_Right);
        float lineHeight = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;

        ImGui::SameLine(contentRegionAvailable.x - lineHeight);
        if (ImGui::InvisibleButton("##CreateMaterial", ImVec2{ lineHeight, lineHeight }) or rightClicked)
        {
          ImGui::OpenPopup("CreateMaterial");
        }
        UI::DrawButtonImage(s_plusIcon, IM_COL32(160, 160, 160, 200), IM_COL32(160, 160, 160, 255), IM_COL32(160, 160, 160, 150), UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f));
        
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
              if (UI::DrawRoundButton("X", UI::Color::Muted, 20, { prevItemHeight, prevItemHeight }))
              {
                smc.materialTable->ClearMaterial((uint32_t)i);
              }

              ImGui::SameLine();
              static float lineHeight  = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;
              if (ImGui::InvisibleButton("##CreateMaterial", ImVec2{ lineHeight, lineHeight }))
              {
                if (s_defaultMaterialAsset == m_selectedMeshMaterialAsset->handle)
                {
                  // TODO: Later increase the material for now only 0 is used
                  // TODO: Might change to UUID if needed
                  std::string materialName = entity.GetName() + "#" + std::to_string((uint32_t)entity) + MaterialExtension;
                  m_selectedMeshMaterialAsset = AssetManager::CreateNewAsset<MaterialAsset>(materialName, Project::GetActive()->GetMaterialDirectory());
                  smc.materialTable->SetMaterial(0, m_selectedMeshMaterialAsset);
                }
                m_showMaterialEditor = m_showMaterialEditor ? false : true;
              }
              UI::DrawButtonImage(m_showMaterialEditor ? s_EyeIcon : s_closeEyeIcon, IM_COL32(160, 160, 160, 200), IM_COL32(160, 160, 160, 255), IM_COL32(160, 160, 160, 150), UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f));
              
              ImGui::NextColumn();
            }
            else
            {
              AssetHandle materialAssetHandle {};
              UI::PropertyAssetReferenceTarget<MaterialAsset>(label.c_str(), "Empty", materialAssetHandle, [smc, i](Ref<MaterialAsset> materialAsset){
                smc.materialTable->SetMaterial((uint32_t)i, materialAsset);
              }, settings);
            }
            ImGui::PopID();
          }
          
          UI::EndPropertyGrid();
          UI::PropertyGridHeaderEnd();
        } // property grid header
      }
    }, s_gearIcon, searchedString);
    
    DrawComponent<RigidBodyComponent>("Rigid Body", entity, [&](RigidBodyComponent& rbc)
                                      {
      UI::BeginPropertyGrid();
      
      static const char* bodyTypeStrings[] = { "Static", "Kinametic", "Dynamic"};
      int currentType = (int)rbc.bodyType;
      UI::PropertyDropdown("Body Type", bodyTypeStrings, 3, &currentType);
      rbc.bodyType = static_cast<RigidBodyComponent::BodyType>(currentType);
      
      UI::EndPropertyGrid();

      {
        UI::ScopedColor header(ImGuiCol_Header, UI::Color::BackgroundPopup);
        if (UI::PropertyGridHeader("Constraints", false, 3, 5))
        {
          UI::BeginPropertyGrid();
          
          UI::Property("Allow Sleep", rbc.allowSleep);
          UI::Property("Enable Gravity", rbc.enableGravity);
          UI::Property("Linear Damping", rbc.linearDamping);
          UI::Property("Angular Damping", rbc.angularDamping);
          
          bool lockX = rbc.angularAxisMove.x;
          if (UI::Property("Allow Angular X", lockX))
          {
            rbc.angularAxisMove.x = lockX;
          }
          bool lockY = rbc.angularAxisMove.y;
          if (UI::Property("Allow Angular Y", lockY))
          {
            rbc.angularAxisMove.y = lockY;
          }
          bool lockZ = rbc.angularAxisMove.z;
          if (UI::Property("Allow Angular Z", lockZ))
          {
            rbc.angularAxisMove.z = lockZ;
          }
          UI::EndPropertyGrid();
          UI::PropertyGridHeaderEnd();
        }
      }
    }, s_gearIcon, searchedString);
    
    DrawComponent<Box3DColliderComponent>("Box 3D Collider", entity, [&](Box3DColliderComponent& bcc)
                                          {
      UI::BeginPropertyGrid();
      
      // Physical
      UI::Property("Size", bcc.size);
      UI::Property("Position Offset", bcc.positionOffset);
      auto quaternion = glm::eulerAngles(bcc.quaternionOffset);
      UI::Property("Quaternion Offset", quaternion);
      bcc.quaternionOffset = glm::quat(quaternion);
      
      UI::EndPropertyGrid();
      
      {
        UI::ScopedColor header(ImGuiCol_Header, UI::Color::BackgroundPopup);
        if (UI::PropertyGridHeader("Material", false, 3, 5))
        {
          ImGui::PushID("Cube Material");
          UI::BeginPropertyGrid();
          UI::Property("Mass Density", bcc.massDensity, 1.0f, 0.0f, 10000.0f);
          UI::PropertySlider("Friction Coefficient", bcc.frictionCoefficient, 0.0f, 1.0f);
          UI::PropertySlider("Bounciness", bcc.bounciness, 0.0f, 1.0f);
          UI::EndPropertyGrid();
          ImGui::PopID();
          UI::PropertyGridHeaderEnd();
        }
      }
    }, s_gearIcon, searchedString);
    
    DrawComponent<SphereColliderComponent>("Sphere Collider", entity, [&](SphereColliderComponent& scc)
                                           {
      UI::BeginPropertyGrid();
      
      // Physical
      UI::Property("Radius", scc.radius);
      UI::Property("Position Offset", scc.positionOffset);
      auto quaternion = glm::eulerAngles(scc.quaternionOffset);
      UI::Property("Quaternion Offset", quaternion);
      scc.quaternionOffset = glm::quat(quaternion);
      
      UI::EndPropertyGrid();
      
      {
        UI::ScopedColor header(ImGuiCol_Header, UI::Color::BackgroundPopup);
        if (UI::PropertyGridHeader("Material", false, 3, 5))
        {
          ImGui::PushID("Sphere Material");
          UI::BeginPropertyGrid();
          UI::Property("Mass Density", scc.massDensity, 0.0f, 10000.0f);
          UI::PropertySlider("Friction Coefficient", scc.frictionCoefficient, 0.0f, 1.0f);
          UI::PropertySlider("Bounciness", scc.bounciness, 0.0f, 1.0f);
          UI::EndPropertyGrid();
          ImGui::PopID();
          UI::PropertyGridHeaderEnd();
        }
      }
    }, s_gearIcon, searchedString);
    
    DrawComponent<CapsuleColliderComponent>("Capsule Collider", entity, [&](CapsuleColliderComponent& ccc)
                                            {
      UI::BeginPropertyGrid();
      
      // Physical
      UI::Property("Radius", ccc.radius);
      UI::Property("height", ccc.height);
      UI::Property("Position Offset", ccc.positionOffset);
      auto quaternion = glm::eulerAngles(ccc.quaternionOffset);
      UI::Property("Quaternion Offset", quaternion);
      ccc.quaternionOffset = glm::quat(quaternion);
      
      UI::EndPropertyGrid();
      
      {
        UI::ScopedColor header(ImGuiCol_Header, UI::Color::BackgroundPopup);
        if (UI::PropertyGridHeader("Material", false, 3, 5))
        {
          ImGui::PushID("Capsue Material");
          UI::BeginPropertyGrid();
          UI::Property("Mass Density", ccc.massDensity, 0.0f, 10000.0f);
          UI::PropertySlider("Friction Coefficient", ccc.frictionCoefficient, 0.0f, 1.0f);
          UI::PropertySlider("Bounciness", ccc.bounciness, 0.0f, 1.0f);
          UI::EndPropertyGrid();
          ImGui::PopID();
          UI::PropertyGridHeaderEnd();
        }
      }
    }, s_gearIcon, searchedString);
    
    DrawComponent<JointComponent>("Joint", entity, [&](JointComponent& fjc)
                                  {
      UI::BeginPropertyGrid();
      
      DrawEntitySelector("Connected Entity", fjc.connectedEntity, fjc.connectedEntity);
      
#ifdef UseLocalSpace
      UI::Property("Use world Space", fjc.isWorldSpace);
#endif
      UI::Property("Allow internal collision", fjc.isCollisionEnabled);
      
      int32_t selected = static_cast<int32_t>(fjc.type);
      static std::vector<std::string> options = {"Fixed", "BallSocket", "Hinge", "Sldier"};
      if (UI::PropertyDropdown("Type", options, (uint32_t)options.size(), &selected))
      {
        fjc.type = static_cast<JointComponent::Type>(selected);
      }
      
      if (fjc.isWorldSpace)
      {
        UI::Property("World Anchor Point", fjc.worldAnchorPoint);
      }
      else
      {
        UI::Property("Local Anchor Point 1", fjc.localAnchorPoint1);
        UI::Property("Local Anchor Point 2", fjc.localAnchorPoint2);
      }
      
      if (fjc.type == IKan::JointComponent::Type::BallSocket)
      {
        UI::Property("Limit Cone", fjc.ballSocketData.coneLimit);
        UI::SetTooltip("NOTE: Cone Axis is from Body 1 COM to Anchor Point");
        if (fjc.ballSocketData.coneLimit)
        {
          auto rotation = glm::degrees(fjc.ballSocketData.coneAngle);
          if (UI::Property("Cone Angle", rotation))
          {
            fjc.ballSocketData.coneAngle = glm::radians(rotation);
          }
        }
      }
      else if (fjc.type == IKan::JointComponent::Type::Hinge)
      {
        if (fjc.isWorldSpace)
        {
          UI::Property("World Anchor Axis", fjc.hingeData.worldAxis, 1.0f, 0.0f, 1.0f);
        }
        else
        {
          UI::Property("Local Anchor Axis 1", fjc.hingeData.localAxis1, 1.0f, 0.0f, 1.0f);
          UI::Property("Local Anchor Axis 2", fjc.hingeData.localAxis2, 1.0f, 0.0f, 1.0f);
        }
        
        UI::Property("Limit", fjc.hingeData.limit);
        UI::Property("Motor", fjc.hingeData.motor);
        if (fjc.hingeData.limit)
        {
          auto rotationMin = glm::degrees(fjc.hingeData.initMinAngleLimit);
          if (UI::Property("Init Min Angle", rotationMin))
          {
            fjc.hingeData.initMinAngleLimit = glm::radians(rotationMin);
          }
          
          auto rotationMax = glm::degrees(fjc.hingeData.initMaxAngleLimit);
          if (UI::Property("Init Max Angle", rotationMax))
          {
            fjc.hingeData.initMaxAngleLimit = glm::radians(rotationMax);
          }
        }
        
        if (fjc.hingeData.motor)
        {
          auto rotation = glm::degrees(fjc.hingeData.initMotorSpeed);
          if (UI::Property("Init Speed", rotation))
          {
            fjc.hingeData.initMotorSpeed = glm::radians(rotation);
          }
          UI::SetTooltip("Degree per seconds");
          
          UI::Property("Max Torque", fjc.hingeData.initMaxMotorTorque);
          UI::SetTooltip("Newton-Meter");
        }
      }
      
      else if (fjc.type == IKan::JointComponent::Type::Slider)
      {
        if (fjc.isWorldSpace)
        {
          UI::Property("World Anchor Axis", fjc.sliderData.worldAxis, 1.0f, 0.0f, 1.0f);
        }
        else
        {
          UI::Property("Local Anchor Axis 1", fjc.sliderData.localAxis1, 1.0f, 0.0f, 1.0f);
        }
        
        UI::Property("Limit", fjc.sliderData.limit);
        UI::Property("Motor", fjc.sliderData.motor);
        if (fjc.sliderData.limit)
        {
          UI::Property("Init Min Translation", fjc.sliderData.initMinTransLimit);
          UI::Property("Init Min Translation", fjc.sliderData.initMaxTransLimit);
        }
        
        if (fjc.sliderData.motor)
        {
          UI::Property("Init Speed", fjc.sliderData.initMotorSpeed);
          UI::SetTooltip("Meter per seconds");
          UI::Property("Max Force", fjc.sliderData.initMaxMotorForce);
          UI::SetTooltip("Newton");
        }
      }
      UI::EndPropertyGrid();
    }, s_gearIcon, searchedString);
  }
  
  void SceneHierarchyPanel::AddComponentPopup()
  {
    if (UI::BeginPopup("AddComponentPanel"))
    {
      ImGui::Separator();
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
      
      if (!m_selectionContext.At(0).HasComponent<CameraComponent>())
      {
        if (ImGui::MenuItem("Camera"))
        {
          [[maybe_unused]] auto& cameraComp = m_selectionContext.At(0).AddComponent<CameraComponent>();
          ImGui::CloseCurrentPopup();
        }
      }

      if (!m_selectionContext.At(0).HasComponent<RigidBodyComponent>())
      {
        if (ImGui::MenuItem("Rigid Body"))
        {
          [[maybe_unused]] auto& rigidBodyComp = m_selectionContext.At(0).AddComponent<RigidBodyComponent>();
          ImGui::CloseCurrentPopup();
        }
      }
      if (!m_selectionContext.At(0).HasComponent<Box3DColliderComponent>())
      {
        if (ImGui::MenuItem("Box 3D Collider"))
        {
          if (!m_selectionContext.At(0).HasComponent<RigidBodyComponent>())
          {
            m_selectionContext.At(0).AddComponent<RigidBodyComponent>();
          }

          [[maybe_unused]] auto& box3DColliderComp = m_selectionContext.At(0).AddComponent<Box3DColliderComponent>();
          ImGui::CloseCurrentPopup();
        }
      }
      if (!m_selectionContext.At(0).HasComponent<SphereColliderComponent>())
      {
        if (ImGui::MenuItem("Sphere Collider"))
        {
          if (!m_selectionContext.At(0).HasComponent<RigidBodyComponent>())
          {
            m_selectionContext.At(0).AddComponent<RigidBodyComponent>();
          }
          [[maybe_unused]] auto& sphereColliderComp = m_selectionContext.At(0).AddComponent<SphereColliderComponent>();
          ImGui::CloseCurrentPopup();
        }
      }
      if (!m_selectionContext.At(0).HasComponent<CapsuleColliderComponent>())
      {
        if (ImGui::MenuItem("Capsule Collider"))
        {
          if (!m_selectionContext.At(0).HasComponent<RigidBodyComponent>())
          {
            m_selectionContext.At(0).AddComponent<RigidBodyComponent>();
          }
          [[maybe_unused]] auto& capsuleColliderComp = m_selectionContext.At(0).AddComponent<CapsuleColliderComponent>();
          ImGui::CloseCurrentPopup();
        }
      }
      if (!m_selectionContext.At(0).HasComponent<JointComponent>())
      {
        if (ImGui::MenuItem("Joint"))
        {
          if (!m_selectionContext.At(0).HasComponent<RigidBodyComponent>())
          {
            m_selectionContext.At(0).AddComponent<RigidBodyComponent>();
          }
          [[maybe_unused]] auto& jointComp = m_selectionContext.At(0).AddComponent<JointComponent>();
          jointComp.worldAnchorPoint = m_selectionContext.At(0).GetTransform().Position();
          ImGui::CloseCurrentPopup();
        }
      }

      UI::EndPopup();
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
        UI::ScopedColorStack entitySelection(ImGuiCol_Header, UI::Color::GroupHeader, ImGuiCol_HeaderHovered,
                                             UI::Color::GroupHeader, ImGuiCol_HeaderActive, UI::Color::GroupHeader);
        
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
} // namespace Kreator
