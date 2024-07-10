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
    
    m_commandBufferSize = commandBufferSizeMB * 1024 * 1024;
    m_commandBuffer = new uint8_t[m_commandBufferSize];
    m_commandBufferPtr = m_commandBuffer;
    memset(m_commandBuffer, 0, m_commandBufferSize);
    
    IK_LOG_INFO(LogModule::Renderer, "Creating Renderer command queue. (Size {0} MB)", commandBufferSizeMB);
  }
  
  RenderCommandQueue::~RenderCommandQueue()
  {
    IK_PROFILE();
    delete[] m_commandBuffer;
    IK_LOG_WARN(LogModule::Renderer, "Destroying Renderer command queue");
  }
  
  void* RenderCommandQueue::Allocate(RenderCommandFn fn, uint32_t size)
  {
    IK_PERFORMANCE("RenderCommandQueue::Allocate");
    IK_ASSERT(m_commandBuffer, "Command Buffer is not allocated");
    
    // Check memory overflow
    {
      [[maybe_unused]] uint64_t memoryUsed = static_cast<uint64_t>(m_commandBufferPtr - m_commandBuffer);
      [[maybe_unused]] uint64_t memoryFree = m_commandBufferSize - memoryUsed;;
      
      // TODO: IKan: Resize?
      IK_ASSERT(memoryFree > 0, "Memory Overflow for Commands");
    }
    
    // 1. Stores the Render Command fucntion pointer in the queue. 'This is lambda that calls the Rendere Command
    //    submited in Renderer::Submit()'
    //    |--------------------------------------------------------------------------------------| // 10 MB memory
    //    ^
    // m_commandBufferPtr
    //    |< RenderCommandFn >|
    *(RenderCommandFn*)m_commandBufferPtr = fn;
    
    // 2. Update the pointer after size of RenderCommandFn pointer
    //    |--------------------------------------------------------------------------------------| // 10 MB memory
    //                        ^
    //                m_commandBufferPtr // Shifter the pointer after RenderCommandFn pointer
    //    |< RenderCommandFn >|
    m_commandBufferPtr += sizeof(RenderCommandFn);
    
    // 3. Store the size of Render command function in queue (Submitted in Renderer::Submit())
    //    |--------------------------------------------------------------------------------------| // 10 MB memory
    //                        ^
    //                m_commandBufferPtr
    //    |< RenderCommandFn >|<  size  >|
    *(uint32_t*)m_commandBufferPtr = size;
    
    // 4. Update the pointer after size of uint32_t which stores the size of Render Command function
    //    |--------------------------------------------------------------------------------------| // 10 MB memory
    //                                   ^
    //                            m_commandBufferPtr // Shifter the pointer after size of uint32 which stores the size
    //    |< RenderCommandFn >|<  size  >|
    m_commandBufferPtr += sizeof(uint32_t);
    
    // 5. Stores the current memory address where Renderer::Submit() will store the actual render command function
    //    |--------------------------------------------------------------------------------------| // 10 MB memory
    //                                   ^
    //                            m_commandBufferPtr
    //                                memory
    //    |< RenderCommandFn >|<  size  >|
    void* memory = m_commandBufferPtr;
    
    // 6. Update the pointer just after the function
    //    |--------------------------------------------------------------------------------------| // 10 MB memory
    //                                   ^                                              ^
    //                                memory                                      m_commandBufferPtr
    //    |< RenderCommandFn >|<  size  >|< Function to be written by Renderer::Submit >|
    m_commandBufferPtr += size;
    
    // 7. Increment the Command count
    m_commandCount++;
    
    // 8. Return the memory address where actual function to be written by Renderer::Submit()
    return memory;
  }
  
  void RenderCommandQueue::Execute()
  {
    IK_PERFORMANCE("RenderCommandQueue::Execute");
    // 1. Store the start address of command queue
    uint8_t* buffer = m_commandBuffer;
    for (uint32_t commandIdx = 0; commandIdx < m_commandCount; commandIdx++)
    {
      // 2. Get the RenderCommandFn pointer
      RenderCommandFn function = *(RenderCommandFn*)buffer;
      
      // 3. Shift the pointer after RenderCommandFn to get size of function
      buffer += sizeof(RenderCommandFn);
      
      // 4. Get the size of function
      uint32_t size = *(uint32_t*)buffer;
      
      // 5. Shift the pointer after size of uint32_t to get actual render function
      buffer += sizeof(uint32_t);
      
      // 6. execute the function
      function(buffer);
      
      // 7. Shift the pointer after Render function
      buffer += size;
    }
    
    // Reset the command queue
    m_commandBufferPtr = m_commandBuffer;
    m_commandCount = 0;
  }
} // namespace IKan
