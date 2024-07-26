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
  bool Copy(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
  {
    IK_PROFILE();
    if (std::filesystem::exists(newFilepath))
    {
      return false;
    }
    
    std::filesystem::copy(oldFilepath, newFilepath, std::filesystem::copy_options::recursive);
    return true;
  }
  bool Rename(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
  {
    IK_PROFILE();
    return Move(oldFilepath, newFilepath);
  }
  bool Move(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
  {
    IK_PROFILE();
    if (std::filesystem::exists(newFilepath))
    {
      return false;
    }
    std::filesystem::rename(oldFilepath, newFilepath);
    return true;
  }
  bool MoveFile(const std::filesystem::path& filepath, const std::filesystem::path& dest)
  {
    IK_PROFILE();
    return Move(filepath, dest / filepath.filename());
  }
  bool Delete(const std::filesystem::path& filepath)
  {
    IK_PROFILE();
    if (!std::filesystem::exists(filepath))
    {
      return false;
    }
    if (std::filesystem::is_directory(filepath))
    {
      return std::filesystem::remove_all(filepath) > 0;
    }
    return std::filesystem::remove(filepath);
  }
} // namespace IKan::Utils::FileSystem
