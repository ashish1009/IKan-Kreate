//
//  Renderer2D.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace IKan
{
  class Texture;
  class Sprite;
  class RenderPass;
  class Font;
  
  struct Renderer2DData
  {
    uint32_t maxQuads = 0;
    uint32_t maxCircles = 0;
    uint32_t maxLines = 0;
    
    Renderer2DData(uint32_t maxQuads = 0, uint32_t maxCircles = 0, uint32_t maxLines = 0)
    : maxQuads(maxQuads), maxCircles(maxCircles), maxLines(maxLines)
    {
      
    }
  };

  /// This is the batch renderer for 2D Renderering
  class Renderer2D
  {
  public:
    /// This function initialzes the Batch renderer. Create All buffers needed to store Data (Both Renderer and CPU)
    /// - Parameters:
    ///   - maxQuads: Max Quad to render in one batch
    ///   - maxCirlces: Max Circles to render in one batch
    ///   - maxLines: Max Lines to render in one batch
    static void Initialize(const Renderer2DData& Renderer2DData);
    /// This functio dhutdown the batch renderer and destoy all the buffer reserved on initialize
    static void Shutdown();

    /// This funcition Adds the quads renderer data to prev allocaged data
    /// - Parameter maxQuads: max quad to be renderered in single batch
    static void AddQuadData(uint32_t maxQuads);
    /// This funcition Adds the circle renderer data to prev allocaged data
    /// - Parameter maxCirlces: max circle to be renderered in single batch
    static void AddCircleData(uint32_t maxCirlces);
    /// This funcition Adds the lines renderer data to prev allocaged data
    /// - Parameter maxLines: max circle to be renderered in single batch
    static void AddLineData(uint32_t maxLines);
    
    /// This function begins the Batch for 2D Rendere (to be called each frame)
    /// - Parameters:
    ///   - camViewProjMat: Camera View projection Matrix
    ///   - cameraViewMat: Camera View Matrix
    static void BeginBatch(const glm::mat4& camViewProjMat, const glm::mat4 &cameraViewMat);
    /// This function Ends the current batch by rendering all the vertex
    static void EndBatch();

    /// This function render the fullscreen quad
    /// - Parameters:
    ///   - texture: Texture to be loaded
    ///   - slot: Slot to be load the texture
    ///   - overrideShader: Override shader flag. if false then deafult shader is used
    /// - Note: If true then bind your own shader
    static void DrawFullscreenQuad(const Ref<Texture>& texture = nullptr, uint32_t slot = 0, bool overrideShader = false);
    
    /// This function draws Quad with color
    /// - Parameters:
    ///   - transform: Transformation matrix of Quad
    ///   - color: Color of Quad
    ///   - objectID: Pixel ID of Quad
    static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int32_t objectID = -1 );
    /// This function draws Quad with color
    /// - Parameters:
    ///   - position: position of the quad
    ///   - scale: scale of the quad
    ///   - rotation: rotation of the quad
    ///   - color: Color of Quad
    ///   - objectID: Pixel ID of Quad
    /// - Note: High Cycle API
    static void DrawQuad(const glm::vec3& position, const glm::vec3& scale = glm::vec3(1.0f),
                         const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec4& color = glm::vec4(1.0f),
                         int32_t objectID = -1 );
    /// This function draws Quad with color
    /// - Parameters:
    ///   - position: position of the quad
    ///   - scale: scale of the quad
    ///   - rotation: rotation of the quad
    ///   - texture: Texture to be uploaded in Batch
    ///   - color: Color of Quad
    ///   - objectID: Pixel ID of Quad
    /// - Note: High Cycle API
    static void DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation,
                         const Ref<Texture>& texture, const glm::vec4& color = glm::vec4(1.0f), int32_t objectID = -1 );
    
    /// This function draws Quad with texture
    /// - Parameters:
    ///   - transform: Transformation matrix of Quad
    ///   - texture: Texture to be uploaded in Batch
    ///   - tintColor: Color of Quad
    ///   - tilingFactor: tiling factor of Texture (Scale by which texture to be Multiplied)
    ///   - objectID: Pixel ID of Quad
    static void DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f),
                         float tilingFactor = 1.0f, int32_t objectID = -1 );
    
    /// This function draws Quad with Subtexture sprite
    /// - Parameters:
    ///   - transform: Transformation matrix of Quad
    ///   - sub_texture: Subtexture component
    ///   - object_id: entity ID of Quad
    static void DrawQuad(const glm::mat4& transform, const Ref<Sprite>& subTexture, const glm::vec4& tintColor = glm::vec4(1.0f),
                         int32_t objectID = -1);
    
    /// This functon draws Quad API with Texture with fixed camera with transform matrix
    /// - Parameters:
    ///   - transform: Transformation matrix of Quad
    ///   - texture: Texture to be uploaded in Batch
    ///   - tintColor: Color of Quad
    ///   - tilingFactor: tiling factor of Texture (Scale by which texture to be Multiplied)
    ///   - entityId: Pixel ID of Quad
    static void DrawFixedViewQuad(const glm::mat4& transform,
                                  const Ref<Texture>& texture,
                                  const glm::vec4& tintColor = glm::vec4(1.0f),
                                  float tilingFactor = 1.0f,
                                  int32_t entityId = -1);
    /// This functon draws Quad API with Texture with fixed camera with transform position and scale
    /// - Parameters:
    ///   - position: Position matrix of Quad
    ///   - position: Size matrix of Quad
    ///   - texture: Texture to be uploaded in Batch
    ///   - tintColor: Color of Quad
    ///   - tilingFactor: tiling factor of Texture (Scale by which texture to be Multiplied)
    ///   - entityDd: Pixel ID of Quad
    static void DrawFixedViewQuad(const glm::vec3& position,
                                  const glm::vec3& scale,
                                  const Ref<Texture>& texture,
                                  const glm::vec4& tintColor = glm::vec4(1.0f),
                                  float tilingFactor = 1.0f,
                                  int32_t entityId = -1);

    /// This function draws circle with color
    /// - Parameters:
    ///   - position: Center Circle
    ///   - radius: radius
    ///   - rotation: rotation
    ///   - tintColor: Color of Circle
    ///   - thickness: thickneess of Circle
    ///   - fade: fade of Circle Edge
    ///   - objectID: Pixel ID of Circle
    static void DrawCircle(const glm::vec3& position, const glm::vec3& radius = glm::vec3(1.0f),
                           const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec4& tintColor = glm::vec4(1.0f),
                           float thickness = 1.0f, float fade = (float)0.005, int32_t objectID = -1);
    
    /// This function draws circle with color
    /// - Parameters:
    ///   - transform: Transformation matrix of Circle
    ///   - color: Color of Circle
    ///   - thickness: thickneess of Circle
    ///   - fade: fade of Circle Edge
    ///   - objectID: Pixel ID of Circle
    static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = (float)0.005,
                           int32_t objectID = -1);
    /// This function draws circle with texture
    /// - Parameters:
    ///   - transform: Transformation matrix of Circle
    ///   - texture: Texture to be uploaded in Batch
    ///   - tilingFactor: tiling factor of Texture (Scale by which texture to be Multiplied)
    ///   - tintColor: Color of Quad
    ///   - thickness: thickneess of Circle
    ///   - fade: fade of Circle Edge
    ///   - objectID: Pixel ID of Circle
    static void DrawCircle(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f),
                           float tilingFactor = 1.0f, float thickness = 1.0f, float fade = (float)0.005, int32_t objectID = -1 );
    
    /// This function draws a line using two points in 3d Space
    /// - Parameters:
    ///   - p0: fisrt point
    ///   - p1: second point
    ///   - color: Color of line
    static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);
    /// This function draws a Recttangle using position and size of rectangle
    /// - Parameters:
    ///   - p0: Top left Point
    ///   - p1: Bottom Right Point
    ///   - color: Color of line
    static void DrawRect(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);
    /// This function draws a Recttangle using position and size of rectangle
    /// - Parameters:
    ///   - p0: Top left Point
    ///   - p1: Bottom Right Point
    ///   - color: Color of line
    static void DrawRect(const glm::vec3& p0, const glm::vec3& p1,
                         const glm::vec3& p2, const glm::vec3& p3, const glm::vec4& color);

    /// This function draws a Recttangle using position and size of rectangle
    /// - Parameters:
    ///   - position: position of rectangle
    ///   - size: size of rectang;e
    ///   - color: Color of line
    static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    /// This function draws a Recttangle using position and size of rectangle
    /// - Parameters:
    ///   - transform: transform
    ///   - color: Color of line
    static void DrawRect(const glm::mat4& transform, const glm::vec4& color);
    
    /// This functions renders the Text in Window using postition and sclae
    /// - Parameters:
    ///   - text: Text to be rendereed
    ///   - font: Font data
    ///   - position: Text Poistion
    ///   - scale: Text Poistion
    ///   - color: Color of text
    ///   - objectID: Pixel ID of Circle
    static void RenderFixedViewText(const std::string& text, Ref<Font> font, glm::vec3 position, const glm::vec2& scale,
                                    const glm::vec4& color, int32_t objectID);
    /// This functions renders the Text in Window using postition and sclae
    /// - Parameters:
    ///   - text: Text to be rendereed
    ///   - font: Font data
    ///   - position: Text Poistion
    ///   - scale: Text Poistion
    ///   - color: Color of text
    ///   - objectID: Pixel ID of Circle
    static void RenderText(const std::string& text, Ref<Font> font, glm::vec3 position, const glm::vec2& scale,
                           const glm::vec4& color, int32_t objectID);

    MAKE_PURE_STATIC(Renderer2D);
    
  private:
    /// This function flsh a single batch
    static void Flush();
    
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
