//
//  SceneRenderer.hpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#pragma once

namespace IKan
{
  /// This class is responsible for Rendering Scene in Viewport
  class SceneRenderer
  {
  public:
    /// This is the default constructor of Scene Renderer
    /// - Parameter debugName: debug name of renderer
    SceneRenderer(const std::string& debugName);
    /// This is the default destructor of Scene Renderer
    ~SceneRenderer();
    
  private:
    // Member Variables ---------------------------------------------------------------------------------------------
    std::string m_debugName;
  };
} // namespace IKan
