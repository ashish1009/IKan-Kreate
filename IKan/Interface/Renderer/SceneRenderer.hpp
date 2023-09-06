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
    glm::mat4 camViewProjection;
    glm::mat4 camView;
  };
  
  class SceneRenderer
  {
  public:
    /// This function create scene renderer instance
    /// - Parameter scene: scene context
    SceneRenderer(Ref<Scene> scene);
    /// This destructor destroys the scene renderer instance
    virtual ~SceneRenderer();
    
    /// This function updates the viewport size for 2D Renderer
    /// - Parameters:
    ///   - width: new width of viewport
    ///   - height: new height of viewport
    void SetViewport(uint32_t width, uint32_t height);
    
    /// This function begins the Batch for 2D Rendere (to be called each frame)
    /// - Parameters:
    ///   - camViewProjMat: Camera View projection Matrix
    ///   - camViewMat: Camera Ciew matrix
    void BeginScene(const glm::mat4& camViewProjMat, const glm::mat4& camViewMat);
    /// This function Ends the current batch by rendering all the vertex
    void EndScene();
    
    /// This function submits the mesh source in scene
    /// - Parameters:
    ///   - mesh: Mesh source
    ///   - transform: mesh transform
    void SubmitMeshSource(Ref<MeshSource> mesh, const glm::mat4& transform);
    
    /// This function begins the Current Render Pass
    void BeginRenderPass();
    /// This function ends the Current Render Pass
    void EndRenderPass();

    /// This function returns the Render pass for 2D Renderer
    Ref<RenderPass> GetRenderPass();
    /// This fucntion returns the final Renderer Image of Taget framebuffer of Current Render Pass
    Ref<Texture> GetFinalImage();
    
    /// This function returns the the Pixel ID from Viewport
    /// - Parameters:
    ///   - mx: x pixel
    ///   - my: y pixel
    ///   - pixeldData: piixel value
    void GetEntityIdFromPixels(int32_t mx, int32_t my, int32_t& pixeldData);
    
  private:
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This function flushes all draw Lists
    void FlushDrawList();
    /// This function pass the 3D Geometry Renderer
    void GeometryPass();
    /// This function clears the draw lists
    void ClearDrawLists();
    
    // Member Variables ---------------------------------------------------------------------------------------------
    Ref<Scene> m_scene;
    
    struct MeshSourceDrawCommand
    {
      Ref<MeshSource> staticMesh;
      glm::mat4 transform;
    };

    std::vector<MeshSourceDrawCommand> m_meshSourceDrawList;
    Scope<SceneRendererData> m_commonData;
  };
} // namespace IKan
