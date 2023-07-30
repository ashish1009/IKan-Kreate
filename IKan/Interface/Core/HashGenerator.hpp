//
//  HashGenerator.hpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#pragma once

namespace IKan
{
  /// Wrapper to generate Hash number
  class Hash
  {
  public:
    /// This function generates the Hash number for const char
    /// - Parameter str: char data
    static uint32_t GenerateFNV(const char* str);
    /// This function generates the Hash number for string
    /// - Parameter str: String Data
    static uint32_t GenerateFNV(const std::string& string);
  };
} // namespace IKan
