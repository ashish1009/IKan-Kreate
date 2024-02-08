//
//  OpenGLShader.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include <glad/glad.h>
#include "Renderer/Graphics/Shader.hpp"
#include "Platform/OpenGL/OpenGLShaderUniforms.hpp"

namespace IKan
{
  // Max Supported Shaders
  static constexpr uint32_t MaxShaderSupported = 3;
  
  /// Interface class for storing Shader
  class OpenGLShader : public Shader
  {
  public:
    /// This is the consturctor of frame buffer the create the buffer with the specification
    /// - Parameter shaderFilePath: Shader Code file path
    OpenGLShader(const std::filesystem::path& shaderFilePath);
    /// This is the default frame buffer destructor that delete the buffer
    ~OpenGLShader();
    
  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function reads the shader code in string and store all the shader present in the file in a map to be used
    /// later by compiler.
    /// - Parameter sourceString: shader code as string
    void PreprocessShader(const std::string& sourceString);
    /// This functions compiles all the shader codes and store their ID in Program ID (renderer_id).
    void Compile();
    /// This function reads and parse the shader code and extracts the structure and uniforms and store them in data
    void Parse();

    /// This function parses the Uniforms that are structure in shader. It will just store the structures only
    /// - Parameters:
    ///   - block: block code of shader
    ///   - domain: type of shader
    void ParseUniformStruct(const std::string& block, ShaderDomain domain);
    /// This function parses the Uniform that are fundamental types (Not struct)
    /// - Parameters:
    ///   - statement: block fo code of shader
    ///   - domain domain of shader
    void ParseUniform(const std::string& statement, ShaderDomain domain);

    /// This function finds the structure stored in shader
    /// - Parameter name: Name of structure
    ShaderStruct* FindStruct(const std::string& name);

    // Member Variables ---------------------------------------------------------------------------------------------
    RendererID m_rendererID;
    std::filesystem::path m_filePath{};
    std::string m_name{};
    std::unordered_map<GLenum /* GL Shader type */, std::string /* Shader code */> m_shaderSourceCodeMap;
    
    std::vector<ShaderStruct*> m_structs; // Stores the structure in the shader
    std::vector<ShaderResourceDeclaration*> m_resources; // Stores the resources of shader like sampler 2D

    Ref<OpenGLShaderUniformBufferDeclaration> m_vsMaterialUniformBuffer; // Uniform data buffer of vertex shader
    Ref<OpenGLShaderUniformBufferDeclaration> m_fsMaterialUniformBuffer; // Uniform data buffer of pixel shader
    Ref<OpenGLShaderUniformBufferDeclaration> m_gsMaterialUniformBuffer; // Uniform data buffer of geometry shader
  };
} // namespace IKan
