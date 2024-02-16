//
//  FolderExplorer.cpp
//  Kreator
//
//  Created by Ashish . on 16/02/24.
//

#include "FolderExplorer.hpp"

namespace Kreator
{
  void FolderExplorer::Initialize()
  {
    IK_PROFILE();
    IK_LOG_TRACE("FolderExplorer", "Initialising the FolderExplorer textures");
  }
  
  void FolderExplorer::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_WARN("FolderExplorer", "Shutting Down the FolderExplorer textures");
  }
} // namespace Kreator
