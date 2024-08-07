//
//  StringUtils.hpp
//  IKan
//
//  Created by Ashish . on 05/07/24.
//

#pragma once

namespace IKan::Utils::String
{
  /// This function breaks the string. Break points are stored in delimiter, any character in the delimeter will be
  /// cosnider as break points
  /// - Parameters:
  ///   - string: string to be broken
  ///   - delimiters: (break point of string). From here string will be split
  /// - Returns: Vector of strings after breaking from break point
  std::vector<std::string> SplitString(const std::string& string, std::string_view delimiters);
  /// This function breaks the string. Break points are stored in delimiter, any character in the delimeter will be
  /// cosnider as break points
  /// - Parameters:
  ///   - string: string to be broken
  ///   - delimiters: (break point of char). From here string will be split
  /// - Returns: Vector of strings after breaking from break point
  std::vector<std::string> SplitString(const std::string& string, const char delimiter);
  /// This function breaks the string from new line or spece or tab (\n \t)
  /// - Parameter string: string to be broken
  /// - Returns: Vector of strings after breaking from break point
  std::vector<std::string> Tokenize(const std::string& string);
  /// This function breaks the string from new line (\n)
  /// - Parameter string: string to be broken
  /// - Returns: Vector of strings after breaking from break point
  std::vector<std::string> GetLines(const std::string& string);
  
  /// This function reads the file and extract the file content in string
  /// - Parameter filePath: file path to be read
  /// - Returns: File content in string
  std::string ReadFromFile(const std::filesystem::path& filePath);
  
  /// This function finds a word/token from string and return the char pointer starting the token
  /// - Parameters:
  ///   - str: Base string from which token need to be searched:
  ///   - token: Token to be searched:
  const char* FindToken(std::string_view str, std::string_view token);
  /// This function finds a block of string from string and return the char pointer starting the strong 'str'
  /// - Parameters:
  ///   - str: Base string from which Block start pointer is returned:
  ///   - outPosition: Output position pointer of block:
  /// - Important: Returns the pointer at the start of any block. Block start is considered as '{'
  std::string GetBlock(std::string_view str, const char** outPosition);
  /// This function extracts the Code statement before ";"
  /// - Parameters:
  ///   - str: token to be extracted from outPosition
  ///   - outPosition: string pointer of the shader code, that will move to some offset offset is the ";" of the strcut
  std::string GetStatement(std::string_view str, const char** outPosition);

  /// This function returns the string as lower stirng
  /// - Parameter string: String to be lower
  std::string ToLower(const std::string& string);
  /// This function returns the copy string as lower
  /// - Parameter string: String to be lower
  std::string ToLowerCopy(const std::string_view& string);
  /// This function returns the string as upper stirng
  /// - Parameter string: String to be upper
  std::string ToUpper(const std::string_view& string);
  
  /// This function Removes the extention from file path
  /// - Parameter filePath: File path from which extension to be removed
  std::string RemoveExtension(const std::string& filePath);

} // namespace IKan::Utils::String
