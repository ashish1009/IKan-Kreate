//
//  FileSystem.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include <spdlog/spdlog.h>
#include "FileSystem.hpp"

namespace IKan::Utils
{
  bool FileSystem::Exists(const std::string &filePath)
  {
    return std::filesystem::exists(filePath);
  }
  
  void FileSystem::Rename(const std::string& oldName, const std::string& newName)
  {
    std::filesystem::rename(oldName, newName);
  }
  
  bool FileSystem::Rename(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
  {
    return Move(oldFilepath, newFilepath);
  }
  
  bool FileSystem::Move(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
  {
    if (FileSystem::Exists(newFilepath))
    {
      return false;
    }
    std::filesystem::rename(oldFilepath, newFilepath);
    return true;
  }
  
  bool FileSystem::Copy(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath)
  {
    if (FileSystem::Exists(newFilepath))
    {
      return false;
    }
    
    std::filesystem::copy(oldFilepath, newFilepath, std::filesystem::copy_options::recursive);
    return true;
  }
  
  bool FileSystem::Delete(const std::filesystem::path& filepath)
  {
    if (!FileSystem::Exists(filepath))
    {
      return false;
    }
    if (std::filesystem::is_directory(filepath))
    {
      return std::filesystem::remove_all(filepath) > 0;
    }
    return std::filesystem::remove(filepath);
  }
  
  std::string FileSystem::Absolute(const std::string &path)
  {
    return std::filesystem::absolute(path);
  }
  

  bool FileSystem::CreateDirectory(const std::string &direcoryPath)
  {
    return std::filesystem::create_directories(direcoryPath);
  }
  
  bool FileSystem::RenameFilename(const std::filesystem::path& oldFilepath, const std::string& newName)
  {
    std::filesystem::path newPath = fmt::format("{0}/{1}{2}",
                                                oldFilepath.parent_path().string().c_str(), // Parent Directory
                                                newName, // File Name
                                                oldFilepath.extension().c_str()); // Extension
    return Rename(oldFilepath, newPath);
  }
  
  bool FileSystem::MoveFile(const std::filesystem::path& filepath, const std::filesystem::path& dest)
  {
    return Move(filepath, dest / filepath.filename());
  }
  
  bool FileSystem::CopyFile(const std::filesystem::path& filepath, const std::filesystem::path& dest)
  {
    return Copy(filepath, dest / filepath.filename());
  }
  
} // namespace IKan::Utils
