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
#ifdef __APPLE__
  /// Maximum Texture slot supported by Senderer Shader Current Open GL Version 4.1 supports only 16 Texture slot in Shader
  static constexpr uint32_t MaxTextureSlotsInShader = 16;
#else
  static constexpr uint32_t MaxTextureSlotsInShader = 0;
#endif

  // Forward Declaration
  class ShaderUniformBufferDeclaration;
  class ShaderResourceDeclaration;

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
    
    /// This function binds the Current Shader to GPU
    virtual void Bind() const = 0;
    /// This function unbinds the Current Shader to GPU
    virtual void Unbind() const = 0;
    
    /// This function set the vertex shader buffer data
    /// - Parameter buffer: buffer data
    virtual void SetVSMaterialUniformBuffer(const Buffer& buffer) = 0;
    /// This function set the fragment shader buffer data
    /// - Parameter buffer: buffer data
    virtual void SetFSMaterialUniformBuffer(const Buffer& buffer) = 0;
    /// This function set the geomatry shader buffer data
    /// - Parameter buffer: buffer data
    virtual void SetGSMaterialUniformBuffer(const Buffer& buffer) = 0;
    
    /// This function returns the Name of Shader
    virtual const std::string& GetName() const = 0;
    /// This function returns the File Path of Shader
    virtual const std::string& GetFilePath() const = 0;
    /// This function returns the Renderer ID of Shader
    virtual RendererID GetRendererID() const = 0;
    
    /// This function returns true if have the vertex shader buffer data
    virtual bool HasVSMaterialUniformBuffer() const = 0;
    /// This function returns true if have the fragment shader buffer data
    virtual bool HasFSMaterialUniformBuffer() const = 0;
    /// This function returns true if have the geomatry shader buffer data
    virtual bool HasGSMaterialUniformBuffer() const = 0;
    
    /// This function resturns the vertex Shader buffer data
    virtual const ShaderUniformBufferDeclaration& GetVSMaterialUniformBuffer() const = 0;
    /// This function resturns the fragment Shader buffer data
    virtual const ShaderUniformBufferDeclaration& GetFSMaterialUniformBuffer() const = 0;
    /// This function resturns the geomatry Shader buffer data
    virtual const ShaderUniformBufferDeclaration& GetGSMaterialUniformBuffer() const = 0;
    
    /// This function returns all the resources
    virtual const std::vector<ShaderResourceDeclaration*>& GetResources() const = 0;
    
    // NOTE: For Int and Int arrar (for Sampler 2D) Unform is setting while compiling the shader so no need to call this explicitly
    
    /// This functions uploads the Matrix 4x4 array value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - values: Value of Uniform
    ///   - count: Size of Mat4 Array
    virtual void SetUniformMat4Array(const std::string& name, const glm::mat4& values, uint32_t count) = 0;
    
    /// This functions uploads the Matrix 4x4 value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - value: Value of Uniform
    virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) = 0;
    /// This functions uploads the Matrix 3x3 value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - value: Value of Uniform
    virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) = 0;
    
    /// This functions uploads the Flaot value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - value: Value of Uniform
    virtual void SetUniformFloat1(const std::string& name, float value) = 0;
    /// This functions uploads the Vec2 value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - value: Value of Uniform
    virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
    /// This functions uploads the Vec3 value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - value: Value of Uniform
    virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
    /// This functions uploads the Vec4 value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - value: Value of Uniform
    virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) = 0;
    
    /// This function creates the shader instance based on the renderer API supported by IKan
    /// - Parameter shaderFilePath: Shader file full path
    [[nodiscard]] static Ref<Shader> Create(const std::string& shaderFilePath);
    
    /// This function deletes all the Texture present int the map
    static void ResetLibrary();

  private:
    inline static std::unordered_map<std::string /* File Path */, Ref<Shader>> s_shaders;
  };
} // namespace IKan
