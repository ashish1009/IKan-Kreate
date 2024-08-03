//
//  FolderExplorer.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

namespace Kreator
{
  /// This class stores the API to explore folder
  class FolderExplorer
  {
  public:
    /// This function initialises the Folder explorer. Path and Textures
    /// - Note: Should get initialized after kreter renderer layer initialize
    static void Initialize();
    /// This function shuts down the Folder explorer. Path and Textures
    static void Shutdown();
    
    /// Thius function render the widget and return the selected file path
    static std::filesystem::path Explore();
    
    /// This function set the popup to show Select folder widget
    /// - Parameters:
    ///   - basePath: Current path for explorer
    ///   - lastPopup: last popup pointer
    static void ShowCreatePopup(const std::filesystem::path& basePath = "", UI::Popup* lastPopup = nullptr);
    /// This function set the popup to show open file widgwet
    /// - Parameters:
    ///   - extenstion: externsion to be opened
    ///   - basePath: Current path for exploree
    ///   - lastPopup: last popup pointer
    static void ShowOpenPopup(const std::string& extenstion, const std::filesystem::path& basePath = "", UI::Popup* lastPopup = nullptr);
    /// This function set the popup to show save file widgwet
    /// - Parameters:
    ///   - basePath: Current path for exploree
    ///   - lastPopup: last popup pointer
    static void ShowSavePopup(const std::filesystem::path& basePath = "", UI::Popup* lastPopup = nullptr);

  private:
    /// This function renders the top bar of viewer
    /// - Parameter height: Height of top bar
    static void RenderTopBar(float height);
    /// This function iterate the direcotries
    /// - Parameter currentDirectory: current directory
    static void DirectoryIterator(const std::filesystem::path& currentDirectory);
    /// This function renders the Size area
    static void RenderSideColumn();
    /// This function renders the main area
    static void RenderMainArea();
    /// This function draw the main active enter button
    static void DrawEnterButton();
    
    /// This function changes the current directory path
    /// - Parameter currentPath: current directory path
    static void ChangeCurrentDirectory(const std::filesystem::path& currentPath);
  };
} // namespace Kreator
