//
//  TextRenderer.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace IKan
{
  class Font;
  
  ///  This is the Wrapper class to initialise use the text renderer as batch of 16 characters in single render
  class TextRenderer
  {
  public:
    /// This function initializes the Text renderer. Create All buffers needed to store Data (Both Renderer and CPU)
    /// - Note: Load the Font before using any other API In client 'LoadFreetype'
    static void Initialise();
    /// This function Shuts down on Text the Batch Renderer. Delete all the allocated Data
    static void Shutdown();
    
    MAKE_PURE_STATIC(TextRenderer);
  };
}
