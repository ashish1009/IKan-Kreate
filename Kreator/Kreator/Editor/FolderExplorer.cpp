//
//  FolderExplorer.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "FolderExplorer.hpp"

#include "RendererLayer.hpp"

namespace Kreator
{
  /// This enum stores the type of explorer popup
  enum class PopupType
  {
    Invalid, Create, Open, Save
  };
  
  /// This function returns the popup type in string
  static std::string_view GetPopupTypeName(PopupType type)
  {
    switch (type) {
      case PopupType::Create: return " Create ";
      case PopupType::Open:   return " Open ";
      case PopupType::Save:   return " Save ";
      case PopupType::Invalid:
      default:
        IK_ASSERT(false, "Invalid Type");
        break;
    }
    return "";
  }
  
  /// This structure stores the File explorer data
  struct Data
  {
    inline static constexpr float PopupWidth = 900.0f;
    
    std::string title {"Kreator Folder Viewer"};
    PopupType popupType {PopupType::Invalid};
    
    UI::Popup explorerPopup{"Folder Explorer", false};
    UI::Popup* lastPopup;
    
    Ref<Image> shadowTexture;
    Ref<Image> folderIcon;
    Ref<Image> backButton;
    Ref<Image> fileIcon;
    Ref<Image> newFolder;
    
    std::filesystem::path currentPath {};
    std::filesystem::path selectedPath {};
    std::filesystem::path returnPath {};
    
    std::string extenstionToBeOpened {};
    
    UI::InputBuffer<256> pathBuffer{"##Project Location"};
    UI::InputBuffer<256> searchBuffer{"##Search"};
    UI::InputBuffer<256> fileNameBuffer{"##FileName"};
    
    bool createNewFolder = false;
    UI::InputBuffer<256> newFolderNameBuffer{"##NewFolderName"};
  };
  static Scope<Data> s_data;
  
  void FolderExplorer::Initialize()
  {
    IK_PROFILE();
    IK_LOG_INFO("FolderExplorer", "Initialising the FolderExplorer");
    
    s_data = CreateScope<Data>();
    s_data->shadowTexture = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/ShadowLineTop.png");
    s_data->folderIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/CBP/Folder.png");
    s_data->backButton = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Back.png");
    s_data->fileIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/File.png");
    s_data->newFolder = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/NewFolder.png");
    
    s_data->pathBuffer.MemCpy(s_data->currentPath.c_str(), 0, s_data->currentPath.string().size());
  }
  
  void FolderExplorer::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_INFO("FolderExplorer", "Shutting Down the FolderExplorer");
    
    s_data.reset();
  }
  
  std::filesystem::path FolderExplorer::Explore()
  {
    IK_ASSERT(s_data, "Initialize the folder explorer data FolderExplorer::Initialize");
    
    // Reset return file path each run
    s_data->returnPath = "";
    
    s_data->explorerPopup.Show(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar, []() {
      IK_PERFORMANCE("FolderExplorer::Explore");
      UI::ScopedColor childBG(ImGuiCol_ChildBg, UI::Color::PopupBackground);
      
      // Viewer title bar ----------
      UI::DrawFilledRect(UI::Color::PopupBackground, 20.0f);
      UI::Text(UI::FontType::SemiHeader, s_data->title, UI::AlignX::Center);
      ImGui::Separator();
      
      // Viewer Table ---------------
      UI::ScopedTable welcomeTable({{"##Outliner", Data::PopupWidth / 3}, {"##DirectoryStructure", -1 /* streched to width */}});
      
      welcomeTable.ShowColumn(0, [](){
        RenderSideColumn();
      });
      
      welcomeTable.ShowColumn(1, [](){
        const float topBarHeight = 20.0f;
        RenderTopBar(topBarHeight);
        
        ImGui::Separator();
        RenderMainArea();
      });
    });
    
    return s_data->returnPath;
  }
  
  void FolderExplorer::RenderTopBar(float height)
  {
    ImGui::BeginChild("##top_bar", ImVec2(0, height));
    ImGui::BeginHorizontal("##top_bar", ImGui::GetWindowSize());
    
    // Back Button
    if (UI::DrawButtonImage("##back", s_data->backButton, {24.0f, 18.0f}))
    {
      if (s_data->currentPath != "/")
      {
        ChangeCurrentDirectory(s_data->currentPath.parent_path());
      }
    }
    
    // Address bar
    s_data->pathBuffer.TextInput(true, Data::PopupWidth / 3);
    
    // Search bar
    s_data->searchBuffer.TextInput(false, Data::PopupWidth / 5, "Search ...");
    
    // New folder Icon
    if (UI::DrawButtonImage("##newFolder", s_data->newFolder, {20.0f, 18.0f}))
    {
      s_data->createNewFolder = true;
      static const std::string newFolder = "New Folder";
      s_data->newFolderNameBuffer.MemCpy(newFolder.c_str(), 0, newFolder.size());
    }
    
    ImGui::EndHorizontal();
    ImGui::EndChild();
  }
  
  void FolderExplorer::RenderSideColumn()
  {
    UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
    UI::ScopedStyle rouning(ImGuiStyleVar_FrameRounding, 3);
    
    if (ImGui::CollapsingHeader("System", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
    {
      static std::filesystem::path BaseDirectory = RendererLayer::Get().GetSystemUserPath();
      DirectoryIterator(BaseDirectory);
    }
    
    // Fixing slight overlap
    UI::ShiftCursorY(20.0f);
    
    if (ImGui::CollapsingHeader("IKan-Kreate", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
    {
      static std::filesystem::path IKanKreateDirectory = RendererLayer::Get().GetIKanKreatorPath();
      DirectoryIterator(IKanKreateDirectory);
    }
    
    // Fixing slight overlap
    UI::ShiftCursorY(20.0f);
    
    if (ImGui::CollapsingHeader("Resources", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
    {
      static std::filesystem::path ClientResourceDirectory = RendererLayer::Get().GetClientResorucePath();
      DirectoryIterator(ClientResourceDirectory);
    }
    
    // Shadow
    UI::DrawShadowInner(s_data->shadowTexture);
  }
  
  void FolderExplorer::RenderMainArea()
  {
    // Search buffer string
    std::string searchString = Utils::String::ToLower(s_data->searchBuffer);
    
    // Store the files in current directory to show them at last
    std::vector<std::filesystem::path> filesInCurrentDirectory;
    
    // Show Directories ------------------------------------------------
    for (const auto& path : std::filesystem::directory_iterator(s_data->currentPath))
    {
#ifdef __APPLE__ // Ignore Apple .DS_Store files
      if (path.path().filename().string().at(0) == '.')
      {
        continue;
      }
#endif
      
      // Store files
      if (!path.is_directory())
      {
        filesInCurrentDirectory.push_back(path);
        continue;
      }
      
      auto directoryName = std::filesystem::relative(path.path(), s_data->currentPath);
      std::string name = directoryName.filename().string();
      
      // Match search string
      if (Utils::String::ToLowerCopy(name).find(searchString) != std::string::npos)
      {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_FramePadding
        | ((s_data->selectedPath == path) ? ImGuiTreeNodeFlags_Selected : 0);
        
        std::string id = name + "_TreeNode";
        bool open = UI::TreeNode(id, name, flags, s_data->folderIcon);
        
        if (ImGui::IsItemHovered())
        {
          if (ImGui::IsItemClicked())
          {
            s_data->selectedPath = path;
          }
          if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
          {
            ChangeCurrentDirectory(path);
          }
        }
        
        // If item clicked
        if (open)
        {
          ImGui::TreePop();
        }
      } // if search match
    } // for each directory
    
    // Show Files ---------------------------------------
    for (const auto& file : filesInCurrentDirectory)
    {
#ifdef __APPLE__ // Ignore Apple .DS_Store files
      if (file.filename().string().at(0) == '.')
      {
        continue;
      }
#endif
      
      auto directoryName = std::filesystem::relative(file, s_data->currentPath);
      std::string name = directoryName.filename().string();
      
      // Match search string
      if (Utils::String::ToLowerCopy(name).find(searchString) != std::string::npos)
      {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_FramePadding
        | ((s_data->selectedPath == file) ? ImGuiTreeNodeFlags_Selected : 0);
        
        std::string id = name + "_TreeNode";
        bool open = UI::TreeNode(id, name, flags, s_data->fileIcon);
        if (ImGui::IsItemClicked())
        {
          s_data->selectedPath = file;
        }
        
        // If item clicked
        if (open)
        {
          ImGui::TreePop();
        }
      } // if search
    } // File iterator
    
    // Rename new folder
    {
      if (s_data->createNewFolder)
      {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_FramePadding;
        UI::TreeNode("_NewFolder", " ", flags, s_data->folderIcon);
        {
          ImGui::SetKeyboardFocusHere();
          ImGui::SetNextItemWidth(100);
          
          ImGui::SameLine();
          UI::ShiftCursor(0, -5);
          if (s_data->newFolderNameBuffer.TextInput(false, 100, "", ImGuiInputTextFlags_EnterReturnsTrue))
          {
            s_data->createNewFolder = false;
            std::filesystem::path newFolderPath = s_data->currentPath / s_data->newFolderNameBuffer.Data();
            std::filesystem::create_directory(newFolderPath);
            s_data->newFolderNameBuffer.Memset(0);
          }
        }
      }
    }
    
    // Buttons and File name
    {
      UI::SetCursorPosY(ImGui::GetWindowHeight() - 30.0f);
      UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 6.0f));
      ImGui::Separator();
      
      s_data->fileNameBuffer.TextInput(false, Data::PopupWidth / 2, "File Name");
      
      ImGui::SameLine();
      if (UI::DrawButton(" Cancel ", UI::FontType::Bold, UI::Color::TextBrighter, UI::Color::TextMuted, 2) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape))
      {
        ImGui::CloseCurrentPopup();
        if (s_data->lastPopup)
        {
          s_data->lastPopup->SetFlag(true);
        }
      }
      
      ImGui::SameLine();
      
      // Save/ Open/ Crete button
      DrawEnterButton();
    }
  }
  
  void FolderExplorer::DrawEnterButton()
  {
    ImU32 buttonColor = UI::Color::TextMuted;
    ImU32 textColor = UI::Color::TextBrighter;
    
    bool validButton = false;
    static std::filesystem::path returnPath {};
    
    switch (s_data->popupType)
    {
      case PopupType::Create:
      {
        if (!s_data->fileNameBuffer.Empty())
        {
          buttonColor = UI::Color::NiceBlueHighlight;
          textColor = UI::Color::Background;
          returnPath = s_data->currentPath / s_data->fileNameBuffer.Data();
          
          validButton = true;
        }
        break;
      }
      case PopupType::Open:
      {
        if (s_data->selectedPath.extension() == s_data->extenstionToBeOpened)
        {
          buttonColor = UI::Color::NiceBlueHighlight;
          textColor = UI::Color::Background;
          returnPath = s_data->selectedPath;
          
          validButton = true;
        }
        break;
      }
      case PopupType::Save:
      {
        if (!s_data->fileNameBuffer.Empty())
        {
          buttonColor = UI::Color::NiceBlueHighlight;
          textColor = UI::Color::Background;
          returnPath = s_data->currentPath / s_data->fileNameBuffer.Data();
          
          validButton = true;
        }

        break;
      }
      case PopupType::Invalid:
      default:
        IK_ASSERT(false, "Invalid popup type")
    }
    
    if (UI::DrawButton(GetPopupTypeName(s_data->popupType), UI::FontType::Bold, textColor, buttonColor, 2) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter))
    {
      if (validButton)
      {
        s_data->returnPath = returnPath;
        returnPath = "";
        ImGui::CloseCurrentPopup();
      }
    }
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
      
      bool open = UI::TreeNode(id, name, ImGuiTreeNodeFlags_SpanFullWidth, s_data->folderIcon);
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
    s_data->currentPath = currentPath;
    s_data->pathBuffer.Memset(0);
    s_data->pathBuffer.MemCpy(s_data->currentPath.c_str(), 0, s_data->currentPath.string().size());
    s_data->selectedPath = "";
  }
  
  void FolderExplorer::ShowCreatePopup(std::string_view title, const std::filesystem::path& basePath, UI::Popup* lastPopup)
  {
    IK_ASSERT(s_data, "Initialize the folder explorer data FolderExplorer::Initialize");
    
    s_data->title = title;
    s_data->explorerPopup.Set("Folder Explorer", true, Data::PopupWidth, 600.0f, true);
    s_data->lastPopup = lastPopup;
    
    if (basePath != "" and std::filesystem::exists(basePath))
    {
      s_data->currentPath = basePath;
    }
    else
    {
      s_data->currentPath = RendererLayer::Get().GetIKanKreatorPath();
    }
    
    s_data->pathBuffer.MemCpy(s_data->currentPath.c_str(), 0, s_data->currentPath.string().size());
    s_data->selectedPath = "";
    s_data->returnPath = "";
    s_data->createNewFolder = false;
    
    s_data->popupType = PopupType::Create;
    s_data->extenstionToBeOpened = "";
  }
  
  void FolderExplorer::ShowOpenPopup(std::string_view title, const std::string& extenstionToBeOpened, const std::filesystem::path& basePath, UI::Popup* lastPopup)
  {
    IK_ASSERT(s_data, "Initialize the folder explorer data FolderExplorer::Initialize");
    
    s_data->title = title;
    s_data->explorerPopup.Set("Folder Explorer", true, Data::PopupWidth, 600.0f, true);
    s_data->lastPopup = lastPopup;
    
    if (basePath != "" and std::filesystem::exists(basePath))
    {
      s_data->currentPath = basePath;
    }
    else
    {
      s_data->currentPath = RendererLayer::Get().GetIKanKreatorPath();
    }
    s_data->pathBuffer.MemCpy(s_data->currentPath.c_str(), 0, s_data->currentPath.string().size());
    s_data->selectedPath = "";
    s_data->returnPath = "";
    s_data->createNewFolder = false;
    
    s_data->popupType = PopupType::Open;
    s_data->extenstionToBeOpened = extenstionToBeOpened;
  }
  void FolderExplorer::ShowSavePopup(std::string_view title, const std::filesystem::path& basePath, UI::Popup* lastPopup)
  {
    IK_ASSERT(s_data, "Initialize the folder explorer data FolderExplorer::Initialize");
    
    s_data->title = title;
    s_data->explorerPopup.Set("Folder Explorer", true, Data::PopupWidth, 600.0f, true);
    s_data->lastPopup = lastPopup;
    
    if (basePath != "" and std::filesystem::exists(basePath))
    {
      s_data->currentPath = basePath;
    }
    else
    {
      s_data->currentPath = RendererLayer::Get().GetIKanKreatorPath();
    }
    s_data->pathBuffer.MemCpy(s_data->currentPath.c_str(), 0, s_data->currentPath.string().size());
    s_data->selectedPath = "";
    s_data->returnPath = "";
    s_data->createNewFolder = false;
    
    s_data->popupType = PopupType::Save;
    s_data->extenstionToBeOpened = "";
  }

} // namespace Kreator
