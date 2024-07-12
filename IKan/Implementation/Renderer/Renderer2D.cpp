//
//  Renderer2D.cpp
//  IKan
//
//  Created by Ashish . on 10/07/24.
//

#include "Renderer2D.hpp"

#include "Renderer/QuadData.hpp"

namespace IKan
{
  /// This stucture stores the Renderer 2D Data
  struct Renderer2DData
  {
    QuadBatchData quadData;
    
    void Destroy()
    {
      IK_PROFILE();
      
      quadData.Destroy();
    }
  };
  static Renderer2DData s_data;

  void Renderer2D::Initialize()
  {
    IK_PROFILE();
    BATCH_INFO("Initialising the Batch Renderer 2D ");
    
    AddQuads(1000);
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
} // namespace IKan
