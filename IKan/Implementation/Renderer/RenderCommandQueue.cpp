//
//  RenderCommandQueue.cpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#include "RenderCommandQueue.hpp"

namespace IKan
{
  RenderCommandQueue::RenderCommandQueue(uint8_t commandBufferSizeMB)
  {
    IK_PROFILE();
    
    uint64_t commandBufferSize = commandBufferSizeMB * 1024 * 1024;
    m_commandBuffer = new uint8_t[commandBufferSize];
    m_commandBufferPtr = m_commandBuffer;
    memset(m_commandBuffer, 0, commandBufferSize);
    
    IK_LOG_INFO(LogModule::Renderer, "Creating Renderer command queue. (Size {0} MB)", commandBufferSizeMB);
  }
  
  RenderCommandQueue::~RenderCommandQueue()
  {
    IK_PROFILE();
    delete[] m_commandBuffer;
    IK_LOG_WARN(LogModule::Renderer, "Destroying Renderer command queue");
  }
} // namespace IKan
