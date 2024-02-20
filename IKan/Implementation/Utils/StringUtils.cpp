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
    std::string result {};
    
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
  
  const char* String::FindToken(const char* str, const std::string& token)
  {
    const char* t {str};
    while ((t = strstr(t, token.c_str()))) {
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
  
  std::string String::GetBlock(const char* str, const char** outPosition) {
    // get the end block brace of the string code
    const char* end {strstr(str, "}")};
    
    // if not found any end bkock statement "}" then return the same string
    if (!end)
    {
      return str;
    }
    
    // Update the out_position pointer after the end block
    // offset the shader string to the end of current strcut
    if (outPosition)
    {
      *outPosition = end;
    }
    
    uint32_t length = (uint32_t)(end - str + 1);
    // return the substring from start to last "}"
    return std::string(str, length);
  }
  
  std::string String::GetStatement(const char* str, const char** outPosition)
  {
    // get the end block brace of the string code
    const char* end = strstr(str, ";");
    
    // if not found any end bkock statement ";" then return the same string
    if (!end)
    {
      return str;
    }
    
    // Update the out_position pointer after the end block
    // offset the shader string to the end of current strcut
    if (outPosition)
    {
      *outPosition = end;
    }
    
    uint32_t length = (uint32_t)(end - str + 1);
    // return the substring from start to last ";"
    return std::string(str, length);
  }
  
  std::vector<std::string> String::SplitString(const std::string& string, const std::string& delimiters)
  {
    size_t start {0};
    // Find the first occurance of splitter
    size_t end {string.find_first_of(delimiters)};
    
    // Stores the result
    std::vector<std::string> result;
    
    // while end of string
    while (end <= std::string::npos)
    {
      // Store the split block
      std::string token = string.substr(start, end - start);
      if (!token.empty())
      {
        result.push_back(token);
      }
      
      if (end == std::string::npos)
      {
        break;
      }
      
      start = end + 1;
      end = string.find_first_of(delimiters, start);
    }
    return result;
  }
  
  std::vector<std::string> String::SplitString(const std::string& string, const char delimiter)
  {
    return SplitString(string, std::string(1, delimiter));
  }
  
  std::vector<std::string> String::Tokenize(const std::string& string)
  {
    return SplitString(string, " \t\n");
  }
  
  std::vector<std::string> String::GetLines(const std::string& string)
  {
    return SplitString(string, "\n");
  }
  
  std::string String::ToLower(const std::string& string)
  {
    std::string result;
    for (const auto& character : string)
    {
      result += std::tolower(character);
    }
    return result;
  }
  
  std::string String::ToLowerCopy(const std::string_view& string)
  {
    std::string result(string);
    return ToLower(result);
  }
  
  std::string String::ToUpper(const std::string_view& string)
  {
    std::string result;
    for (const auto& character : string)
    {
      result += std::toupper(character);
    }
    
    return result;
  }

  std::string String::RemoveExtension(const std::string& filePath)
  {
    return filePath.substr(0, filePath.find_last_of('.'));
  }

} // namespace IKan::Utils
