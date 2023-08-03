//
//  FileSystem.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

#include <filesystem>

namespace IKan::Utils
{
  /// This Static Singleton class is wrapper utility for managing std::filesystem APIs
  class FileSystem
  {
  public:
    // Common APIs --------------------------------------------------------------------------------------------------
    /// This function checks the if File/Directory is present in the disk or not
    /// - Parameter filePath: File/Directory path
    static bool Exists(const std::string& filePath);
    /// This function rename the File/Directory
    /// - Parameters:
    ///   - oldName: Old Name of File/Directory
    ///   - newName: New Name of File/Directory
    static void Rename(const std::string& oldName, const std::string& newName);
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
    /// This function Copies the File/Directory
    /// - Parameters:
    ///   - oldFilepath: Old Name of File/Directory
    ///   - newFilepath: New Name of File/Directory
    static bool Copy(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath);
    /// This function deletes the File/Directory
    /// - Parameter filepath: File/Directory path to be deleted
    static bool Delete(const std::filesystem::path& filepath);
    /// This function returns the absolute File/Directory path
    /// - Parameter path: Current Relative File/Directory path passed
    static std::string Absolute(const std::string& path);
    /// This function returns the absolute File/Directory path for Kreator
    /// - Parameter path: Current Relative File/Directory path passed
    static std::string KreatorAbsolute(const std::string &path);

    // Directory ----------------------------------------------------------------------------------------------------
    /// This function creates the directory. It creates all the folder in hierarchy (if any of them do not exists)
    /// - Parameter direcoryPath: Direcory path
    /// - Note: Path should be without file name, just Direcotries
    static bool CreateDirectory(const std::string& direcoryPath);
    
    // File ---------------------------------------------------------------------------------------------------------
    /// This function rename the file
    /// - Parameters:
    ///   - oldFilepath: Old Name of file
    ///   - newName: New Name of file
    static bool RenameFilename(const std::filesystem::path& oldFilepath, const std::string& newName);
    /// This function moves the file
    /// - Parameters:
    ///   - filepath: source
    ///   - dest: destination
    static bool MoveFile(const std::filesystem::path& filepath, const std::filesystem::path& dest);
    /// This function copies the file
    /// - Parameters:
    ///   - filepath: source
    ///   - dest: destination
    static bool CopyFile(const std::filesystem::path& filepath, const std::filesystem::path& dest);
    
    MAKE_PURE_STATIC(FileSystem);
  }; 
} // namespace IKan::Utils
