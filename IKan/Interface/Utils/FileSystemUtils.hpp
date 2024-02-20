//
//  FileSystemUtils.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

namespace IKan::Utils
{
  /// This Static Singleton class is wrapper utility for managing std::filesystem APIs
  class FileSystem
  {
  public:
    /// This function returns the absolute File/Directory path for Kreator
    /// - Parameter path: Current Relative File/Directory path passed
    static std::filesystem::path IKanAbsolute(const std::filesystem::path& path);
    /// This function Copies the File/Directory
    /// - Parameters:
    ///   - oldFilepath: Old Name of File/Directory
    ///   - newFilepath: New Name of File/Directory
    static bool Copy(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath);
    /// This function rename the File/Folder
    /// - Parameters:
    ///   - oldName: Old Name of File/Folder
    ///   - newName: New Name of File/Folder
    static bool Rename(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath);
    /// This function Moves the File/Directory
    /// - Parameters:
    ///   - oldFilepath: Old Name of File/Directory
    ///   - newFilepath: New Name of File/Directory
    static bool Move(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath);
    /// This function moves the file
    /// - Parameters:
    ///   - filepath: source
    ///   - dest: destination
    static bool MoveFile(const std::filesystem::path& filepath, const std::filesystem::path& dest);
    /// This function deletes the File/Directory
    /// - Parameter filepath: File/Directory path to be deleted
    static bool Delete(const std::filesystem::path& filepath);

  };
} // namespace IKan::Utils
