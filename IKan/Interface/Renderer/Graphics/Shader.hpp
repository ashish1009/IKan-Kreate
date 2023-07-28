//
//  Shader.hpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#pragma once

#include "Renderer/Renderer.hpp"

namespace IKan
{
  /// This is the interface class to compile Shader code
  /// - Note: Format of shader file should be
  /// #type vertex
  ///   ...... Vertex shader code
  /// #type fragment
  ///   ...... Fragment shader code
  /// #type geometry
  ///   ...... Geoometry shader code
  /// And so on ......
  /// - Important: Only 3 shader supported by IKan : Vertex, Fragment and Geometry
  class Shader
  {
  public:
    /// This is the virtual default destructor for Shader
    virtual ~Shader() = default;
    
    /// This function creates the shader instance based on the renderer API supported by IKan
    /// - Parameter shaderFilePath: Shader file full path
    [[nodiscard]] static Ref<Shader> Create(const std::string& shaderFilePath);
  };
} // namespace IKan
