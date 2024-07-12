//
//  Pipeline.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Factories.hpp"

namespace IKan
{
  /// This class is the interface for creating pipeline for renderer
  class Pipeline
  {
  public:
    /// Default virtual destructor
    virtual ~Pipeline() = default;
  };
} // namespace IKan
