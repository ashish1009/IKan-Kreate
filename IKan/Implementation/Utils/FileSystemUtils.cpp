//
//  FileSystemUtils.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "FileSystemUtils.hpp"

namespace IKan::Utils
{
  std::filesystem::path FileSystem::IKanAbsolute(const std::filesystem::path &path)
  {
    IK_PROFILE();
    if (path == "")
    {
      return "";
    }
    
    std::vector<std::string> dirs {Utils::String::SplitString(std::filesystem::absolute(path), '/')};
    std::filesystem::path result {"/"};
    for (const std::string& dir : dirs)
    {
      result = (dir == ".." ? result.parent_path() : result / dir);
    }
    return result;
  }
} // namespace IKan::Utils
