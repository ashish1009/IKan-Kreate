//
//  SceneRenderer.hpp
//  IKan
//
//  Created by Ashish . on 17/07/24.
//

#pragma once

namespace IKan
{
  /// This class stores the API to submit the shapes to render them each frame
  /// - Note: This class renderes each shape inside a framebuffer
  class SceneRenderer
  {
  public:
    /// This Constructore creaates the scene renderer instance
    /// - Parameter debugName: debug name of renderer
    SceneRenderer(std::string_view debugName);
    /// This Destrcutro destroys the scene renderer instance
    ~SceneRenderer();
    
  private:
    std::string m_debugName {"IKan Renderer"};    
  };
} // namespace IKan
