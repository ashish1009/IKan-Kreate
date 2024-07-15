//
//  Renderer2D.hpp
//  IKan
//
//  Created by Ashish . on 10/07/24.
//

#pragma once

#include "Renderer/UI/Font.hpp"

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
    
    /// This function submits the circle to draw in batch renderer
    /// - Parameters:
    ///   - transform: Transformation matrix of Circle
    ///   - color: Color of circle
    ///   - texture: Texture to be uploaded in Batch
    ///   - tilingFactor: tiling factor of Texture (Scale by which texture to be Multiplied)
    ///   - thickness: thickneess of Circle
    ///   - fade: fade of Circle Edge
    ///   - objectID: Pixel ID of Circle
    static void SubmitCircle(const glm::mat4& transform, const glm::vec4& color = Utils::Math::UnitVec4,
                             const Ref<Texture>& texture = nullptr, float tilingFactor = 1.0f,
                             float thickness = 1.0f, float fade = (float)0.005, int32_t objectID = -1);

    /// This function submits the circle to draw in batch renderer
    /// - Parameters:
    ///   - position: Center Circle
    ///   - radius: radius same for all axir
    ///   - rotation: rotation of circle
    ///   - color: Color of circle
    ///   - texture: Texture to be uploaded in Batch
    ///   - tilingFactor: tiling factor of Texture (Scale by which texture to be Multiplied)
    ///   - thickness: thickneess of Circle
    ///   - fade: fade of Circle Edge
    ///   - objectID: Pixel ID of Circle
    static void SubmitCircle(const glm::vec3& position, float radius = 1.0f, const glm::vec3& rotation = Utils::Math::ZeroVec3,
                             const glm::vec4& color = Utils::Math::UnitVec4, const Ref<Texture>& texture = nullptr,
                             float tilingFactor = 1.0f, float thickness = 1.0f, float fade = (float)0.005f, int32_t objectID = -1);
      
    /// This function submits the circle to draw in batch renderer
    /// - Parameters:
    ///   - position: Center Circle
    ///   - radius: radius
    ///   - rotation: rotation
    ///   - color: Color of circle
    ///   - texture: Texture to be uploaded in Batch
    ///   - tilingFactor: tiling factor of Texture (Scale by which texture to be Multiplied)
    ///   - thickness: thickneess of Circle
    ///   - fade: fade of Circle Edge
    ///   - objectID: Pixel ID of Circle
    static void SubmitCircle(const glm::vec3& position, const glm::vec2& radius = Utils::Math::UnitVec2,
                             const glm::vec3& rotation = Utils::Math::ZeroVec3, const glm::vec4& color = Utils::Math::UnitVec4,
                             const Ref<Texture>& texture = nullptr, float tilingFactor = 1.0f,
                             float thickness = 1.0f, float fade = (float)0.005, int32_t objectID = -1);
    
    /// This function submits the line to draw in batch renderer
    /// - Parameters:
    ///   - p0: fisrt point
    ///   - p1: second point
    ///   - color: Color of line
    static void SubmitLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);
    /// This function submits the rectangle to draw in batch renderer with top corner point position
    /// - Parameters:
    ///   - p0: Top left Point
    ///   - p1: Bottom Right Point
    ///   - color: Color of line
    static void SubmitRect(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);
    /// This function submits the rectangle to draw in batch renderer with 4 points
    /// - Parameters:
    ///   - p0: Top left Point
    ///   - p1: Bottom Right Point
    ///   - color: Color of line
    static void SubmitRect(const glm::vec3& p0, const glm::vec3& p1,
                         const glm::vec3& p2, const glm::vec3& p3, const glm::vec4& color);
    
    /// This function submits the rectangle to draw in batch renderer with position and size
    /// - Parameters:
    ///   - position: position of rectangle
    ///   - size: size of rectangle
    ///   - color: Color of line
    static void SubmitRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = Utils::Math::UnitVec4);
    /// This function submits the rectangle to draw in batch renderer with transform
    /// - Parameters:
    ///   - transform: transform
    ///   - color: Color of line
    static void SubmitRect(const glm::mat4& transform, const glm::vec4& color);
    
    /// This functions renders the Text in Window using postition and sclae
    /// - Parameters:
    ///   - text: Text to be rendereed
    ///   - font: Font data
    ///   - position: Text Poistion
    ///   - scale: Text Poistion
    ///   - color: Color of text
    ///   - objectID: Pixel ID
    static void SubmitFixedViewText(const std::string &text, glm::vec3 position, const glm::vec2& scale  = Utils::Math::UnitVec2,
                                    const glm::vec4& color = Utils::Math::UnitVec4, Ref<Font> font = Font::GetDefaultFont(),
                                    int32_t objectID = -1);
    /// This functions renders the Text in Window using postition and sclae
    /// - Parameters:
    ///   - text: Text to be rendereed
    ///   - font: Font data
    ///   - position: Text Poistion
    ///   - scale: Text Poistion
    ///   - color: Color of text
    ///   - objectID: Pixel ID&
    static void SubmitText(const std::string &text, glm::vec3 position, const glm::vec2& scale = Utils::Math::UnitVec2,
                           const glm::vec4& color = Utils::Math::UnitVec4, Ref<Font> font = Font::GetDefaultFont(),
                           int32_t objectID  = -1);

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
    
    /// This function is the helper function to rendere the circle
    /// - Parameters:
    ///   - transform: transform matrix of circle
    ///   - texture: texture to be binded in circle
    ///   - tilingFactor: tiling factor of texture
    ///   - color: color of circle
    ///   - thickness: thickness of circle
    ///   - fade: cirlce face
    ///   - objectID: object/pixel id
    static void SubmitCircleImpl(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor,
                                 const glm::vec4& color, float thickness, float fade, int32_t objectID);

    /// This functions renders the Text in Window using postition and sclae
    /// - Parameters:
    ///   - text: Text to be rendereed
    ///   - font: Font data
    ///   - position: Text Poistion
    ///   - scale: Text Poistion
    ///   - color: Color of text
    ///   - objectID: Pixel ID
    static void SubmitTextImpl(const std::string& text, Ref<Font> font, glm::vec3 position, const glm::vec2& scale,
                               const glm::vec4& color, int32_t objectID);

  };
} // namespace IKan
