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
    /// This function finds a word/token from string and return the char pointer starting the token
    /// - Parameters:
    ///   - str: Base string from which token need to be searched:
    ///   - token: Token to be searched:
    static const char*FindToken(const char* str, const std::string& token);
    /// This function finds a block of string from string and return the char pointer starting the strong 'str'
    /// - Parameters:
    ///   - str: Base string from which Block start pointer is returned:
    ///   - outPosition: Output position pointer of block:
    /// - Important: Returns the pointer at the start of any block. Block start is considered as '{'
    static std::string GetBlock(const char* str, const char** outPosition);
    
    /// This function extracts the Code statement before ";"
    /// - Parameters:
    ///   - str: token to be extracted from outPosition
    ///   - outPosition: string pointer of the shader code, that will move to some offset offset is the ";" of the strcut
    static std::string GetStatement(const char* str, const char** outPosition);
    /// This function breaks the string. Break points are stored in delimiter, any character in the
    /// delimeter will be cosnider as break points
    /// - Parameters:
    ///   - string: broken strings vector
    ///   - delimiters: (break point of string). From here string will be split
    static std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters);
    /// This function breaks the string. Break points are stored in delimiter, any character in the delimeter
    /// will be cosnider as break points
    /// - Parameters:
    ///   - string: broken strings vector
    ///   - delimiters: (break point of char). From here string will be split
    static std::vector<std::string> SplitString(const std::string& string, const char delimiter);
    /// This function breaks the string. Break points are stored in delimiter, any character in the delimeter
    /// will be cosnider as break points Split point (\t\n).
    /// - Parameter string: broken strings vector
    static std::vector<std::string> Tokenize(const std::string& string);
    /// This function breaks the string. Break points are stored in delimiter, any character in the delimeter
    /// will be cosnider as break points Split point (\n).
    /// - Parameter string: broken strings vector
    static std::vector<std::string> GetLines(const std::string& string);
    
    /// This function returns the string as lower stirng
    /// - Parameter string: String to be lower
    static std::string ToLower(const std::string& string);
    /// This function returns the copy string as lower
    /// - Parameter string: String to be lower
    static std::string ToLowerCopy(const std::string_view& string);
    /// This function returns the string as upper stirng
    /// - Parameter string: String to be upper
    static std::string ToUpper(const std::string_view& string);

    /// This function Removes the extention from file path
    /// - Parameter filePath: File path from which extension to be removed
    static std::string RemoveExtension(const std::string& filePath);
  };
} // namespace IKan::Utils
