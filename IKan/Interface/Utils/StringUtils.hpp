//
//  StringUtils.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

namespace IKan::Utils
{
  /// This is the wrapper class for String Utilities functions
  class String
  {
  public:
    /// This function read the file from path given in 'filePath' and store all the date in the file in a string.
    /// - Returns the content of this give file as string
    /// - Parameter filePath: File path to be read:
    static std::string ReadFromFile(const std::filesystem::path& filePath);
  };
} // namespace IKan::Utils
