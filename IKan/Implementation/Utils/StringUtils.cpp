//
//  StringUtils.cpp
//  IKan
//
//  Created by Ashish . on 05/07/24.
//

#include "StringUtils.hpp"

namespace IKan::Utils::String
{
  std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters)
  {
    IK_PROFILE();
    // Return empty vector if no delimiters are given
    if (delimiters.size() == 0)
    {
      return {};
    }
    
    // Stores the result
    std::vector<std::string> result;

    // Find first occurance of delimiter
    size_t end = string.find_first_of(delimiters);
    
    // Iterate entire string
    size_t start = 0;
    while (end <= std::string::npos)
    {
      // Store the splited string in vector
      std::string token = string.substr(start, end - start);
      if (!token.empty())
      {
        result.emplace_back(token);
      }
      
      // if we reached end of string
      if (end == std::string::npos)
      {
        break;
      }
      
      start = end + 1;
      end = string.find_first_of(delimiters, start);
    }
    return result;
  }
  
  std::vector<std::string> SplitString(const std::string& string, const char delimiter)
  {
    return SplitString(string, std::string(1, delimiter));
  }
  
  std::vector<std::string> Tokenize(const std::string& string)
  {
    return SplitString(string, " \t\n");
  }
  
  std::vector<std::string> GetLines(const std::string& string)
  {
    return SplitString(string, "\n");
  }
  
  std::string ReadFromFile(const std::filesystem::path& filepath)
  {
    IK_PROFILE();
    
    // File content to be stored in this string
    std::string result {};
    
    // Read the file and store the data in string
    std::ifstream in(filepath.string(), std::ios::in | std::ios::binary);
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
} // namespace IKan::Utils::String
