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
  bool FileSystem::Copy(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
  {
    if (std::filesystem::exists(newFilepath))
    {
      return false;
    }
    
    std::filesystem::copy(oldFilepath, newFilepath, std::filesystem::copy_options::recursive);
    return true;
  }
  bool FileSystem::Rename(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
  {
    return Move(oldFilepath, newFilepath);
  }
  bool FileSystem::Move(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
  {
    if (std::filesystem::exists(newFilepath))
    {
      return false;
    }
    std::filesystem::rename(oldFilepath, newFilepath);
    return true;
  }
  bool FileSystem::MoveFile(const std::filesystem::path& filepath, const std::filesystem::path& dest)
  {
    return Move(filepath, dest / filepath.filename());
  }
  bool FileSystem::Delete(const std::filesystem::path& filepath)
  {
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
} // namespace IKan::Utils
