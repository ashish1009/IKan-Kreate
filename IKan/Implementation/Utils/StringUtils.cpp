//
//  StringUtils.cpp
//  IKan
//
//  Created by Ashish . on 05/07/24.
//

#include "StringUtils.hpp"

namespace IKan::Utils::String
{
  std::vector<std::string> SplitString(const std::string& string, std::string_view delimiters)
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
  
  const char* FindToken(std::string_view str, std::string_view token)
  {
    IK_PROFILE();
    const char* t {str.data()};
    while ((t = strstr(t, token.data())))
    {
      // Check if left character of token is empty
      // using '/' as indicator of first character of line
      bool left = str == t or isspace(t[-1]) or t[-1] == '/';
      
      // Check if right character of token is empty
      bool right = !t[token.size()] or isspace(t[token.size()]);
      
      // Checking token is whole word
      if (left and right)
      {
        return t;
      }
      
      t += token.size();
    }
    return nullptr;
  }
  
  std::string GetBlock(std::string_view str, const char** outPosition)
  {
    IK_PROFILE();
    // get the end block brace of the string code
    const char* end {strstr(str.data(), "}")};
    
    // if not found any end block statement "}" then return the same string
    if (!end)
    {
      return str.data();
    }
    
    // Update the out_position pointer after the end block
    // offset the shader string to the end of current strcut
    if (outPosition)
    {
      *outPosition = end;
    }
    
    uint32_t length = (uint32_t)(end - str.data() + 1);
    
    // return the substring from start to last "}"
    return std::string(str.data(), length);
  }
  
  std::string GetStatement(std::string_view str, const char** outPosition)
  {
    IK_PROFILE();
    // get the end block brace of the string code
    const char* end = strstr(str.data(), ";");
    
    // if not found any end bkock statement ";" then return the same string
    if (!end)
    {
      return str.data();
    }
    
    // Update the out_position pointer after the end block
    // offset the shader string to the end of current strcut
    if (outPosition)
    {
      *outPosition = end;
    }
    
    uint32_t length = (uint32_t)(end - str.data() + 1);
    // return the substring from start to last ";"
    return std::string(str.data(), length);
  }
} // namespace IKan::Utils::String
