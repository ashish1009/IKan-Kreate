//
//  FolderExplorer.hpp
//  Kreator
//
//  Created by Ashish . on 03/08/23.
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
    /// - Parameters:
    ///   - lastPopupFlad: Last Popup flag to be opened again
    ///   - basePath: Base Folder Explorer Patg
    ///   - title: Title of popeup
    static void SelectPopup(const std::string& title, const std::filesystem::path& basePath, bool *lastPopupFlad = nullptr);
    /// This function set the popup to show open file widgwet
    /// - Parameters:
    ///   - lastPopupFlad: Last Popup flag to be opened again
    ///   - basePath: Base Folder Explorer Patg
    ///   - title: Title of popeup
    static void OpenPopup(const std::string& title, const std::filesystem::path& basePath, bool *lastPopupFlad = nullptr );
    /// This function set the popup to show save file widgwet
    /// - Parameters:
    ///   - basePath: Base Folder Explorer Patg
    ///   - title: Title of popeup
    static void SavePopup(const std::string& title, const std::filesystem::path& basePath);
    
  private:
    /// This function set the popup to show open file widgwet
    /// - Parameters:
    ///   - lastPopupFlad: Last Popup flag to be opened again
    ///   - basePath: Base Folder Explorer Patg
    static void PopupImpl(const std::filesystem::path& basePath, bool *lastPopupFlad = nullptr);
  };
} // namespace Kreator
