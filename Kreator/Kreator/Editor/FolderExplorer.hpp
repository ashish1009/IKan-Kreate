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
    
    DELETE_ALL_CONSTRUCTORS(FolderExplorer);
  };
} // namespace Kreator
