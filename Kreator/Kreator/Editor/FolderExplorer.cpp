//
//  FolderExplorer.cpp
//  Kreator
//
//  Created by Ashish . on 03/08/23.
//

#include "FolderExplorer.hpp"
#include "RendererLayer.hpp"

namespace Kreator
{
#define MAX_PATH_LENGTH 255

  using namespace IKan;
  
  enum class PopupType
  {
    Open, Select, Save
  };

  struct Data
  {
    bool popup = false;
    bool* lastPopupFlag;
    char* openPathBuffer;
    PopupType popupType;
    std::filesystem::path currentPath;
    Ref<Image> shadowwTexture;
    Ref<Image> backButton;
    Ref<Image> folderIcon, fileTex;

    Data()
    {
      openPathBuffer = iknew char[MAX_PATH_LENGTH];
    }
    
    ~Data()
    {
      ikdelete [] openPathBuffer;
    }

  };
  Scope<Data> s_fileExplorerData;

  void FolderExplorer::Initialize()
  {
    IK_LOG_TRACE("FolderExplorer", "Initialising the FolderExplorer textures");
    s_fileExplorerData = CreateScope<Data>();
    s_fileExplorerData->shadowwTexture = Image::Create(RendererLayer::GetClientFilePath() / "Resources/Textures/Icons/ShadowLineTop.png");
    s_fileExplorerData->backButton = Image::Create(RendererLayer::GetClientFilePath() / "Resources/Textures/Icons/Back.png");
    s_fileExplorerData->folderIcon = Image::Create(RendererLayer::GetClientFilePath() / "Resources/Textures/Icons/Folder.png");
    s_fileExplorerData->fileTex = Image::Create(RendererLayer::GetClientFilePath() / "Resources/Textures/Icons/File.png");
  }
  
  void FolderExplorer::Shutdown()
  {
    IK_LOG_WARN("FolderExplorer", "Shutting Down the FolderExplorer textures");
    s_fileExplorerData.reset();
  }

  std::filesystem::path FolderExplorer::Explore()
  {
    std::filesystem::path returnPath = "";
    
    memcpy(s_fileExplorerData->openPathBuffer, s_fileExplorerData->currentPath.c_str(), s_fileExplorerData->currentPath.string().size());

    if (s_fileExplorerData->popup)
    {
      ImGui::OpenPopup("Folder Explorer");
      s_fileExplorerData->popup = false;
    }
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2{ 700, 325 });

    if (ImGui::BeginPopupModal("Folder Explorer", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove
                               | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground))
    {
      const float topBarHeight = 40;
      auto* drawList = ImGui::GetWindowDrawList();

      // Draw the address Bar -------------------------------------------------------------
      {
        UI::ScopedStyle windowPadding (ImGuiStyleVar_WindowPadding, ImVec2(20, 20));

        const ImVec2 addresBarMin = ImGui::GetCursorScreenPos();
        const ImVec2 addresBarMax =
        {
          addresBarMin.x + ImGui::GetContentRegionAvail().x,
          addresBarMin.y + topBarHeight
        };

        drawList->AddRectFilled(addresBarMin, addresBarMax, UI::Theme::Color::BackgroundPopup);

        UI::ShiftCursorX(ImGui::GetCurrentWindow()->WindowPadding.x);
        UI::ShiftCursorY(ImGui::GetCurrentWindow()->WindowPadding.y);

        // Render Icon Buttons -----------------------------------------------------------
        {
          UI::ScopedColor button(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
          UI::ScopedColor buttonhovered(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
          UI::ScopedColor buttonavtive(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
          auto navigationButton = [](const char* labelId, const Ref<Image>& icon)
          {
            const ImU32 buttonCol = UI::Theme::Color::BackgroundDark;
            const ImU32 buttonColP = UI::ColorWithMultipliedValue(UI::Theme::Color::BackgroundDark, 0.8f);
            UI::ScopedColorStack buttonColors(ImGuiCol_Button, buttonCol,
                                              ImGuiCol_ButtonHovered, buttonCol,
                                              ImGuiCol_ButtonActive, buttonColP);

            const float iconSize = 20.0f;
            const float iconPadding = 3.0f;
            const bool clicked = ImGui::Button(labelId, ImVec2(iconSize, iconSize));
            UI::DrawButtonImage(icon, UI::ColorWithMultipliedValue(Kreator_UI::Color::NiceBlue, 0.8),
                                UI::ColorWithMultipliedValue(Kreator_UI::Color::NiceBlue, 1.2f),
                                UI::ColorWithMultipliedValue(Kreator_UI::Color::NiceBlue, 0.5f),
                                UI::RectExpanded(UI::GetItemRect(), -iconPadding, -iconPadding));

            return clicked;
          };

          if (navigationButton("##back", s_fileExplorerData->backButton))
          {
//            if (s_fileExplorerData->currentPath != s_fileExplorerData->parentPath)
            {
              s_fileExplorerData->currentPath = s_fileExplorerData->currentPath.parent_path();
              memset(s_fileExplorerData->openPathBuffer, 0, MAX_PATH_LENGTH);
            }
          }
          UI::SetTooltip("Previous directory");
        }

        // Draw Address bar --------------------------------------------------------------
        ImGui::SameLine();
        float addressBarWidth = s_fileExplorerData->popupType == PopupType::Save ? 330 : 550;
        ImGui::SetNextItemWidth(addressBarWidth);
        ImGui::InputTextWithHint("##new_project_location", "Project Location",
                                 s_fileExplorerData->openPathBuffer, MAX_PATH_LENGTH, ImGuiInputTextFlags_ReadOnly);

        // Save/Open?Select Close buttons ------------------------------------------------
        {
          ImGui::PushFont(Kreator_UI::GetBoldFont());

          ImGui::SameLine();
          std::string buttonTitle = "";
          switch(s_fileExplorerData->popupType)
          {
            case PopupType::Open :   buttonTitle = "Open";    break;
            case PopupType::Select : buttonTitle = "Select";  break;
            case PopupType::Save :   buttonTitle = "Save";    break;
            default:
              IK_ASSERT(false);
          }

          if ((UI::DrawRoundButton(buttonTitle.c_str(), Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 20)) or
              (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter)))
          {

          }

          ImGui::SameLine();
          if ((UI::DrawRoundButton("Close", Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 20)) or
              (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape)))
          {
            ImGui::CloseCurrentPopup();
            if (s_fileExplorerData->lastPopupFlag)
            {
              *s_fileExplorerData->lastPopupFlag = true;
            }
          }

          ImGui::PopFont();
        }
      }

      // Folder Explorer Main -----------------------------------------------------------
      {
        UI::PushID();
        UI::ScopedColor bgCol(ImGuiCol_ChildBg, UI::Theme::Color::BackgroundPopup);
        UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(3, 8));
        if (ImGui::BeginTable(UI::GenerateID(), 1 /* Num Columns */, ImGuiTableFlags_SizingFixedFit, ImVec2(0.0f, 0.0f)))
        {
          ImGui::TableSetupColumn("Open Folder", ImGuiTableColumnFlags_WidthStretch, 0);
          ImGui::TableNextRow();
          
          // Content Outliner
          ImGui::TableSetColumnIndex(0);
          ImGui::BeginChild("##Open_Folder", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - 110));
          {
            {
              UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

              // Draw top shadow-----------------------------------------------------------------
              ImRect shadowRect =
              {
                {
                  ImGui::GetCursorScreenPos().x,
                  ImGui::GetCursorScreenPos().y + 2
                },
                { ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
                  ImGui::GetCursorScreenPos().y + 20
                }
              };
              
              ImRect windowRect = UI::RectExpanded(shadowRect, 0.0f, 0.0f);
              ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
              UI::DrawShadowInner(s_fileExplorerData->shadowwTexture, 10.0f, windowRect, 1.0f, windowRect.GetWidth() / 8,
                                  false, false, true, false);

              for (auto entry : std::filesystem::directory_iterator(s_fileExplorerData->currentPath))
              {
                if (entry.path().filename() == ".DS_Store")
                {
                  continue;
                }
                
                auto directoryName = std::filesystem::relative(entry.path(), s_fileExplorerData->currentPath);
                std::string name = directoryName.filename().string();
                std::string id = name + "_TreeNode";
                
                // ImGui item height hack
                auto* window = ImGui::GetCurrentWindow();
                window->DC.CurrLineSize.y = 20.0f;
                window->DC.CurrLineTextBaseOffset = 3.0f;
                
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_FramePadding;
//                ((s_fileExplorerData->selectedFilePath == entry) ? ImGuiTreeNodeFlags_Selected : 0);
                
                // Tree Node
                auto tex = entry.is_directory() ? s_fileExplorerData->folderIcon : s_fileExplorerData->fileTex;
                bool open = UI::TreeNode(id, name, flags, tex);
                if (ImGui::IsItemClicked())
                {
                  if (entry.is_directory())
                  {
                    s_fileExplorerData->currentPath = entry;
                  }
                  else
                  {
                    if (s_fileExplorerData->popupType == PopupType::Open)
                    {
//                      s_fileExplorerData->selectedFilePath = entry;
                    }
                  }
                }
                
                // Fixing slight overlap
                UI::ShiftCursorY(3.0f);
                
                // If item clicked
                if (open)
                {
                  ImGui::TreePop();
                }
              }
            }
          }
          ImGui::EndChild(); // folders_fileExplorerData->common
          ImGui::EndTable();
        }
        UI::PopID();
      }
      ImGui::EndPopup();
    }
//    ImGui::PopStyleVar(1);
 
    return returnPath;
  }
  
  void FolderExplorer::SelectPopup(bool* lastPopupFlag, const std::filesystem::path& basePath)
  {
    s_fileExplorerData->popupType = PopupType::Select;
    PopupImpl(lastPopupFlag, basePath);
  }

  void FolderExplorer::PopupImpl(bool* lastPopupFlag, const std::filesystem::path& basePath)
  {
    IK_ASSERT(Utils::FileSystem::Exists(basePath));

    s_fileExplorerData->popup = true;
    s_fileExplorerData->lastPopupFlag = lastPopupFlag;
    s_fileExplorerData->currentPath = basePath;
  }
} // namespace Kreator
