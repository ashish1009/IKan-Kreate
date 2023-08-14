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
    // Bind the Pipeline Vertex Array
    glBindVertexArray(m_rendererID);
    
    AddAttrib(m_specification.vertexLayout);
  }
  
  void OpenGLPipeline::AddAttrib(const BufferLayout& layout)
  {
#ifdef IK_ENABLE_LOG
    // Store the vertex attribute
    Table::Data tableData;
    tableData.title = "Vertex Attributes in Pipeline (Stride : " + std::to_string(layout.GetStride()) + ")";
    tableData.numColumns = 4;
    tableData.columnHeaders =
    {
      "Name", "Type", "Offset", "Size"
    };
    Table verticesData(tableData);
#endif
    
    uint32_t index = 0;
    for (const auto& element : layout.GetElements())
    {
  #ifdef IK_ENABLE_LOG
      verticesData.AddRow({
        element.name, ShaderDataTypeToString(element.type),
        std::to_string(element.offset),
        std::to_string(element.size)
      });
  #endif
      
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
#ifdef IK_ENABLE_LOG
    verticesData.Dump(Log::Level::Debug, "Pipeline");
#endif
  }
  
  void OpenGLPipeline::Bind() const
  {
    glBindVertexArray(m_rendererID);
  }
  
  void OpenGLPipeline::Unbind() const
  {
    glBindVertexArray(0);
  }
  
  RendererID OpenGLPipeline::GetRendererID() const
  {
    return m_rendererID;
  }
  
  OpenGLPipeline::Specification& OpenGLPipeline::GetSpecification()
  {
    return m_specification;
  }
  
  const OpenGLPipeline::Specification& OpenGLPipeline::GetSpecification() const
  {
    return m_specification;
  };
} // namespace IKan
