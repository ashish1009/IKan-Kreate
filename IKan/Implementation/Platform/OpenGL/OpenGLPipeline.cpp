//
//  OpenGLPipeline.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include <glad/glad.h>
#include "OpenGLPipeline.hpp"

namespace IKan
{
#define PIPELINE_LOG(...) IK_LOG_DEBUG(LogModule::Pipeline, __VA_ARGS__);
  
  /// This function returns the Open GL Data type from user defined Shader data type
  /// - Parameter type: Shader data type
  [[maybe_unused]] static const char* ShaderDataTypeToString(ShaderDataType type)
  {
    switch (type)
    {
      case ShaderDataType::NoType:   return "NoType";
      case ShaderDataType::Float:    return "Float ";
      case ShaderDataType::Float2:   return "Float2";
      case ShaderDataType::Float3:   return "Float3";
      case ShaderDataType::Float4:   return "Float4";
      case ShaderDataType::Mat3:     return "Mat3  ";
      case ShaderDataType::Mat4:     return "Mat4  ";
      case ShaderDataType::Int:      return "Int   ";
      case ShaderDataType::Int2:     return "Int2  ";
      case ShaderDataType::Int3:     return "Int3  ";
      case ShaderDataType::Int4:     return "Int4  ";
      case ShaderDataType::Bool:     return "Bool  ";
    }
    
    IK_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
  }
  
  /// This function returns the Open GL Data type from user defined Shader data type
  /// - Parameter type: Shader data type
  [[maybe_unused]] static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
  {
    switch (type)
    {
      case ShaderDataType::NoType:   return 0;
      case ShaderDataType::Float:    return GL_FLOAT;
      case ShaderDataType::Float2:   return GL_FLOAT;
      case ShaderDataType::Float3:   return GL_FLOAT;
      case ShaderDataType::Float4:   return GL_FLOAT;
      case ShaderDataType::Mat3:     return GL_FLOAT;
      case ShaderDataType::Mat4:     return GL_FLOAT;
      case ShaderDataType::Int:      return GL_INT;
      case ShaderDataType::Int2:     return GL_INT;
      case ShaderDataType::Int3:     return GL_INT;
      case ShaderDataType::Int4:     return GL_INT;
      case ShaderDataType::Bool:     return GL_BOOL;
    }
    
    IK_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
  }

  OpenGLPipeline::OpenGLPipeline(const Specification& spec)
  : m_specification(spec)
  {
    glGenVertexArrays(1, &m_rendererID);
    glBindVertexArray(m_rendererID);
    
    PIPELINE_LOG("Creating Open GL Pipeline with Renderer ID {0}", m_rendererID);    
    Invalidate();
  }
  
  OpenGLPipeline::~OpenGLPipeline()
  {
    PIPELINE_LOG("Destroying Open GL Pipeline with Renderer ID {0}", m_rendererID);
    glDeleteVertexArrays(1, &m_rendererID);
  }
  
  void OpenGLPipeline::Invalidate()
  {
    // Layout reference
    const auto& layout = m_specification.layout;
    
    // Bind the Pipeline Vertex Array
    glBindVertexArray(m_rendererID);
    
    uint32_t index = 0;
    PIPELINE_LOG("  Vertex Attributes");
    PIPELINE_LOG("  -----------------");
    for (const auto& element : layout.GetElements())
    {
      PIPELINE_LOG("  {0} | {1}", ShaderDataTypeToString(element.type), element.name);

      switch (element.type)
      {
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
        case ShaderDataType::Bool:
        {
          glEnableVertexAttribArray(index);
          glVertexAttribIPointer(index, (int)element.count, ShaderDataTypeToOpenGLBaseType(element.type),
                                 (int)layout.GetStride(), (const void*)element.offset);
          index++;
          break;
        }
          
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        {
          glEnableVertexAttribArray(index);
          glVertexAttribPointer(index, (int)element.count, ShaderDataTypeToOpenGLBaseType(element.type),
                                element.normalized ? GL_TRUE : GL_FALSE, (int)layout.GetStride(), (const void*)element.offset);
          index++;
          break;
        }
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
        {
          uint32_t count = element.count;
          for (uint8_t i = 0; i < count; i++)
          {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, (int)count, ShaderDataTypeToOpenGLBaseType(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE, (int)layout.GetStride(),
                                  (const void*)(sizeof(float) * count * i));
            glVertexAttribDivisor(index, 1);
            index++;
          }
          break;
        }
        default:
        {
          IK_ASSERT(false, "Unknown ShaderDataType!");
        }
      } // switch (element.Type)
    } // For each elements
  }
} // namespace IKan
