//
//  FolderExplorer.hpp
//  Kreator
//
//  Created by Ashish . on 16/02/24.
//

#pragma once

#include "Renderer/Graphics/Texture.hpp"

namespace Kreator
{
  class FolderExplorer
  {
  public:
    /// This function initialise the Folder explorer. Path and Textures
    static void Initialize();
    /// This function shutdown the Folder explorer. Path and Textures
    static void Shutdown();
    
    /// Thius function render the widget and return the selected file path
    static std::filesystem::path Explore();
    
    /// This function set the popup to show Select folder widgwet
    /// - Parameter basePath: Base start path
    /// - Parameter lastPopupFlag: last popup flag pointer
    static void Select(const std::filesystem::path& basePath = "", bool *lastPopupFlag = nullptr);
    /// This function set the popup to show open file widgwet
    /// - Parameter extenstion: externsion to be opened
    /// - Parameter basePath: Base start path
    /// - Parameter lastPopupFlag: last popup flag pointer
    static void Open(const std::string& extenstion, const std::filesystem::path& basePath = "", bool *lastPopupFlag = nullptr);
    /// This function set the popup to show save file widgwet
    /// - Parameter lastPopupFlag: last popup flag pointer
    /// - Parameter basePath: Base start path
    static void Save(const std::filesystem::path& basePath = "", bool *lastPopupFlag = nullptr);
    
    DELETE_ALL_CONSTRUCTORS(FolderExplorer);
    
  private:
    /// This function renders the top bar of viewer
    /// - Parameter height: Height of top bar
    static void RenderTopBar(float height);
    /// This function iterate the direcotries
    /// - Parameter currentDirectory: current directory
    static void DirectoryIterator(const std::filesystem::path& currentDirectory);
    /// This funcion renders the Size area
    static void RenderSideColumn();
    /// This funcion renders the main area
    static void RenderMainArea();
    
    /// This function changes the current directory path
    /// - Parameter currentPath: current directory path
    static void ChangeCurrentDirectory(const std::filesystem::path& currentPath);
  };
} // namespace Kreator
