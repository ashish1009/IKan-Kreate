//
//  RendererBuffers.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "RendererBuffers.hpp"
#include "Platform/OpenGL/OpenGLRendererBuffers.hpp"

namespace IKan
{
  namespace BufferUtils
  {
    // Utils ----------------------------------------------------------------------------------------------------------
    /// This function returns the number of bytes in Shader data types. (Size)
    /// - Parameter type: Shader data type
    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
      switch (type)
      {
        case ShaderDataType::NoType : return 0;
        case ShaderDataType::Float  : return 4;
        case ShaderDataType::Float2 : return 4 * 2;
        case ShaderDataType::Float3 : return 4 * 3;
        case ShaderDataType::Float4 : return 4 * 4;
        case ShaderDataType::Mat3   : return 4 * 3 * 3;
        case ShaderDataType::Mat4   : return 4 * 4 * 5;
        case ShaderDataType::Int    : return 4;
        case ShaderDataType::Int2   : return 4 * 2;
        case ShaderDataType::Int3   : return 4 * 3;
        case ShaderDataType::Int4   : return 4 * 4;
        case ShaderDataType::Bool   : return 1;
      }
      IK_ASSERT(false, "Invalid Type");
      return 0;
    }
    
    /// This function returns the number of same data type elements. (Count)
    /// - Parameter type: Shader data type
    static uint32_t GetElementCount(ShaderDataType type)
    {
      switch (type)
      {
        case ShaderDataType::NoType : return 0;
        case ShaderDataType::Float  : return 1;
        case ShaderDataType::Float2 : return 2;
        case ShaderDataType::Float3 : return 3;
        case ShaderDataType::Float4 : return 4;
        case ShaderDataType::Mat3   : return 3;
        case ShaderDataType::Mat4   : return 4;
        case ShaderDataType::Int    : return 1;
        case ShaderDataType::Int2   : return 2;
        case ShaderDataType::Int3   : return 3;
        case ShaderDataType::Int4   : return 4;
        case ShaderDataType::Bool   : return 1;
      }
      IK_ASSERT(false, "Unkown Shader datatype!! ");
    }
  } // namespace BufferUtils
  
  // Buffer Element --------------------------------------------------------------------------------------------------
  BufferElement::BufferElement(const std::string& name, ShaderDataType type, bool normalized)
  : name(name), type(type), size(BufferUtils::ShaderDataTypeSize(type)), count(BufferUtils::GetElementCount(type)),
  offset(0), normalized(normalized)
  {
    
  }

  Ref<IndexBuffer> IndexBuffer::CreateWithSize(void *data, uint32_t size)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl: return CreateRef<OpenGLIndexBuffer>(data, size);
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
  
  Ref<IndexBuffer> IndexBuffer::CreateWithCount(void *data, uint32_t count)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl: return CreateRef<OpenGLIndexBuffer>(data, count * SizeOfSingleIndices);
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
} // namespace IKan
