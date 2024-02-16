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
    /// - Parameters:
    static void Select();
    /// This function set the popup to show open file widgwet
    static void Open();
    /// This function set the popup to show save file widgwet
    static void Save();

    DELETE_ALL_CONSTRUCTORS(FolderExplorer);
  };
} // namespace Kreator
