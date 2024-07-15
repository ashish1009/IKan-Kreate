//
//  OpenGLPipeline.cpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#include "OpenGLPipeline.hpp"

#include <glad/glad.h>

namespace IKan
{
#define PIPELINE_LOG(...) IK_LOG_DEBUG(LogModule::Pipeline, __VA_ARGS__);

  namespace PipelineUtils
  {
    /// This function returns the Open GL Data type from user defined Shader data type
    /// - Parameter type: Shader data type
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
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
  } // namespace PipelineUtils
  
  OpenGLPipeline::OpenGLPipeline(const PipelineSpecification& spec)
  : m_specification(spec)
  {
    Renderer::Submit([this](){
      IK_PROFILE();
      glGenVertexArrays(1, &m_rendererID);
      
      PIPELINE_LOG("Creating Open GL Pipeline (ID : {0}).", m_rendererID);
      m_specification.vertexLayout.DebugLog();
      
      glBindVertexArray(m_rendererID);

      uint32_t index = 0;
      for (const BufferElement& element : m_specification.vertexLayout.GetElements())
      {
        switch (element.type)
        {
          case ShaderDataType::Int:
          case ShaderDataType::Int2:
          case ShaderDataType::Int3:
          case ShaderDataType::Int4:
          case ShaderDataType::Bool:
          {
            glEnableVertexAttribArray(index);
            glVertexAttribIPointer(index, (int)element.count, PipelineUtils::ShaderDataTypeToOpenGLBaseType(element.type),
                                   (int)m_specification.vertexLayout.GetStride(), (const void*)element.offset);
            index++;
            break;
          }
            
          case ShaderDataType::Float:
          case ShaderDataType::Float2:
          case ShaderDataType::Float3:
          case ShaderDataType::Float4:
          {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, (int)element.count, PipelineUtils::ShaderDataTypeToOpenGLBaseType(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE, (int)m_specification.vertexLayout.GetStride(), (const void*)element.offset);
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
              glVertexAttribPointer(index, (int)count, PipelineUtils::ShaderDataTypeToOpenGLBaseType(element.type),
                                    element.normalized ? GL_TRUE : GL_FALSE, (int)m_specification.vertexLayout.GetStride(),
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
      }
    });
  }
  
  OpenGLPipeline::~OpenGLPipeline()
  {
    Renderer::Submit([this](){
      IK_PROFILE();
      PIPELINE_LOG("Destroying Open GL Pipeline (ID : {0}).", m_rendererID);
      glDeleteVertexArrays(1, &m_rendererID);
    });
    
    // Reset the shader on destruction
    m_specification.shader.reset();
  }
  
  void OpenGLPipeline::Bind() const
  {
    Renderer::Submit([this](){
      glBindVertexArray(m_rendererID);
    });
  }
  
  void OpenGLPipeline::Unbind() const
  {
    Renderer::Submit([](){
      glBindVertexArray(0);
    });
  }
  
  RendererID OpenGLPipeline::GetRendererID() const
  {
    return m_rendererID;
  }
  
  const PipelineSpecification& OpenGLPipeline::GetSpecification() const
  {
    return m_specification;
  };
} // namespace IKan
