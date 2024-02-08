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
    static std::string IKanAbsolute(const std::filesystem::path& path);
  };
} // namespace IKan::Utils
