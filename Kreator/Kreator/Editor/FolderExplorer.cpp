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
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 10));

    if (ImGui::BeginPopupModal("Folder Explorer", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove
                               | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground))
    {
      const float topBarHeight = 40;
      auto* drawList = ImGui::GetWindowDrawList();
      
      // Draw the adress Bar rectangle ---------------------------------------------------
      {
        const ImVec2 addresBarMin = ImGui::GetCursorScreenPos();
        const ImVec2 addresBarMax =
        {
          addresBarMin.x + ImGui::GetWindowWidth(),
          addresBarMin.y + topBarHeight
        };
        
        drawList->AddRectFilled(addresBarMin, addresBarMax, UI::Theme::Color::BackgroundPopup);
      }
      
      // Draw the Main Area Bar rectangle -------------------------------------------------
      {
        const ImVec2 mainAreaMin =
        {
          ImGui::GetCursorScreenPos().x,
          ImGui::GetCursorScreenPos().y + topBarHeight
        };
        const ImVec2 mainAreaMax =
        {
          mainAreaMin.x + ImGui::GetWindowWidth(),
          mainAreaMin.y + 700 - topBarHeight
        };
        
        drawList->AddRectFilled(mainAreaMin, mainAreaMax, UI::Theme::Color::GroupHeader);
      }
      
      {
        UI::ScopedStyle windowPadding (ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
        
        UI::ShiftCursorX(ImGui::GetCurrentWindow()->WindowPadding.x);
        UI::ShiftCursorY(ImGui::GetCurrentWindow()->WindowPadding.y);
        
        // Render Icon Buttons -----------------------------------------------------------
        {
          UI::ScopedColor button(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
          UI::ScopedColor buttonhovered(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
          UI::ScopedColor buttonavtive(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
          auto contenBrowserButton = [](const char* labelId, const Ref<Image>& icon)
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
          
          if (contenBrowserButton("##back", s_fileExplorerData->backButton))
          {
            
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

      // Draw top shadow-----------------------------------------------------------------
      ImRect shadowRect =
      {
        {
          ImGui::GetCursorScreenPos().x,
          ImGui::GetCursorScreenPos().y + 2
        },
        { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - 11,
          ImGui::GetCursorScreenPos().y + 20
        }
      };
      
      ImRect windowRect = UI::RectExpanded(shadowRect, 0.0f, 0.0f);
      ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
      UI::DrawShadowInner(s_fileExplorerData->shadowwTexture, 10.0f, windowRect, 1.0f, windowRect.GetWidth() / 8,
                          false, false, true, false);

      // Folder Explorer Main -----------------------------------------------------------
      UI::PushID();
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
        
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet ;
        //| ((s_fileExplorerData->selectedFilePath == entry) ? ImGuiTreeNodeFlags_Selected : 0);
        
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
      UI::PopID();
      ImGui::EndPopup();
    }
    ImGui::PopStyleVar(1);
 
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
