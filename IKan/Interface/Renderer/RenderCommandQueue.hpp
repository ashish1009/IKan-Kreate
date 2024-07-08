//
//  RenderCommandQueue.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

namespace IKan 
{
  /// This class stores the renderer commands in a queue to execute them in one go. By default stores 10MB memory
  class RenderCommandQueue
  {
  public:
    typedef void(*RenderCommandFn)(void*);

    /// This constructor initializes the queue buffer for renderer commands
    /// - Parameter commandBufferSizeMB: command buffer size in MB: Default 10MB
    RenderCommandQueue(uint8_t commandBufferSizeMB = 10);
    /// This destructor destroys the queue buffer of renderer commands
    ~RenderCommandQueue();
    
  private:
    uint8_t* m_commandBuffer {nullptr};
    uint8_t* m_commandBufferPtr {nullptr};
    uint32_t m_commandCount {0};
  };
} // namespace IKan
