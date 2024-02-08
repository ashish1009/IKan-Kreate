//
//  Renderer2D.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "Renderer2D.hpp"
#include "Renderer/RendererStats.hpp"
#include "Renderer/2D/BatchData.h"
#include "Renderer/2D/QuadData.h"
#include "Renderer/2D/CircleData.h"

namespace IKan
{
#define BATCH_INFO(...) IK_LOG_INFO(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_TRACE(...) IK_LOG_TRACE(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_WARN(...) IK_LOG_WARN(LogModule::Renderer2D, __VA_ARGS__)
  
  void Renderer2D::Initialize(const Renderer2DBatchLimit& data)
  {
    IK_PROFILE();
    BATCH_INFO("Initialising the Batch Renderer 2D ");
  }
  
  void Renderer2D::Shutdown()
  {
    IK_PROFILE();
    BATCH_INFO("Shutting Down the Batch Renderer 2D ");
  }
} // namespace IKan
