//
//  Shader.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Factories.hpp"

namespace IKan
{
  /// This class stores the interface for creating, compiling the shader for rendering
  class Shader
  {
  public:
    /// Default virtual destructor
    virtual ~Shader();
  };
  
  /// This class stores the loaded compiled shaders in a library (map)
  class ShaderLibrary
  {
  public:
    /// This function returns the pre loaded shader from library
    /// - Parameter shaderFilePath: shader file path
    /// - Note: Creates new if not present in map
    static Ref<Shader> Get(const std::filesystem::path& shaderFilePath);
    /// This function clear the shaders loaded in library
    static void Clear();
    
    DELETE_ALL_CONSTRUCTORS(ShaderLibrary);
    
  private:
    inline static std::unordered_map<std::filesystem::path, Ref<Shader>> s_shaders;
  };
} // namespace IKan
