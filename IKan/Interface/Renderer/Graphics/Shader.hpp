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
  // Forward Declaration
  class ShaderUniformBufferDeclaration;
  class ShaderResourceDeclaration;

  /// This class stores the interface for creating, compiling the shader for rendering
  class Shader
  {
  public:
    /// Default virtual destructor
    virtual ~Shader() = default;
    
    /// This function binds the current Shader to the renderer
    virtual void Bind() const = 0;
    /// This function unbinds the current Shader from the renderer
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
    
    /// This function returns the Renderer ID of Shader
    virtual RendererID GetRendererID() const = 0;
    
    /// This function returns the Shader file path
    virtual const std::filesystem::path& GetFilePath() const = 0;
    /// This function returns the Shader Name
    virtual const std::string& GetName() const = 0;
    
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
    virtual void SetUniformMat4Array(std::string_view name, const glm::mat4& values, uint32_t count) = 0;
    
    /// This functions uploads the Matrix 4x4 value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - value: Value of Uniform
    virtual void SetUniformMat4(std::string_view name, const glm::mat4& value) = 0;
    /// This functions uploads the Matrix 3x3 value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - value: Value of Uniform
    virtual void SetUniformMat3(std::string_view name, const glm::mat3& value) = 0;
    
    /// This functions uploads the Flaot value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - value: Value of Uniform
    virtual void SetUniformFloat1(std::string_view name, float value) = 0;
    /// This functions uploads the Vec2 value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - value: Value of Uniform
    virtual void SetUniformFloat2(std::string_view name, const glm::vec2& value) = 0;
    /// This functions uploads the Vec3 value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - value: Value of Uniform
    virtual void SetUniformFloat3(std::string_view name, const glm::vec3& value) = 0;
    /// This functions uploads the Vec4 value to shader
    /// - Parameters:
    ///   - name: Name of Uniform
    ///   - value: Value of Uniform
    virtual void SetUniformFloat4(std::string_view name, const glm::vec4& value) = 0;
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
