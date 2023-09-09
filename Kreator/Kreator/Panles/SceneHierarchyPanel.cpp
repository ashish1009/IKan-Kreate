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
  template<typename T, typename UIFunction>
  static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, const Ref<Image>& settingsIcon,
                            bool canBeRemoved = true)
  {
    if (entity.HasComponent<T>())
    {
      // NOTE(Peter):
      //  This fixes an issue where the first "+" button would display the "Remove" buttons for ALL components on an Entity.
      //  This is due to ImGui::TreeNodeEx only pushing the id for it's children if it's actually open
      ImGui::PushID((void*)typeid(T).hash_code());
      auto& component = entity.GetComponent<T>();
      ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
      
      bool open = Kreator_UI::PropertyGridHeader(name);
      bool right_clicked  = ImGui::IsItemClicked(ImGuiMouseButton_Right);
      float lineHeight  = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;
      
      bool resetValues = false;
      bool removeComponent = false;
      
      ImGui::SameLine(contentRegionAvailable.x - lineHeight);
      if (ImGui::InvisibleButton("##options", ImVec2{ lineHeight, lineHeight }) || right_clicked)
      {
        ImGui::OpenPopup("ComponentSettings");
      }
      UI::DrawButtonImage(settingsIcon, IM_COL32(160, 160, 160, 200),
                          IM_COL32(160, 160, 160, 255),
                          IM_COL32(160, 160, 160, 150),
                          UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f));
      
      if (UI::BeginPopup("ComponentSettings"))
      {
        if (ImGui::MenuItem("Reset"))
          resetValues = true;
        
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
        ImGui::TreePop();
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
  
  static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
  {
    bool modified = false;
    
    UI::ScopedStyle framePdding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2));
    UI::ScopedStyle framerounding(ImGuiStyleVar_FrameRounding, 15);
    
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
            
      auto drawControl = [&] (const std::string& label, float& value, const ImVec4& colourN,
                              const ImVec4& colourH,
                              const ImVec4& colourP) {
        {
          UI::ScopedStyle buttonFrame(ImGuiStyleVar_FramePadding, ImVec2(framePadding, 0.0f));
          UI::ScopedStyle buttonRounding(ImGuiStyleVar_FrameRounding, 1.0f);
          UI::ScopedColorStack buttonColours(ImGuiCol_Button, colourN,
                                             ImGuiCol_ButtonHovered, colourH,
                                             ImGuiCol_ButtonActive, colourP);
          
          UI::ScopedFont boldFont(Kreator_UI::GetBoldFont());

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
        modified |= ImGui::DragFloat(("##" + label).c_str(), &value, 0.1f, 0.0f, 0.0f, "%.2f");
        
        if (!UI::IsItemDisabled())
        {
          UI::DrawItemActivityOutline(2.0f, true, Kreator_UI::Color::Accent);
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
    m_selectionContext.Clear();
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
          m_context->UnparentEntity(entity, false);
        }
        
        ImGui::EndDragDropTarget();
      }

      ImGui::End();
      
      {
        UI::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(2.0, 4.0f));
        ImGui::Begin("Properties");
      }
      
      if (m_selectionContext.Size() == 1)
      {
        DrawComponents(m_selectionContext.At(0));
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
      | ImGuiTableFlags_RowBg
      | ImGuiTableFlags_Sortable;
      
      const int numColumns = 3;
      if (ImGui::BeginTable("##SceneHierarchy-Table", numColumns, tableFlags, ImVec2(ImGui::GetContentRegionAvail())))
      {
        ImGui::TableSetupColumn("Label");
        ImGui::TableSetupColumn(" ");
        ImGui::TableSetupColumn(" ");
        
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
    float roundingVal = 15.0f;
    UI::ScopedStyle rounding (ImGuiStyleVar_FrameRounding, roundingVal);
    
    ImGui::AlignTextToFramePadding();
    auto ID = entity.GetComponent<IDComponent>().ID;
    ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
    UI::ShiftCursor(4.0f, 4.0f);
    
    bool isHoveringID = false;
    if (entity.HasComponent<TagComponent>())
    {
      const float iconOffset = 3.0f;
      UI::ShiftCursor(4.0f, iconOffset);
      UI::Image(s_pencilIcon, ImVec2(s_pencilIcon->GetWidth(), s_pencilIcon->GetHeight()),
                ImVec2(1.0f, 0.0f), ImVec2(0.0f, 1.0f), ImColor(128, 128, 128, 255).Value);
      
      ImGui::SameLine(0.0f, 4.0f);
      UI::ShiftCursorY(-iconOffset);
      
      auto& tag = entity.GetComponent<TagComponent>().tag;
      char buffer[256];
      memset(buffer, 0, 256);
      memcpy(buffer, tag.c_str(), tag.length());
      ImGui::PushItemWidth(contentRegionAvailable.x * 0.4f);
      
      UI::ScopedStyle frameBorder(ImGuiStyleVar_FrameBorderSize, 0.0f);
      UI::ScopedColor frameColour(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
      UI::ScopedFont boldFont(Kreator_UI::GetBoldFont());

      if (ImGui::InputText("##Tag", buffer, 256))
      {
        tag = std::string(buffer);
      }
      UI::DrawItemActivityOutline(roundingVal, false, Kreator_UI::Color::Accent);
      
      isHoveringID = ImGui::IsItemHovered();
      
      ImGui::PopItemWidth();
    }
    
    // ID
    if (isHoveringID)
    {
      ImGui::SameLine();
      ImGui::TextDisabled("%llx", (uint64_t)ID);
    }
    
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 textSize = ImGui::CalcTextSize(" ADD        ");
    textSize.x += GImGui->Style.FramePadding.x * 2.0f;
    {
      UI::ScopedColorStack addCompButtonColours(ImGuiCol_Button, IM_COL32(70, 70, 70, 200),
                                                ImGuiCol_ButtonHovered, IM_COL32(70, 70, 70, 255),
                                                ImGuiCol_ButtonActive, IM_COL32(70, 70, 70, 150));
      
      ImGui::SameLine(contentRegionAvailable.x - (textSize.x + GImGui->Style.FramePadding.x));
      if (ImGui::Button(" ADD       ", ImVec2(textSize.x + 4.0f, lineHeight + 2.0f)))
      {
        ImGui::OpenPopup("AddComponentPanel");
      }
      
      const float pad = 4.0f;
      const float iconWidth = ImGui::GetFrameHeight() - pad * 2.0f;
      const float iconHeight = iconWidth;
      ImVec2 iconPos = ImGui::GetItemRectMax();
      iconPos.x -= iconWidth + pad;
      iconPos.y -= iconHeight + pad;
      ImGui::SetCursorScreenPos(iconPos);
      UI::ShiftCursor(-5.0f, -1.0f);
      
      UI::Image(s_plusIcon, ImVec2(iconWidth, iconHeight),
                ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
                ImColor(160, 160, 160, 255).Value);
    }
    
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    
    AddComponentPopup();
    ImGui::Separator();
    
    DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component)
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
      DrawVec3Control("Scale", scale);
      component.UpdateScale(scale);
      
      ImGui::EndTable();
      
      UI::ShiftCursorY(-8.0f);
      UI::ShiftCursorY(18.0f);
    }, s_gearIcon, false);
    
    DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& cc)
                                   {
      Kreator_UI::BeginPropertyGrid();
      UI::ScopedStyle framePdding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2));
      
      // Projection Type
      const char* projTypeStrings[] = { "Perspective", "Orthographic" };
      int currentProj = (int)cc.camera.GetProjectionType();
      if (Kreator_UI::PropertyDropdown("Projection", projTypeStrings, 2, &currentProj))
      {
        cc.camera.SetProjectionType((SceneCamera::ProjectionType)currentProj);
      }
      
      // Perspective parameters
      if (cc.camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
      {
        float verticalFOV = cc.camera.GetDegPerspectiveVerticalFOV();
        if (Kreator_UI::Property("Vertical FOV", verticalFOV))
          cc.camera.SetDegPerspectiveVerticalFOV(verticalFOV);
        
        float nearClip = cc.camera.GetPerspectiveNearClip();
        if (Kreator_UI::Property("Near Clip", nearClip))
          cc.camera.SetPerspectiveNearClip(nearClip);
        float farClip = cc.camera.GetPerspectiveFarClip();
        if (Kreator_UI::Property("Far Clip", farClip))
          cc.camera.SetPerspectiveFarClip(farClip);
      }
      
      // Orthographic parameters
      else if (cc.camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
      {
        float orthoSize = cc.camera.GetOrthographicSize();
        if (Kreator_UI::Property("Size", orthoSize, 1.0f))
          cc.camera.SetOrthographicSize(orthoSize);
        
        float nearClip = cc.camera.GetOrthographicNearClip();
        if (Kreator_UI::Property("Near Clip", nearClip, 0.1f, -1.0f, 0.0f))
          cc.camera.SetOrthographicNearClip(nearClip);
        float farClip = cc.camera.GetOrthographicFarClip();
        if (Kreator_UI::Property("Far Clip", farClip, 0.1f, 0.0f, 1.0f))
          cc.camera.SetOrthographicFarClip(farClip);
      }
      
      Kreator_UI::Property("Main Camera", cc.primary);
      Kreator_UI::EndPropertyGrid();
    }, s_gearIcon);
    
    DrawComponent<TextComponent>("Text", entity, [](TextComponent& tc)
                                 {
      Kreator_UI::BeginPropertyGrid();
      Kreator_UI::PropertyMultiline("Text String", tc.textString);
      
      Kreator_UI::PropertyAssetReferenceSettings settings;
      bool customFont = tc.assetHandle != Font::GetDefaultFont()->handle;
      if (customFont)
      {
        settings.advanceToNextColumn = false;
        settings.widthOffset = ImGui::GetStyle().ItemSpacing.x + 28.0f;
      }
      Kreator_UI::PropertyAssetReference<Font>("Font", tc.assetHandle, nullptr, settings);
      if (customFont)
      {
        ImGui::SameLine();
        float prevItemHeight = ImGui::GetItemRectSize().y;
        if (ImGui::Button("X", { prevItemHeight, prevItemHeight }))
        {
          tc.assetHandle = Font::GetDefaultFont()->handle;
        }
        ImGui::NextColumn();
      }
      
      Kreator_UI::PropertyColor("Color", tc.color);
      Kreator_UI::EndPropertyGrid();
    }, s_gearIcon);
    
    DrawComponent<QuadComponent>("Quad", entity, [](QuadComponent& qc)
                                 {
      Kreator_UI::BeginPropertyGrid();
      
      Kreator_UI::PropertyAssetReferenceSettings settings;
      Kreator_UI::PropertyAssetReference<Image>("Texture", qc.texture, nullptr, settings);
      
      Kreator_UI::PropertyColor("Color", qc.color);
      Kreator_UI::EndPropertyGrid();
    }, s_gearIcon);
    
    DrawComponent<CircleComponent>("Circle", entity, [](CircleComponent& cc)
                                   {
      Kreator_UI::BeginPropertyGrid();
      
      Kreator_UI::PropertyAssetReferenceSettings settings;
      Kreator_UI::PropertyAssetReference<Image>("Texture", cc.texture, nullptr, settings);
      
      Kreator_UI::PropertyColor("Color", cc.color);
      Kreator_UI::Property("Thickness", cc.thickness, 0.1, 0.0, 1.0);
      Kreator_UI::Property("Fade", cc.fade, 0.1, 0.0, 5.0);
      Kreator_UI::EndPropertyGrid();
    }, s_gearIcon);
    
    DrawComponent<StaticMeshComponent>("Static Mesh", entity, [&](StaticMeshComponent& smc)
                                       {
      Ref<MeshSource> mesh = AssetManager::GetAsset<MeshSource>(smc.staticMesh);
      
      Kreator_UI::BeginPropertyGrid();
      Kreator_UI::PropertyAssetReferenceSettings settings;
      Kreator_UI::PropertyAssetReference<MeshSource>("Mesh", smc.staticMesh, nullptr, settings);
      Kreator_UI::EndPropertyGrid();
    }, s_gearIcon);
    
    DrawComponent<RigidBodyComponent>("Rigid Body", entity, [&](RigidBodyComponent& rbc)
                                      {
      Kreator_UI::BeginPropertyGrid();
      
      static const char* bodyTypeStrings[] = { "Static", "Kinametic", "Dynamic"};
      int currentType = (int)rbc.bodyType;
      Kreator_UI::PropertyDropdown("Body Type", bodyTypeStrings, 3, &currentType);
      rbc.bodyType = static_cast<RigidBodyComponent::BodyType>(currentType);
      
      Kreator_UI::Property("Allow Sleep", rbc.allowSleep);
      Kreator_UI::Property("Linear Damping", rbc.liniarDamping);
      Kreator_UI::Property("Angular Damping", rbc.angularDamping);

      Kreator_UI::EndPropertyGrid();
    }, s_gearIcon);
    
    DrawComponent<Box3DColliderComponent>("Box 3D Collider", entity, [&](Box3DColliderComponent& bcc)
                                          {
      Kreator_UI::BeginPropertyGrid();
      
      // Physical
      Kreator_UI::Property("Size", bcc.size);
      Kreator_UI::Property("Position Offset", bcc.positionOffset);
      auto quaternion = glm::eulerAngles(bcc.quaternionOffset);
      Kreator_UI::Property("Quaternion Offset", quaternion);
      bcc.quaternionOffset = glm::quat(quaternion);
      
      // Material
      Kreator_UI::Property("Friction Coefficient", bcc.frictionCoefficient, 0.01, 0.0f, 1.0f);
      Kreator_UI::Property("Mass Density", bcc.massDensity, 0.1f, 0.0f, 10000.0f);
      Kreator_UI::Property("Bounciness", bcc.bounciness, 0.01f, 0.0f, 1.0f);
      
      Kreator_UI::EndPropertyGrid();
    }, s_gearIcon);
    
    DrawComponent<SphereColliderComponent>("Sphere Collider", entity, [&](SphereColliderComponent& scc)
                                           {
      Kreator_UI::BeginPropertyGrid();
      
      // Physical
      Kreator_UI::Property("Radius", scc.radius);
      Kreator_UI::Property("Position Offset", scc.positionOffset);
      auto quaternion = glm::eulerAngles(scc.quaternionOffset);
      Kreator_UI::Property("Quaternion Offset", quaternion);
      scc.quaternionOffset = glm::quat(quaternion);
      
      // Material
      Kreator_UI::Property("Friction Coefficient", scc.frictionCoefficient, 0.01, 0.0f, 1.0f);
      Kreator_UI::Property("Mass Density", scc.massDensity, 0.1f, 0.0f, 10000.0f);
      Kreator_UI::Property("Bounciness", scc.bounciness, 0.01f, 0.0f, 1.0f);
      Kreator_UI::EndPropertyGrid();
    }, s_gearIcon);
    
    DrawComponent<CapsuleColliderComponent>("Capsule Collider", entity, [&](CapsuleColliderComponent& ccc)
                                            {
      Kreator_UI::BeginPropertyGrid();
      
      // Physical
      Kreator_UI::Property("Radius", ccc.radius);
      Kreator_UI::Property("height", ccc.height);
      Kreator_UI::Property("Position Offset", ccc.positionOffset);
      auto quaternion = glm::eulerAngles(ccc.quaternionOffset);
      Kreator_UI::Property("Quaternion Offset", quaternion);
      ccc.quaternionOffset = glm::quat(quaternion);
      
      // Material
      Kreator_UI::Property("Friction Coefficient", ccc.frictionCoefficient, 0.01, 0.0f, 1.0f);
      Kreator_UI::Property("Mass Density", ccc.massDensity, 0.1f, 0.0f, 10000.0f);
      Kreator_UI::Property("Bounciness", ccc.bounciness, 0.01f, 0.0f, 1.0f);
      Kreator_UI::EndPropertyGrid();
    }, s_gearIcon);

    DrawComponent<JointComponent>("Joint", entity, [&](JointComponent& fjc)
                                            {
      Kreator_UI::BeginPropertyGrid();
      
      
      Entity targetEntity = m_context->TryGetEntityWithUUID(fjc.connectedEntity);
      if (Kreator_UI::PropertyEntityReference("Connected Entity", targetEntity))
      {
        fjc.connectedEntity = targetEntity.GetUUID();
      }

      Kreator_UI::Property("Use world Space", fjc.isWorldSpace);

      int32_t selected = static_cast<int32_t>(fjc.type);
      if (Kreator_UI::PropertyDropdown("Type",
                                       {"Fixed", "BallSocket", "Hinge"},
                                       3, &selected))
      {
        fjc.type = static_cast<JointComponent::Type>(selected);
      }

      if (fjc.isWorldSpace)
      {
        Kreator_UI::Property("World Anchor Point", fjc.worldAnchorPoint);
      }
      else
      {
        Kreator_UI::Property("Local Anchor Point 1", fjc.localAnchorPoint1);
        Kreator_UI::Property("Local Anchor Point 2", fjc.localAnchorPoint2);
      }
      
      if (fjc.type == IKan::JointComponent::Type::BallSocket)
      {
        Kreator_UI::Property("Limit Cone", fjc.ballSocketData.coneLimit);
        auto rotation = glm::degrees(fjc.ballSocketData.coneAngle);
        if (Kreator_UI::Property("Cone Angle", rotation))
        {
          fjc.ballSocketData.coneAngle = glm::radians(rotation);
        }
      }
      else if (fjc.type == IKan::JointComponent::Type::Hinge)
      {
        if (fjc.isWorldSpace)
        {
          Kreator_UI::Property("World Anchor Axis", fjc.hingeData.worldAxis);
        }
        else
        {
          Kreator_UI::Property("Local Anchor Axis 1", fjc.hingeData.localAxis1);
          Kreator_UI::Property("Local Anchor Axis 2", fjc.hingeData.localAxis2);
        }
        
        auto rotationMin = glm::degrees(fjc.hingeData.initMinAngleLimit);
        if (Kreator_UI::Property("Init Min Angle", rotationMin))
        {
          fjc.hingeData.initMinAngleLimit = glm::radians(rotationMin);
        }

        auto rotationMax = glm::degrees(fjc.hingeData.initMaxAngleLimit);
        if (Kreator_UI::Property("Init Max Angle", rotationMax))
        {
          fjc.hingeData.initMaxAngleLimit = glm::radians(rotationMax);
        }
        
        Kreator_UI::Property("Init Speed", fjc.hingeData.initMotorSpeed);
        Kreator_UI::Property("Init Torque", fjc.hingeData.initMaxMotorTorque);
      }
      Kreator_UI::EndPropertyGrid();
    }, s_gearIcon);
  }
  
  void SceneHierarchyPanel::AddComponentPopup()
  {
    UI::ScopedFont boldFont(Kreator_UI::GetBoldFont());

    if (UI::BeginPopup("AddComponentPanel"))
    {
      if (!m_selectionContext.At(0).HasComponent<CameraComponent>())
      {
        if (ImGui::MenuItem("Camera"))
        {
          m_selectionContext.At(0).AddComponent<CameraComponent>();
          ImGui::CloseCurrentPopup();
        }
      }
      if (!m_selectionContext.At(0).HasComponent<QuadComponent>())
      {
        if (ImGui::MenuItem("Quad"))
        {
          m_selectionContext.At(0).AddComponent<QuadComponent>();
          ImGui::CloseCurrentPopup();
        }
      }
      if (!m_selectionContext.At(0).HasComponent<CircleComponent>())
      {
        if (ImGui::MenuItem("Circle"))
        {
          m_selectionContext.At(0).AddComponent<CircleComponent>();
          ImGui::CloseCurrentPopup();
        }
      }
      if (!m_selectionContext.At(0).HasComponent<TextComponent>())
      {
        if (ImGui::MenuItem("Text"))
        {
          auto& textComp = m_selectionContext.At(0).AddComponent<TextComponent>();
          textComp.assetHandle = Font::GetDefaultFont()->handle;
          ImGui::CloseCurrentPopup();
        }
      }
      if (!m_selectionContext.At(0).HasComponent<StaticMeshComponent>())
      {
        if (ImGui::MenuItem("Mesh"))
        {
          auto& meshComp = m_selectionContext.At(0).AddComponent<StaticMeshComponent>();
          meshComp.staticMesh = 0;
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
          [[maybe_unused]] auto& box3DColliderComp = m_selectionContext.At(0).AddComponent<Box3DColliderComponent>();
          ImGui::CloseCurrentPopup();
        }
      }
      if (!m_selectionContext.At(0).HasComponent<SphereColliderComponent>())
      {
        if (ImGui::MenuItem("Sphere Collider"))
        {
          [[maybe_unused]] auto& sphereColliderComp = m_selectionContext.At(0).AddComponent<SphereColliderComponent>();
          ImGui::CloseCurrentPopup();
        }
      }
      if (!m_selectionContext.At(0).HasComponent<CapsuleColliderComponent>())
      {
        if (ImGui::MenuItem("Capsule Collider"))
        {
          [[maybe_unused]] auto& meshColliderComp = m_selectionContext.At(0).AddComponent<CapsuleColliderComponent>();
          ImGui::CloseCurrentPopup();
        }
      }
      if (!m_selectionContext.At(0).HasComponent<JointComponent>())
      {
        if (ImGui::MenuItem("Joint"))
        {
          [[maybe_unused]] auto& meshColliderComp = m_selectionContext.At(0).AddComponent<JointComponent>();
          ImGui::CloseCurrentPopup();
        }
      }
      UI::EndPopup();
    }
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
    
    const bool isSelected = m_selectionContext.Find(entity);
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
        
        if (m_selectionContext.Size() == 1)
        {
          ImGui::Separator();
          if (ImGui::MenuItem("Delete"))
          {
            entityDeleted = true;
          }
          if (ImGui::MenuItem("Duplicate"))
          {
            m_context->DuplicateEntity(m_selectionContext.At(0));
          }
        }
      }
      ImGui::EndPopup();
    }
    
    // <> column -------------------------------------------------------------------------------------------------
    ImGui::TableNextColumn();

    // <> column -------------------------------------------------------------------------------------------
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
      
      auto beginMnuForDefaultMesh = [this, menuForDefaultMesh](Entity& newEntity, const std::string& name) {
        if (ImGui::BeginMenu(name.c_str()))
        {
          menuForDefaultMesh(newEntity, name);
          std::string title = "Rigid" + name;
          if (ImGui::MenuItem(title.c_str()))
          {
            newEntity = m_context->CreateEntity(name);
            std::string file = Project::GetActive()->GetMeshPath("Default/");
            file += name;
            file += ".fbx";
            auto meshSourceHandle = AssetManager::CreateMemoryOnlyAssetWithFile<MeshSource>(file, file);
            newEntity.AddComponent<StaticMeshComponent>(meshSourceHandle);

            newEntity.AddComponent<RigidBodyComponent>();
            if (name == "Cube")
            {
              newEntity.AddComponent<Box3DColliderComponent>();
            }
            else if (name == "Capsule")
            {
              newEntity.AddComponent<CapsuleColliderComponent>();
            }
            else if (name == "Sphere")
            {
              newEntity.AddComponent<SphereColliderComponent>();
            }
          }
          ImGui::EndMenu();
        }
      };
      
      if (ImGui::MenuItem("Empty Mesh"))
      {
        newEntity = m_context->CreateEntity("Mesh");
        newEntity.AddComponent<StaticMeshComponent>();
      }
      
      beginMnuForDefaultMesh(newEntity, "Cube");
      beginMnuForDefaultMesh(newEntity, "Capsule");
      beginMnuForDefaultMesh(newEntity, "Sphere");
      menuForDefaultMesh(newEntity, "Cone");
      menuForDefaultMesh(newEntity, "Cylinder");
      menuForDefaultMesh(newEntity, "Plane");
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
    if (m_selectionContext.Find(entity))
    {
      m_selectionContext.Erase(entity);
    }
  }

  void SceneHierarchyPanel::SetSelectedEntity(Entity entity, bool multipleSelection)
  {
    if (!multipleSelection)
    {
      m_selectionContext.Clear();
    }
    
    if (!entity)
    {
      return;
    }

    m_selectionContext.PushBack(entity);
    if (m_selectionChangedCallback)
    {
      m_selectionChangedCallback(m_selectionContext);
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

  void SceneHierarchyPanel::OnEntityDestroyed(Entity entity)
  {
    m_entityDeletedCallback(m_selectionContext);

    if (m_selectionContext.Find(entity))
    {
      m_selectionContext.Erase(entity);
    }
    
    m_context->DestroyEntity(entity);
  }

} // namespace Kreator
