//
//  String.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "StringUtils.hpp"

namespace IKan::Utils
{
  std::string String::GetFileNameFromPath(const std::string &filePath)
  {
    // Get the last slash (/ or \) position
    auto lastSlash = filePath.find_last_of("/\\");
    
    // If last slash found then update the position next to last slash else make it at the start of file_path
    // May be entire file path is file name as there is no folder hierarchy
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    
    // Get the last Dot (.) position
    auto lastDot = filePath.find_last_of('.');
    
    // Count the number of character in file name
    // if we found the last dot then count will be from last slash till last dot
    // if we do not found the last dot then count will be from last slash to end of file path
    size_t count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
    
    // Return the string from last slash to count
    return filePath.substr(lastSlash, count);
  }
  
  std::string String::GetExtensionFromPath(const std::string &filePath)
  {
    // Get the last Dot (.) position
    auto lastDot = filePath.find_last_of('.');
    
    // If there are consecuting '..' then do not consider this as extenstion Dot as this might be change directory back ../..
    if (filePath.at(lastDot - 1) == '.')
    {
      lastDot = std::string::npos;
    }
    
    // Count the number of character in file extension
    // if we found the last dot then count will be from last dot till end of path
    // if we do not found the last dot then count will be 0 as we return the empty string
    size_t count = lastDot == std::string::npos ? 0 : filePath.size() - lastDot;
    
    // If last dot found then Return the string from last dot to count else return ""
    return lastDot != std::string::npos ? filePath.substr(lastDot, count) : "";
  }
  
  std::string String::GetDirectoryFromPath(const std::string &filePath)
  {
    // Get the last Dot (.) position to check is there a file present in the file path
    auto lastDot = filePath.find_last_of('.');
    
    // If there are consecuting '..' then do not consider this as extenstion Dot as this might be change directory back ../..
    if (filePath.at(lastDot - 1) == '.')
    {
      lastDot = std::string::npos;
    }
    
    // If there is no file present then entire file_path is directory
    // If there is a file present in path then dirctory is from begin till last slash
    return (lastDot != std::string::npos) ? filePath.substr(0, filePath.find_last_of("/\\")) : filePath;
  }
  
  std::string String::RemoveExtension(const std::string& filePath)
  {
    return filePath.substr(0, filePath.find_last_of('.'));
  }
  
  std::string String::ReadFromFile(const std::string& filePath)
  {
    // Output string after reading the file
    std::string result = "";
    
    // Read the file and store the data in string
    std::ifstream in(filePath, std::ios::in | std::ios::binary);
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
        IK_LOG_ERROR(LogModule::StringUtils, "Could not read from file '{0}'", filePath);
      }
    }
    else
    {
      IK_LOG_ERROR(LogModule::StringUtils, "Could not open file '{0}'", filePath);
    }
    return result;
  }
  
  const char* String::FindToken(const char* str, const std::string& token)
  {
    const char* t = str;
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
    const char* end = strstr(str, "}");
    
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
    size_t start = 0;
    // Find the first occurance of splitter
    size_t end = string.find_first_of(delimiters);
    
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
  
  std::string String::ToLowerCopy(const std::string_view string)
  {
    std::string result(string);
    ToLower(result);
    return result;
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
  
  std::string String::BytesToString(uint64_t bytes)
  {
    constexpr uint64_t GB = 1024 * 1024 * 1024;
    constexpr uint64_t MB = 1024 * 1024;
    constexpr uint64_t KB = 1024;
    
    char buffer[32];
    
    if (bytes > GB)
    {
      sprintf(buffer, "%.2f GB", (float)bytes / (float)GB);
    }
    else if (bytes > MB)
    {
      sprintf(buffer, "%.2f MB", (float)bytes / (float)MB);
    }
    else if (bytes > KB)
    {
      sprintf(buffer, "%.2f KB", (float)bytes / (float)KB);
    }
    else
    {
      sprintf(buffer, "%.2f bytes", (float)bytes);
    }
    
    return std::string(buffer);
  }
  
} // namespace IKan::Utils
