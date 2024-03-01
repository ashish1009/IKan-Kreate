//
//  SceneRenderer.hpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#pragma once

#include "Renderer/Graphics/FrameBuffer.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Renderer/MaterialAsset.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/Light.hpp"
#include "Camera/Camera.hpp"

namespace IKan
{
  /// Scene renderer Camera Data
  struct SceneRendererCamera
  {
    Camera camera;
    glm::vec3 position;
    glm::mat4 viewMatrix;
    float near, far; //Non-reversed
    float FOV;
  };

  /// This class is responsible for Rendering Scene in Viewport
  class SceneRenderer
  {
  public:
    /// This function initialize the Scene renderer
    static void Initialize();
    /// This function shuts down the Scene renderer
    static void Shutdown();
    
    /// This is the default constructor of Scene Renderer
    /// - Parameter debugName: debug name of renderer
    SceneRenderer(const std::string& debugName);
    /// This is the default destructor of Scene Renderer
    ~SceneRenderer();

    /// This funciton begins the scene renderering
    /// - Parameters:
    ///   - sceneCamera: Scene Renderer camera data
    ///   - directionLight: direction liht
    void BeginScene(const SceneRendererCamera& sceneCamera, const DirectionLight& directionLight);
    /// This funciton ends the scene renderering
    void EndScene();

    /// This function updates the viewport size of scene rendere
    /// - Parameters:
    ///   - width: width of viewport
    ///   - height: height of viewport
    void SetViewportSize(uint32_t width, uint32_t height);

    /// This funcion set the callback for debug renderer
    /// - Parameter func: callback funciton
    void SetDebugRenderer(const std::function<void()>& func);

    /// This function submits the mesh in scene
    /// - Parameters:
    ///   - meshHandle: mesh handle
    ///   - transform: mesh transform
    ///   - materilTable: Material table
    ///   - tilingFactor: tiling factor
    void SubmitMesh(AssetHandle meshHandle, const glm::mat4& transform, Ref<MaterialTable> materilTable, float tilingFactor);
    /// This function submits the mesh in scene
    /// - Parameters:
    ///   - mesh: mesh reference
    ///   - transform: mesh transform
    ///   - materialProperty: mesh material property
    ///   - materialtextures: mesh material texctures
    void SubmitSelectedMesh(AssetHandle meshHandle, const glm::mat4& transform, Ref<MaterialTable> materilTable, float tilingFactor);

    /// This function loads the skybox data and init the IBL
    /// - Parameter skyboxTexturePath: skybox image file path
    static void SubmitSkyboxImage(const std::string& skyboxTexturePath);
    /// This function loads the skybox data and init the IBL
    /// - Parameter skyboxAsset: skybox image asset
    static void SubmitSkyboxImage(const AssetHandle& skyboxAsset);

    /// This function returns the final render pass image
    Ref<Texture> GetFinalImage() const;

  private:
    // Member functions ---------------------------------------------------------------------------------------------
    /// This function renders the mesh in geometry
    /// - Parameters:
    ///   - mesh: Mesh to be render
    ///   - transform: mesh transform
    ///   - material: material
    ///   - tilingFactor: tiling factor
    void RenderMeshGeometry(Ref<Mesh> mesh, const glm::mat4& transform, float tilingFactor, Ref<Material> material);
    /// This function renders the submesh
    /// - Parameters:
    ///   - mesh: mesh
    ///   - transform: mesh transform
    ///   - material: material
    void RenderSubmesh(Ref<Mesh> mesh, const glm::mat4& transform, Ref<Material> material);

    /// This function render geometry Pass
    void GeometryPass();
    /// This function render composit Pass
    void CompositePass();
    /// This function passes the scene in bloom
    void BloomPass();
    /// This function renders the final pass image
    void FinalPass();

    /// This function loads the skybox image
    static void SubmitSkyboxImageImp();

    // Member Variables ---------------------------------------------------------------------------------------------
    std::string m_debugName;
    uint32_t m_viewportWidth, m_viewportHeight;

    // Debug Renderer from client
    std::function<void()> m_debugRenderer = []() {};

    struct MeshDrawData
    {
      Ref<Mesh> mesh;
      Ref<MaterialTable> materilTable;
      float tilingFactor;
      glm::mat4 transform;
    };
    std::vector<MeshDrawData> m_meshDrawList, m_selectedMeshDrawList;

    Ref<FrameBuffer> m_viewportRenderPass, m_geometryRenderPass;

    // Environment Data
    SceneRendererCamera sceneCamera;

    inline static Ref<Material> s_colorMaterial;
    inline static Ref<Shader> s_finalImageShader, s_equiractangularToCubemapShader, s_irradianceShader, s_prefilterShader, s_skymapShader;
    inline static Ref<Texture> s_envTexture, s_envCubemap, s_irradianceMap, s_prefilterMap;

    inline static SceneRendererCamera s_sceneCamera;
    inline static DirectionLight s_directionLight;
  };
} // namespace IKan
