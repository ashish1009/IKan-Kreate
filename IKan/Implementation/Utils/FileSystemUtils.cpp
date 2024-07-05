//
//  FileSystemUtils.cpp
//  IKan
//
//  Created by Ashish . on 05/07/24.
//

#include "FileSystemUtils.hpp"

#include "Utils/StringUtils.hpp"

namespace IKan::Utils::FileSystem
{
  std::filesystem::path IKanAbsolute(const std::filesystem::path &path)
  {
    // Return if no path is passed
    if (path == "")
    {
      return "";
    }
    
    // Store the directories in vector (Split from ' /')
    std::vector<std::string> dirs = String::SplitString(std::filesystem::absolute(path), '/');

    // Store the result
    std::filesystem::path result = "/";

    // Iterate all the directories in file path
    for (std::string_view dir : dirs)
    {
      result = (dir == ".." ? result.parent_path() : result / dir);
    }
    
    return result;
  }
} // namespace IKan::Utils::FileSystem
