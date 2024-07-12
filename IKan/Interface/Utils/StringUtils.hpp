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
  std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters);
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
} // namespace IKan::Utils::String
