//
//  FileSystemUtils.hpp
//  IKan
//
//  Created by Ashish . on 05/07/24.
//

#pragma once

namespace IKan::Utils::FileSystem
{
  /// This function returns the absolute File/Directory path for IKan
  /// - Parameter path: Current Relative File/Directory path passed
  std::filesystem::path IKanAbsolute(const std::filesystem::path& path);
} // namespace IKan::Utils
