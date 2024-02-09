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
#include "Renderer/2D/LineData.h"

namespace IKan
{
#define BATCH_INFO(...) IK_LOG_INFO(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_TRACE(...) IK_LOG_TRACE(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_WARN(...) IK_LOG_WARN(LogModule::Renderer2D, __VA_ARGS__)
  
  struct Renderer2DData
  {
    QuadBatchData quadData;
    CircleBatchData circleData;
    LineBatchData lineData;
    FullScreenQuad fullScreenQuad;
    
    glm::mat4 cameraViewProjectionMatrix;
    glm::mat4 cameraViewMatrix;
    
    void Destroy()
    {
      quadData.Destroy();
      circleData.Destroy();
      lineData.Destroy();
      fullScreenQuad.Destroy();
    }
  };
  static Renderer2DData s_data;

  void Renderer2D::Initialize(const Renderer2DBatchLimit& data)
  {
    IK_PROFILE();
    BATCH_INFO("Initialising the Batch Renderer 2D ");
    
    AddQuadData(data.maxQuads);
    AddCircleData(data.maxCircles);
    AddLineData(data.maxLines);
    s_data.fullScreenQuad.Initialize();
  }
  
  void Renderer2D::Shutdown()
  {
    IK_PROFILE();
    BATCH_INFO("Shutting Down the Batch Renderer 2D ");
    
    s_data.Destroy();
  }

  void Renderer2D::AddQuadData(uint32_t maxQuads)
  {
    IK_PROFILE();
    RETURN_IF(maxQuads == 0);
    s_data.quadData.Initialize(maxQuads);
    RendererStatistics::Get()._2d.maxQuads = s_data.quadData.maxElement;
  }

  void Renderer2D::AddCircleData(uint32_t maxCircles)
  {
    IK_PROFILE();
    RETURN_IF(maxCircles == 0);
    s_data.circleData.Initialize(maxCircles);
    RendererStatistics::Get()._2d.maxCircles = s_data.circleData.maxElement;
  }

  void Renderer2D::AddLineData(uint32_t maxLines)
  {
    IK_PROFILE();
    RETURN_IF(maxLines == 0);
    s_data.lineData.Initialize(maxLines);
    RendererStatistics::Get()._2d.maxLines = s_data.lineData.maxElement;
  }
  
  void Renderer2D::BeginBatch(const glm::mat4 &camViewProjMat, const glm::mat4 &cameraViewMat)
  {
    IK_PERFORMANCE("Renderer2D::BeginBatch");
    s_data.cameraViewProjectionMatrix = camViewProjMat;
    s_data.cameraViewMatrix = cameraViewMat;
    
    s_data.quadData.StartBatch(camViewProjMat);
    s_data.circleData.StartBatch(camViewProjMat);
    s_data.lineData.StartBatch(camViewProjMat);
  }
  
  void Renderer2D::EndBatch()
  {
    IK_PERFORMANCE("Renderer2D::BeginBatch");
    s_data.quadData.Flush();
    s_data.circleData.Flush();
    s_data.lineData.Flush();
  }

} // namespace IKan
