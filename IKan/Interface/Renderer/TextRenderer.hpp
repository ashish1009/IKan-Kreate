//
//  TextRenderer.hpp
//  IKan
//
//  Created by Ashish . on 09/02/24.
//

#pragma once

namespace IKan
{
  class Font;
  
  ///  This is the Wrapper class to initialize use the text renderer as batch of 16 characters in single render
  class TextRenderer
  {
  public:
    /// This function initializes the Text renderer. Create All buffers needed to store Data (Both Renderer and CPU)
    /// - Note: Load the Font before using any other API In client 'LoadFreetype'
    static void Initialize();
    /// This function Shuts down on Text the Batch Renderer. Delete all the allocated Data
    static void Shutdown();
    
    /// This function begins the Batch for 2D Rendere (to be called each frame)
    /// - Parameter camViewProjMat: Camera View projection Matrix
    static void BeginBatch(const glm::mat4& camViewProjMat);
    /// This function Ends the current batch by rendering all the vertex
    static void EndBatch();

    DELETE_ALL_CONSTRUCTORS(TextRenderer);
    
  private:
    /// This function flsh a single batch
    static void Flush();
    /// This function moves to next batch in single frame
    static void NextBatch();

  };
} // namespace IKan
