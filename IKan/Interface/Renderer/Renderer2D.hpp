//
//  Renderer2D.hpp
//  IKan
//
//  Created by Ashish . on 10/07/24.
//

#pragma once

namespace IKan
{
  // Forward declaration
  class Texture;

  /// This class stores the APIs to initialize the 2D batch renderer and draw the 2D shapes
  class Renderer2D
  {
  public:
    /// This function initializes the 2D batch renderer
    static void Initialize();
    /// This function shuts down the 2D batch renderer
    static void Shutdown();
    
    /// This funcition Adds the quad batch limit to prev allocated data
    /// - Parameter quads: quads to be renderered in single batch
    static void AddQuads(uint32_t quads);
    /// This funcition Adds the circle batch limit to prev allocated data
    /// - Parameter circles: circles to be renderered in single batch
    static void AddCircles(uint32_t circles);
    /// This funcition Adds the line batch limit to prev allocated data
    /// - Parameter lines: lines to be renderered in single batch
    static void AddLines(uint32_t lines);
    
    /// This function starts the batch rendering for 2D Shapes per frame
    /// - Parameters:
    ///   - cameraViewProjectionMatrix: camera view projection matrix
    ///   - cameraViewMatrix: camera view matrix
    static void BeginBatch(const glm::mat4& cameraViewProjectionMatrix, const glm::mat4& cameraViewMatrix);
    /// This function end the batch rendering for 2D Shapes per frame
    static void EndBatch();
    
    /// This function submits the quad to draw in batch renderer
    /// - Parameters:
    ///   - transform: Transformation matrix of Quad
    ///   - color: Color of Quad
    ///   - texture: Texture to be uploaded in Batch
    ///   - tilingFactor: tiling factor of quad
    ///   - objectID: Pixel/Entity ID of Quad
    static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color = Utils::Math::UnitVec4,
                           const Ref<Texture>& texture = nullptr, float tilingFactor = 1.0f, int32_t objectID = -1);
    /// This function submits the quad to draw in batch renderer
    /// - Parameters:
    ///   - position: position of the quad
    ///   - scale: scale of the quad
    ///   - rotation: rotation of the quad (in degrees)
    ///   - color: Color of Quad
    ///   - texture: Texture to be uploaded in Batch
    ///   - tilingFactor: tiling factor of quad
    ///   - objectID: Pixel/Entity ID of Quad
    /// - Note: Low Performance API as it computes the transform matrix from position, rotation and scale
    static void SubmitQuad(const glm::vec3& position, const glm::vec2& scale = Utils::Math::UnitVec2,
                           const glm::vec3& rotation = Utils::Math::ZeroVec3, const glm::vec4& color = Utils::Math::UnitVec4,
                           const Ref<Texture>& texture = nullptr, float tilingFactor = 1.0f, int32_t objectID = -1);
    
  private:
    /// This function is the helper function to rendere the quad
    /// - Parameters:
    ///   - transform: Transformation matrix of Quad
    ///   - color: Color of Quad
    ///   - texture: Texture to be uploaded in Batch
    ///   - tilingFactor: tiling factor of quad
    ///   - textureCoords: Texture coordinates of quad
    ///   - objectID: Pixel/Entity ID of Quad
    static void SubmitQuadImpl(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture>& texture,
                               float tilingFactor, const glm::vec2* textureCoords, int32_t objectID);
  };
} // namespace IKan
