//
//  FolderExplorer.cpp
//  Kreator
//
//  Created by Ashish . on 16/02/24.
//

#include "FolderExplorer.hpp"
#include "KreatorLayer.hpp"

namespace Kreator
{
  enum class PopupType
  {
    Invalid, Select, Open, Save
  };
  
  struct Data
  {
    bool popup {false};
    PopupType popupType {PopupType::Invalid};

    Ref<Texture> shadowTexture;
    Ref<Texture> folderIcon;
  };
  static Scope<Data> s_fileExplorerData;

  void FolderExplorer::Initialize()
  {
    IK_PROFILE();
    IK_LOG_TRACE("FolderExplorer", "Initialising the FolderExplorer textures");
    s_fileExplorerData = CreateScope<Data>();
    s_fileExplorerData->shadowTexture = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/ShadowLineTop.png");
    s_fileExplorerData->folderIcon = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/Folder.png");
  }
  
  void FolderExplorer::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_WARN("FolderExplorer", "Shutting Down the FolderExplorer textures");
    s_fileExplorerData.reset();
  }
  
  std::filesystem::path FolderExplorer::Explore()
  {
    std::filesystem::path returnPath {};
    
    if (s_fileExplorerData->popup)
    {
      ImGui::OpenPopup("Kreator File Viewer");
      s_fileExplorerData->popup = false;
    }
    
    UI::SetNextWindowAtCenterWithSize({900, 600});

    if (ImGui::BeginPopupModal("Kreator File Viewer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove ))
    {
      // Viewer title bar ----------
      {
        float titlebarHeight = 20.0f;
        
        const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;
        
        // Draw the title Bar rectangle ---------------------------------------------------
        const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
        const ImVec2 titlebarMax =
        {
          ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - 16,
          ImGui::GetCursorScreenPos().y + titlebarHeight
        };
        
        UI::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y));
        auto* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(titlebarMin, titlebarMax, UI::Color::Titlebar);
        
        // Title
        {
          UI::ScopedColor muted(ImGuiCol_Text, UI::Color::TextDarker);
          UI::ScopedFont header(UI::GetSemiHeaderFont());
          
          static const std::string title = "Kreator file Viewer";
          UI::ShiftCursorX(ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize(title.c_str()).x / 2);
          ImGui::Text("Kreator file Viewer");
        }
      } // Title bar Scope
      
      // Viewer Table ---------------
      {
        UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
        UI::ScopedStyle cellPadding(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 2.0f));
        UI::ScopedStyle rouning(ImGuiStyleVar_FrameRounding, 15);

        ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;
        if (ImGui::BeginTable(UI::GenerateID(), 2 /* Num Columns */, tableFlags, ImVec2(0.0f, 0.0f)))
        {
          UI::PushID();
          // Left Column
          {
            ImGui::TableSetupColumn("Outliner", 0, 300.0f);
            ImGui::TableSetupColumn("Directory Structure", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableNextRow();

            // Content Outliner
            ImGui::TableSetColumnIndex(0);
            ImGui::BeginChild("##folders_common");
            {
              UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
              
              if (ImGui::CollapsingHeader("System", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
              {
                static std::filesystem::path BaseDirectory = KreatorLayer::Get().GetSystemUserPath();
                DirectoryIterator(BaseDirectory);
              }

              // Fixing slight overlap
              UI::ShiftCursorY(20.0f);
              
              if (ImGui::CollapsingHeader("IKan-Kreate", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
              {
                static std::filesystem::path IKanKreateDirectory = KreatorLayer::Get().GetIKanKreatorPath();
                DirectoryIterator(IKanKreateDirectory);
              }

              // Fixing slight overlap
              UI::ShiftCursorY(20.0f);
              
              if (ImGui::CollapsingHeader("Resources", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
              {
                static std::filesystem::path ClientResourceDirectory = KreatorLayer::Get().GetClientResorucePath();
                DirectoryIterator(ClientResourceDirectory);
              }

              // Draw side shadow
              ImRect windowRect = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), 0.0f, 0.0f);
              ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
              UI::DrawShadowInner(s_fileExplorerData->shadowTexture, 15.0f, windowRect, 1.0f, windowRect.GetHeight() / 4.0f, false, true, false, false);
              ImGui::PopClipRect();
            }
            ImGui::EndChild();
          } // Left Columns
          
          // Right column
          {
            // Directory Content
            ImGui::TableSetColumnIndex(1);
            const float topBarHeight = 26.0f;
            const float bottomBarHeight = 0.0f;
            ImGui::BeginChild("##directory_structure", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - topBarHeight - bottomBarHeight - 14 /* Separator offset */));
            {
              ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
              RenderTopBar(topBarHeight);
              ImGui::PopStyleVar();
              ImGui::Separator();
            }
            ImGui::EndChild();
          } // right column
          UI::PopID();
          ImGui::EndTable();
        } // Begin Table
      } // Viewere Scope
      ImGui::EndPopup();
    } // Begin Popup
    return returnPath;
  }
  
  void FolderExplorer::RenderTopBar(float height)
  {
    ImGui::BeginChild("##top_bar", ImVec2(0, height));
    ImGui::BeginHorizontal("##top_bar", ImGui::GetWindowSize());
    {
    }
    ImGui::EndHorizontal();
    ImGui::EndChild();
  }
  
  void FolderExplorer::DirectoryIterator(const std::filesystem::path& currentDirectory)
  {
    for (const auto& directory : std::filesystem::directory_iterator(currentDirectory))
    {
      const std::filesystem::path& path = directory.path();
      std::string name = path.filename().string();
      auto i = name.at(0);
#ifdef __APPLE__ // Ignore Apple .DS_Store files
      if (name.at(0) == '.')
      {
        continue;
      }
#endif
      
      // Show only directory
      if (!directory.is_directory())
      {
        continue;
      }
      
      std::string id = name + "_TreeNode";
      
      // ImGui item height hack
      auto* window = ImGui::GetCurrentWindow();
      window->DC.CurrLineSize.y = 20.0f;
      window->DC.CurrLineTextBaseOffset = 3.0f;
      
      bool open = UI::TreeNode(id, name, ImGuiTreeNodeFlags_SpanFullWidth, s_fileExplorerData->folderIcon);
      
      // Fixing slight overlap
      UI::ShiftCursorY(3.0f);
      
      if (open)
      {
        DirectoryIterator(directory);
        ImGui::TreePop();
      }
    }
  }
  
  void FolderExplorer::Select()
  {
    s_fileExplorerData->popup = true;
    s_fileExplorerData->popupType = PopupType::Select;
  }
  
  void FolderExplorer::Open()
  {
    s_fileExplorerData->popup = true;
    s_fileExplorerData->popupType = PopupType::Open;
  }
  
  void FolderExplorer::Save()
  {
    s_fileExplorerData->popup = true;
    s_fileExplorerData->popupType = PopupType::Save;
  }
} // namespace Kreator
