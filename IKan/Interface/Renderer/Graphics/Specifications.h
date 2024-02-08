//
//  Specifications.h
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Graphics/BufferLayout.hpp"

namespace IKan
{
  struct PipelineSpecification
  {
    std::string debugName;
    BufferLayout vertexLayout;
  };
} // namespace IKan
