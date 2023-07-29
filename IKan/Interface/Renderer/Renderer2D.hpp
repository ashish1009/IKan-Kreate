//
//  Renderer2D.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace IKan
{
  class Image;
  class Texture;
  class Sprite;

  /// This is the batch renderer for 2D Renderering
  class Renderer2D
  {
  public:
    /// This function initialzes the Batch renderer. Create All buffers needed to store Data (Both Renderer and CPU)
    /// - Parameters:
    ///   - maxQuads: Max Quad to render in one batch
    ///   - maxCirlces: Max Circles to render in one batch
    ///   - maxLines: Max Lines to render in one batch
    static void Initialise(uint32_t maxQuads, uint32_t maxCirlces, uint32_t maxLines);
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
    
    /// This function updates the viewport size for 2D Renderer
    /// - Parameters:
    ///   - width: new width of viewport
    ///   - height: new height of viewport
    static void SetViewport(uint32_t width, uint32_t height);

    /// This function begins the Batch for 2D Rendere (to be called each frame)
    /// - Parameter camViewProjMat: Camera View projection Matrix
    static void BeginBatch(const glm::mat4& camViewProjMat);
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
    /// This function draws Quad with texture
    /// - Parameters:
    ///   - transform: Transformation matrix of Quad
    ///   - texture: Texture to be uploaded in Batch
    ///   - tintColor: Color of Quad
    ///   - tilingFactor: tiling factor of Texture (Scale by which texture to be Multiplied)
    ///   - objectID: Pixel ID of Quad
    static void DrawQuad(const glm::mat4& transform, const Ref<Image>& texture, const glm::vec4& tintColor = glm::vec4(1.0f),
                         float tilingFactor = 1.0f, int32_t objectID = -1 );
    
    /// This function draws Quad with Subtexture sprite
    /// - Parameters:
    ///   - transform: Transformation matrix of Quad
    ///   - sub_texture: Subtexture component
    ///   - object_id: entity ID of Quad
    static void DrawQuad(const glm::mat4& transform, const Ref<Sprite>& subTexture, const glm::vec4& tintColor = glm::vec4(1.0f),
                         int32_t objectID = -1);
    
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
    static void DrawTextureQuad(const glm::mat4& transform, const Ref<Image>& texture, const glm::vec2* textureCoords,
                                float tilingFactor, const glm::vec4& tintColor, int32_t objectID);

  };
} // namespace IKan
