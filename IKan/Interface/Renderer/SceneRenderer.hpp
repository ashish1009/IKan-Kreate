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

  static constexpr uint32_t MAX_LIGHTS = 10;
  
  // Should be same format as PBR Shader
  struct PointLightData
  {
    float active; //  Not using bool as shader uniform dont support
    glm::vec3 position;
    glm::vec3 radiance;
  };

  struct SceneCameraData
  {
    glm::vec3 position;
    glm::mat4 viewProjection;
    float distance;
  };
  
  /// This structure stores the common data for Batch 2D Renderer
  struct SceneRendererData
  {
    bool needResize = false;
    uint32_t viewportWidth, viewportHeight;
    Ref<RenderPass> renderPass;
    Ref<Material> highlightMaterial;
    SceneCameraData sceneCamera;
    
    // Light Data
    uint32_t numLightStored = 0;
    std::array<PointLightData, 1> pointLights;
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
    
    /// This function adds the point light in scene
    /// - Parameter pointLight: point light data
    void SubmitPointLight(const PointLightData& pointLight);
    
    /// This function begins the Batch for 2D Rendere (to be called each frame)
    /// - Parameter sceneCamera: Camera data
    void BeginScene(const SceneCameraData& sceneCamera);
    /// This function Ends the current batch by rendering all the vertex
    void EndScene();
    
    /// This function submits the mesh source in scene
    /// - Parameters:
    ///   - mesh: Mesh source
    ///   - transform: mesh transform
    ///   - oevrridenMaterial: override material reference
    ///  - Note: Vertex Attribute of shader should bea s follow
    ///          layout (location = 0) in vec3 a_Position;
    ///          layout (location = 1) in vec3 a_Normal;
    ///          layout (location = 2) in vec2 a_TexCoord;
    ///          layout (location = 3) in vec3 a_Tangent;
    ///          layout (location = 4) in vec3 a_Binormal;
    void SubmitMeshSource(Ref<MeshSource> mesh, const glm::mat4& transform, const Ref<Material>& oevrridenMaterial = nullptr);
    /// This function submits the selected mesh source in scene
    /// - Parameters:
    ///   - mesh: Mesh source
    ///   - transform: mesh transform
    void SubmitSelectedMeshSource(Ref<MeshSource> mesh, const glm::mat4& transform);

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
    struct MeshSourceDrawCommand
    {
      Ref<MeshSource> staticMesh;
      glm::mat4 transform;
      Ref<Material> oevrridenMaterial = nullptr;
    };

    // Member Functions ---------------------------------------------------------------------------------------------
    /// This function flushes all draw Lists
    void FlushDrawList();
    /// This function pass the 3D Geometry Renderer
    void GeometryPass();
    /// This function pass the 3D Composit Stencil Renderer
    void CompositePass();
    /// This function clears the draw lists
    void ClearDrawLists();
    /// This function renders the mesh
    /// - Parameters:
    ///   - submeshes: mesh submeshes
    ///   - pipeline: pipeline
    ///   - material: shader
    ///   - transform: transform
    void RenderMesh(const std::vector<SubMesh>& submeshes, const Ref<Pipeline>& pipeline,
                    const Ref<Material>& material, const glm::mat4& transform);
    
    // Member Variables ---------------------------------------------------------------------------------------------
    Ref<Scene> m_scene;
    
    std::vector<MeshSourceDrawCommand> m_meshSourceDrawList;
    std::vector<MeshSourceDrawCommand> m_selectedMeshSourceDrawList;
    Scope<SceneRendererData> m_commonData;
  };
} // namespace IKan
