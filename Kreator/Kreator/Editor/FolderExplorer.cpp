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
    bool* lastPopupFlag;
    PopupType popupType {PopupType::Invalid};
    
    std::filesystem::path currentPath {};
    std::filesystem::path selectedPath {};
    std::filesystem::path returnPath {};

    std::string extenstionToBeOpened {};
    
    GUI_InputBuffer<256> pathBuffer;
    char fileBuffer[256];
    char searchBuffer[256];

    Ref<Texture> shadowTexture;
    Ref<Texture> folderIcon;
    Ref<Texture> backButton;
    Ref<Texture> fileIcon;
    Ref<Texture> newFolder;
    
    bool createNewFolder = false;
    char newFolderNameBuffer[256];
  };
  static Scope<Data> s_fileExplorerData;

  void FolderExplorer::Initialize()
  {
    IK_PROFILE();
    IK_LOG_TRACE("FolderExplorer", "Initialising the FolderExplorer textures");
    s_fileExplorerData = CreateScope<Data>();
    s_fileExplorerData->shadowTexture = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/ShadowLineTop.png");
    s_fileExplorerData->folderIcon = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/CBP/Folder.png");
    s_fileExplorerData->backButton = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/Back.png");
    s_fileExplorerData->fileIcon = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/File.png");
    s_fileExplorerData->newFolder = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/NewFolder.png");

    s_fileExplorerData->currentPath = KreatorLayer::Get().GetIKanKreatorPath();
    s_fileExplorerData->pathBuffer.MemCpy(s_fileExplorerData->currentPath.c_str(), 0, s_fileExplorerData->currentPath.string().size());
  }
  
  void FolderExplorer::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_WARN("FolderExplorer", "Shutting Down the FolderExplorer textures");
    s_fileExplorerData.reset();
  }
  
  std::filesystem::path FolderExplorer::Explore()
  {
    if (s_fileExplorerData->popup)
    {
      ImGui::OpenPopup("Kreator File Viewer");
      s_fileExplorerData->popup = false;
    }
    
    // Reset return file path each run
    s_fileExplorerData->returnPath = "";
    
    UI::SetNextWindowAtCenterWithSize({900, 600});
    UI::ScopedColor bgCol(ImGuiCol_ChildBg, UI::Color::Background);

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

        ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;
        if (ImGui::BeginTable(UI::GenerateID(), 2 /* Num Columns */, tableFlags, ImVec2(0.0f, 0.0f)))
        {
          UI::PushID();
          // Left Column ------------------
          UI::ScopedStyle rouning(ImGuiStyleVar_FrameRounding, 5);
          ImGui::TableSetupColumn("Outliner", 0, 300.0f);
          ImGui::TableSetupColumn("Directory Structure", ImGuiTableColumnFlags_WidthStretch);
          ImGui::TableNextRow();
          
          // Content Outliner
          ImGui::TableSetColumnIndex(0);
          ImGui::BeginChild("##folders_common");
          {
            RenderSideColumn();
          }
          ImGui::EndChild();
          
          // Right column ------------------
          // Directory Content
          ImGui::TableSetColumnIndex(1);
          const float topBarHeight = 38.0f;
          const float bottomBarHeight = 0.0f;
          ImGui::BeginChild("##directory_structure", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - topBarHeight - bottomBarHeight));
          {
            RenderTopBar(topBarHeight);
            RenderMainArea();
          }
          ImGui::EndChild();
          
          UI::PopID();
          ImGui::EndTable();
        } // Begin Table
      } // Viewer Table Scope
      ImGui::EndPopup();
    } // Begin Popup
    
    return s_fileExplorerData->returnPath;
  }
  
  void FolderExplorer::RenderTopBar(float height)
  {
    UI::ScopedStyle frameBorder(ImGuiStyleVar_FrameBorderSize, 0.0f);
    UI::ScopedStyle rouning(ImGuiStyleVar_FrameRounding, 5);
    ImGui::BeginChild("##top_bar", ImVec2(0, height));
    ImGui::BeginHorizontal("##top_bar", ImGui::GetWindowSize());
    {
      // Render Icon Buttons
      auto browserButton = [](const char* labelId, const Ref<Texture>& icon)
      {
        const ImU32 buttonCol = UI::Color::GroupHeader;
        const ImU32 buttonColP = UI::ColorWithMultipliedValue(buttonCol, 0.6f);
        const ImU32 buttonColH = UI::ColorWithMultipliedValue(buttonCol, 0.8f);
        UI::ScopedColorStack buttonColors(ImGuiCol_Button, buttonCol, ImGuiCol_ButtonHovered, buttonColH, ImGuiCol_ButtonActive, buttonColP);
        
        const float iconSize = 24.0f;
        const float iconPadding = 3.0f;
        const bool clicked = ImGui::Button(labelId, ImVec2(iconSize, iconSize));
        UI::DrawButtonImage(icon, UI::Color::TextDarker,
                            UI::ColorWithMultipliedValue(UI::Color::TextDarker, 1.2f),
                            UI::ColorWithMultipliedValue(UI::Color::TextDarker, 0.8f),
                            UI::RectExpanded(UI::GetItemRect(), -iconPadding, -iconPadding));
        
        return clicked;
      };
      
      UI::ShiftCursor(8, 10);
      // Back button
      {
        if (browserButton("##back", s_fileExplorerData->backButton))
        {
          if (s_fileExplorerData->currentPath != "/")
          {
            ChangeCurrentDirectory(s_fileExplorerData->currentPath.parent_path());
          }
        }
      }
      
      // Address bar
      {
        UI::ScopedColor muted(ImGuiCol_Text, UI::Color::TextDarker);
        ImGui::SetNextItemWidth(350);
        ImGui::InputTextWithHint("##new_project_location", "Project Location", s_fileExplorerData->pathBuffer.Data(), s_fileExplorerData->pathBuffer.Size(), ImGuiInputTextFlags_ReadOnly);
      }
      
      // Search bar
      {
        ImGui::SetNextItemWidth(200);
        ImGui::InputTextWithHint("##regsearch", "Search ...", s_fileExplorerData->searchBuffer, 256);
      }
      
      // New folder Icon
      {
        if (browserButton("##newFolder", s_fileExplorerData->newFolder))
        {
          s_fileExplorerData->createNewFolder = true;
          static const std::string newFolder = "New Folder";
          memcpy(s_fileExplorerData->newFolderNameBuffer, newFolder.c_str(), newFolder.size());
        }
      }
    }
    ImGui::EndHorizontal();
    ImGui::EndChild();
    
    ImGui::Separator();
  }
  
  void FolderExplorer::RenderSideColumn()
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
  
  void FolderExplorer::RenderMainArea()
  {
    UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 5.0f));
    
    std::vector<std::filesystem::path> filesInCurrentDirectory;
    std::string searchString = Utils::String::ToLower(s_fileExplorerData->searchBuffer);
    
    // Show Directories
    for (const auto& directory : std::filesystem::directory_iterator(s_fileExplorerData->currentPath))
    {
#ifdef __APPLE__ // Ignore Apple .DS_Store files
      if (directory.path().filename().string().at(0) == '.')
      {
        continue;
      }
#endif
      // Tree Node
      if (!directory.is_directory())
      {
        filesInCurrentDirectory.push_back(directory);
        continue;
      }
      
      auto directoryName = std::filesystem::relative(directory.path(), s_fileExplorerData->currentPath);
      std::string name = directoryName.filename().string();
      std::string id = name + "_TreeNode";
      
      if (Utils::String::ToLowerCopy(name).find(searchString) != std::string::npos)
      {
        // ImGui item height hack
        auto* window = ImGui::GetCurrentWindow();
        window->DC.CurrLineSize.y = 20.0f;
        window->DC.CurrLineTextBaseOffset = 3.0f;
        
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_FramePadding
        | ((s_fileExplorerData->selectedPath == directory) ? ImGuiTreeNodeFlags_Selected : 0);
        
        bool open = UI::TreeNode(id, name, flags, s_fileExplorerData->folderIcon);
        if (ImGui::IsItemHovered())
        {
          if (ImGui::IsItemClicked())
          {
            s_fileExplorerData->selectedPath = directory;
          }
          if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
          {
            ChangeCurrentDirectory(directory);
          }
        }
        
        // Fixing slight overlap
        UI::ShiftCursorY(3.0f);
        
        // If item clicked
        if (open)
        {
          ImGui::TreePop();
        }
      } // If search
    } // Directory iterator
    
    // Rename new folder
    {
      if (s_fileExplorerData->createNewFolder)
      {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_FramePadding;
        UI::TreeNode("_NewFolder", " ", flags, s_fileExplorerData->folderIcon);
        {
          ImGui::SetKeyboardFocusHere();
          ImGui::SetNextItemWidth(100);
          
          ImGui::SameLine();
          UI::ShiftCursor(10, -5);
          if (ImGui::InputText("##rename", s_fileExplorerData->newFolderNameBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue))
          {
            s_fileExplorerData->createNewFolder = false;
            std::filesystem::path newFolderPath = s_fileExplorerData->currentPath / s_fileExplorerData->newFolderNameBuffer;
            std::filesystem::create_directory(newFolderPath);
            memset(s_fileExplorerData->newFolderNameBuffer, 0, 256);
          }
        }
      }
    }
    
    if (s_fileExplorerData->popupType != PopupType::Select)
    {
      // Show Files
      for (const auto& file : filesInCurrentDirectory)
      {
#ifdef __APPLE__ // Ignore Apple .DS_Store files
        if (file.filename().string().at(0) == '.')
        {
          continue;
        }
#endif
        if (s_fileExplorerData->popupType == PopupType::Open and file.extension() != s_fileExplorerData->extenstionToBeOpened)
        {
          continue;
        }
        auto directoryName = std::filesystem::relative(file, s_fileExplorerData->currentPath);
        std::string name = directoryName.filename().string();
        std::string id = name + "_TreeNode";
        
        if (Utils::String::ToLowerCopy(name).find(searchString) != std::string::npos)
        {
          // ImGui item height hack
          auto* window = ImGui::GetCurrentWindow();
          window->DC.CurrLineSize.y = 20.0f;
          window->DC.CurrLineTextBaseOffset = 3.0f;
          
          ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_FramePadding
          | ((s_fileExplorerData->selectedPath == file) ? ImGuiTreeNodeFlags_Selected : 0);
          
          bool open = UI::TreeNode(id, name, flags, s_fileExplorerData->fileIcon);
          if (ImGui::IsItemClicked())
          {
            s_fileExplorerData->selectedPath = file;
          }
          
          // Fixing slight overlap
          UI::ShiftCursorY(3.0f);
          
          // If item clicked
          if (open)
          {
            ImGui::TreePop();
          }
        } // if search
      } // File iterator
    }
   
    // Buttons and File name
    {
      ImGui::PushFont(UI::GetBoldFont());
      UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 0.0f));
      UI::SetCursorPosY(520);
      ImGui::Separator();
      
      if (s_fileExplorerData->popupType == PopupType::Save)
      {
        UI::ShiftCursor(8, 10);
        ImGui::SetNextItemWidth(530);
        ImGui::InputTextWithHint("## FileName", "File Name", s_fileExplorerData->fileBuffer, 256);
        
        ImGui::SameLine();
        UI::DrawRoundButton(" Save ", UI::Color::NiceThemeHighlight, 5);
      } // if Save
      else
      {
        UI::ShiftCursor(ImGui::GetContentRegionAvail().x / 2 - 180, 10);
        if (s_fileExplorerData->popupType == PopupType::Open)
        {
          if (UI::DrawRoundButton(" Open ", UI::Color::NiceThemeHighlight, 5) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter))
          {
            ImGui::CloseCurrentPopup();
            if (s_fileExplorerData->lastPopupFlag)
            {
              *s_fileExplorerData->lastPopupFlag = true;
            }
            s_fileExplorerData->returnPath = s_fileExplorerData->selectedPath;
          }
        } // if open
        else if (s_fileExplorerData->popupType == PopupType::Select)
        {
          if (UI::DrawRoundButton(" Select ", UI::Color::NiceThemeHighlight, 5) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter))
          {
            if (!s_fileExplorerData->createNewFolder and s_fileExplorerData->selectedPath != "")
            {
              ImGui::CloseCurrentPopup();
              if (s_fileExplorerData->lastPopupFlag)
              {
                *s_fileExplorerData->lastPopupFlag = true;
              }
              
              s_fileExplorerData->returnPath = s_fileExplorerData->selectedPath;
            }
          } // if select
        }
      } // else of if Save
            
      ImGui::SameLine();
      if (UI::DrawRoundButton("Cancel", UI::Color::Muted, 5) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape))
      {
        if (!s_fileExplorerData->createNewFolder)
        {
          ImGui::CloseCurrentPopup();
          if (s_fileExplorerData->lastPopupFlag)
          {
            *s_fileExplorerData->lastPopupFlag = true;
          }
        }
        else
        {
          s_fileExplorerData->createNewFolder = false;
          memset(s_fileExplorerData->newFolderNameBuffer, 0, 256);
        }
      }
      ImGui::PopFont();
    } // Button
  }
  
  void FolderExplorer::DirectoryIterator(const std::filesystem::path& currentDirectory)
  {
    for (const auto& directory : std::filesystem::directory_iterator(currentDirectory))
    {
      const std::filesystem::path& path = directory.path();
      std::string name = path.filename().string();
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
      if (ImGui::IsItemClicked())
      {
        ChangeCurrentDirectory(directory);
      }
      // Fixing slight overlap
      UI::ShiftCursorY(3.0f);
      
      if (open)
      {
        DirectoryIterator(directory);
        ImGui::TreePop();
      }
    }
  }
  
  void FolderExplorer::ChangeCurrentDirectory(const std::filesystem::path &currentPath)
  {
    s_fileExplorerData->currentPath = currentPath;
    s_fileExplorerData->pathBuffer.Memset(0);
    s_fileExplorerData->pathBuffer.MemCpy(s_fileExplorerData->currentPath.c_str(), 0, s_fileExplorerData->currentPath.string().size());
    s_fileExplorerData->selectedPath = "";
  }
  
  void FolderExplorer::Select(bool *lastPopupFlag)
  {
    s_fileExplorerData->popup = true;
    s_fileExplorerData->lastPopupFlag = lastPopupFlag;
    s_fileExplorerData->currentPath = KreatorLayer::Get().GetIKanKreatorPath();
    s_fileExplorerData->pathBuffer.MemCpy(s_fileExplorerData->currentPath.c_str(), 0, s_fileExplorerData->currentPath.string().size());
    s_fileExplorerData->selectedPath = "";
    s_fileExplorerData->returnPath = "";
    s_fileExplorerData->createNewFolder = false;
    
    s_fileExplorerData->popupType = PopupType::Select;
  }
  
  void FolderExplorer::Open(const std::string& extenstionToBeOpened, bool *lastPopupFlag)
  {
    s_fileExplorerData->popup = true;
    s_fileExplorerData->lastPopupFlag = lastPopupFlag;
    s_fileExplorerData->currentPath = KreatorLayer::Get().GetIKanKreatorPath();
    s_fileExplorerData->pathBuffer.MemCpy(s_fileExplorerData->currentPath.c_str(), 0, s_fileExplorerData->currentPath.string().size());
    s_fileExplorerData->selectedPath = "";
    s_fileExplorerData->returnPath = "";
    s_fileExplorerData->createNewFolder = false;
    
    s_fileExplorerData->popupType = PopupType::Open;
    s_fileExplorerData->extenstionToBeOpened = extenstionToBeOpened;
  }
  
  void FolderExplorer::Save(bool *lastPopupFlag)
  {
    s_fileExplorerData->popup = true;
    s_fileExplorerData->lastPopupFlag = lastPopupFlag;
    s_fileExplorerData->currentPath = KreatorLayer::Get().GetIKanKreatorPath();
    s_fileExplorerData->pathBuffer.MemCpy(s_fileExplorerData->currentPath.c_str(), 0, s_fileExplorerData->currentPath.string().size());
    s_fileExplorerData->selectedPath = "";
    s_fileExplorerData->returnPath = "";
    s_fileExplorerData->createNewFolder = false;

    s_fileExplorerData->popupType = PopupType::Save;
  }
} // namespace Kreator
