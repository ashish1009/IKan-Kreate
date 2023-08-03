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
  };
} // namespace IKan
