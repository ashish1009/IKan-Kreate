//
//  SceneRenderer.hpp
//  IKan
//
//  Created by Ashish . on 11/08/23.
//

#pragma once

#include "Camera/Camera.hpp"
#include "Scene/Scene.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/Graphics/Pipeline.hpp"
#include "Renderer/Graphics/RenderPass.hpp"

namespace IKan
{
  class Texture;
  class Renderer2DData;
  /// This structure stores the common data for Batch 2D Renderer
  struct SceneRendererData
  {
    bool needResize = false;
    uint32_t viewportWidth, viewportHeight;
    Ref<RenderPass> renderPass;
  };
  
  class SceneRenderer
  {
  public:
    /// This function create scene renderer instance
    /// - Parameters:
    ///   - scene: scene context
    ///   - renderer2DData: renderer 2D Data
    SceneRenderer(Ref<Scene> scene, const Renderer2DData& renderer2DData);
    /// This destructor destroys the scene renderer instance
    virtual ~SceneRenderer();
    
    /// This function initializes the Scene renderer
    /// - Parameter rendere2DData: rendere 2D Data 
    void Initialize(const Renderer2DData& rendere2DData);
    
    /// This function updates the viewport size for 2D Renderer
    /// - Parameters:
    ///   - width: new width of viewport
    ///   - height: new height of viewport
    void SetViewport(uint32_t width, uint32_t height);
    
    /// This function begins the Batch for 2D Rendere (to be called each frame)
    /// - Parameter camViewProjMat: Camera View projection Matrix
    void BeginBatch(const glm::mat4& camViewProjMat);
    /// This function Ends the current batch by rendering all the vertex
    void EndBatch();
    
    /// This function submits the mesh to scene
    /// - Parameter mesh: mesh source
    void SubmitMeshSource(Ref<MeshSource> mesh);

    /// This function begins the Current Render Pass
    static void BeginRenderPass();
    /// This function ends the Current Render Pass
    static void EndRenderPass();

    /// This function returns the Render pass for 2D Renderer
    static Ref<RenderPass> GetRenderPass();
    /// This fucntion returns the final Renderer Image of Taget framebuffer of Current Render Pass
    static Ref<Texture> GetFinalImage();
    
    /// This function returns the the Pixel ID from Viewport
    /// - Parameters:
    ///   - mx: x pixel
    ///   - my: y pixel
    ///   - pixeldData: piixel value
    static void GetEntityIdFromPixels(int32_t mx, int32_t my, int32_t& pixeldData);
    
  private:
    Ref<Scene> m_scene;
    Ref<Pipeline> m_geometryPipeline;
    
    struct MeshKey
    {
      MeshKey();
    };
    
    struct MeshSourceDrawCommand
    {

    };
    
    std::map<MeshKey, MeshSourceDrawCommand> m_meshSourceDrawList;
    inline static Scope<SceneRendererData> s_commonData;
  };
} // namespace IKan
