//
//  Renderer2D.cpp
//  IKan
//
//  Created by Ashish . on 10/07/24.
//

#include "Renderer2D.hpp"

#include "Renderer/QuadData.hpp"
#include "Renderer/CircleData.hpp"
#include "Renderer/LineData.hpp"

namespace IKan
{
  /// This stucture stores the Renderer 2D Data
  struct Renderer2DData
  {
    glm::mat4 cameraViewProjectionMatrix;
    glm::mat4 cameraViewMatrix;

    QuadBatchData quadData;
    CircleBatchData circleData;
    LineBatchData lineData;

    void Destroy()
    {
      IK_PROFILE();
      
      quadData.Destroy();
      circleData.Destroy();
      lineData.Destroy();
    }
  };
  static Renderer2DData s_data;
  
  void Renderer2D::Initialize()
  {
    IK_PROFILE();
    BATCH_INFO("Initialising the Batch Renderer 2D ");
    
    AddQuads(1000);
    AddCircles(1000);
    AddLines(10000);
  }
  void Renderer2D::Shutdown()
  {
    IK_PROFILE();
    BATCH_WARN("Shutting Down the Batch Renderer 2D ");
    
    s_data.Destroy();
  }
  
  void Renderer2D::AddQuads(uint32_t quads)
  {
    IK_PROFILE();
    if (quads == 0)
    {
      return;
    }
    
    s_data.quadData.Initialize(quads);
  }
  
  void Renderer2D::AddCircles(uint32_t circles)
  {
    IK_PROFILE();
    if (circles == 0)
    {
      return;
    }
    
    s_data.circleData.Initialize(circles);
  }
  
  void Renderer2D::AddLines(uint32_t lines)
  {
    IK_PROFILE();
    if (lines == 0)
    {
      return;
    }
    
    s_data.lineData.Initialize(lines);
  }
  
  void Renderer2D::BeginBatch(const glm::mat4 &cameraViewProjectionMatrix, const glm::mat4 &cameraViewMatrix)
  {
    IK_PERFORMANCE("Renderer2D::BeginBatch");
    s_data.cameraViewProjectionMatrix = cameraViewProjectionMatrix;
    s_data.cameraViewMatrix = cameraViewMatrix;
    
    s_data.quadData.StartBatch(s_data.cameraViewProjectionMatrix);
    s_data.circleData.StartBatch(s_data.cameraViewProjectionMatrix);
    s_data.lineData.StartBatch(s_data.cameraViewProjectionMatrix);
  }
  
  void Renderer2D::EndBatch()
  {
    IK_PERFORMANCE("Renderer2D::BeginBatch");
    s_data.quadData.Flush();
    s_data.circleData.Flush();
    s_data.lineData.Flush();
  }
} // namespace IKan
