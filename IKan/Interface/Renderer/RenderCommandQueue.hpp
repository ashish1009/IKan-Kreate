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
    
    /// This function allocates the memory for fucntion pointer in queue
    /// - Parameters:
    ///   - func: Render Command function pointer
    ///   - size: size of function
    /// - Returns: Allocated memory pointer
    void* Allocate(RenderCommandFn func, uint32_t size);
    
    /// This function executes the render commands in queue
    void Execute();
    
    DELETE_COPY_MOVE_CONSTRUCTORS(RenderCommandQueue);

  private:
    uint8_t* m_commandBuffer {nullptr};
    uint8_t* m_commandBufferPtr {nullptr};
    uint64_t m_commandBufferSize {0};
    uint32_t m_commandCount {0};
  };
} // namespace IKan
