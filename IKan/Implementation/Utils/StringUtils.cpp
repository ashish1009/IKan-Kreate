//
//  StringUtils.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "StringUtils.hpp"

namespace IKan::Utils
{
  std::string String::ReadFromFile(const std::filesystem::path& filePath)
  {
    IK_PROFILE();
    
    // Output string after reading the file
    std::string result = "";
    
    // Read the file and store the data in string
    std::ifstream in(filePath.string(), std::ios::in | std::ios::binary);
    if (in)
    {
      in.seekg(0, std::ios::end);
      size_t size = (size_t)in.tellg();
      if (-1 != (int32_t)size)
      {
        result.resize(size);
        in.seekg(0, std::ios::beg);
        in.read(&result[0], (long)size);
        in.close();
      }
      else
      {
        assert(false);
      }
    }
    else
    {
      assert(false);
    }
    return result;
  }
} // namespace IKan::Utils
