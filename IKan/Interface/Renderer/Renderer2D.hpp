//
//  Renderer2D.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

namespace IKan
{
  class Texture;
  
  struct Renderer2DBatchLimit
  {
    uint32_t maxQuads {0};
    uint32_t maxCircles {0};
    uint32_t maxLines {0};
  };
  
  /// This is the batch renderer for 2D Renderering
  class Renderer2D
  {
  public:
    /// This function initialzes the 2D renderer.
    /// - Parameter data: Batch Rendere Limit data
    static void Initialize(const Renderer2DBatchLimit& data);
    /// This function dhutdown the 2D renderer.
    static void Shutdown();
    
    /// This funcition Adds the quads renderer data to prev allocaged data
    /// - Parameter maxQuads: max quad to be renderered in single batch
    static void AddQuadData(uint32_t maxQuads);
    /// This funcition Adds the circle renderer data to prev allocaged data
    /// - Parameter maxCirlces: max circle to be renderered in single batch
    static void AddCircleData(uint32_t maxCirlces);
    /// This funcition Adds the maxLines renderer data to prev allocaged data
    /// - Parameter maxLines: max maxLines to be renderered in single batch
    static void AddLineData(uint32_t maxLines);
    
    /// This function begins the Batch for 2D Rendere (to be called each frame)
    /// - Parameters:
    ///   - camViewProjMat: Camera View projection Matrix
    ///   - cameraViewMat: Camera View Matrix
    static void BeginBatch(const glm::mat4& camViewProjMat, const glm::mat4& cameraViewMat);
    /// This function Ends the current batch by rendering all the vertex
    static void EndBatch();

    /// This function is the helper function to rendere the quad
    /// - Parameters:
    ///   - transform: transform matrix of quad
    ///   - texture: texture to be binded in quad
    ///   - textureCoords: texture coordinates
    ///   - tilingFactor: tiling factor of texture
    ///   - tintColor: color of quad
    ///   - objectID: object/pixel id
    static void DrawTextureQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec2* textureCoords,
                                float tilingFactor, const glm::vec4& tintColor, int32_t objectID);
    
    /// This function is the helper function to rendere the circle
    /// - Parameters:
    ///   - transform: transform matrix of circle
    ///   - texture: texture to be binded in circle
    ///   - tilingFactor: tiling factor of texture
    ///   - tintColor: color of circle
    ///   - thickness: thickness of circle
    ///   - fade: cirlce face
    ///   - objectID: object/pixel id
    static void DrawTextureCircle(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor,
                                  const glm::vec4& tintColor, float thickness, float fade, int32_t objectID);

  };
} // namespace IKan
