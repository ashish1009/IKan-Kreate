//
//  KreatorLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/02/24.
//

#include "KreatorLayer.hpp"

namespace Kreator
{
  // Viewport -----------------------------------------------------------------------------------------------------
  void Viewport::GetMouseViewportSpace()
  {
    IK_PERFORMANCE("Viewport::GetMouseViewportSpace");
    auto [mx, my] = ImGui::GetMousePos();
    const auto& viewportBounds = bounds;
    mx -= viewportBounds[0].x;
    my -= viewportBounds[0].y;
    auto viewportWidth = viewportBounds[1].x - viewportBounds[0].x;
    auto viewportHeight = viewportBounds[1].y - viewportBounds[0].y;
    
    mouseViewportSpace = { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
  }
  bool Viewport::IsMouseSpaceInViewport()
  {
    return (mouseViewportSpace.spaceMouseX >= -1.0f and mouseViewportSpace.spaceMouseX <= 1.0f and
            mouseViewportSpace.spaceMouseY >= -1.0f and mouseViewportSpace.spaceMouseY <= 1.0f);
  }

  // Kreator Layer ---------------------------------------------------------------------------------------------------
  
#define RETRUN_IF_NO_PROJECT() \
if (!Project::GetActive()) return
  
#define RETRUN_IF_NO_SCENE() \
if (!m_currentScene) return
  
  // Kretor Resource Path
#define KreatorResourcePath(path) std::filesystem::absolute(m_clientResourcePath / path)

  KreatorLayer* KreatorLayer::s_instance = nullptr;
  KreatorLayer& KreatorLayer::Get()
  {
    return *s_instance;
  }

  KreatorLayer::KreatorLayer(const std::filesystem::path& clientResourcePath, const std::filesystem::path& systemUserPath,
                             const std::filesystem::path& iKanKreatePath, Ref<UserPreferences> userPreferences)
  : Layer("Kreator Renderer"), m_clientResourcePath(clientResourcePath), m_systemUserPath(systemUserPath), 
  m_iKanKreatePath(iKanKreatePath), m_userPreferences(userPreferences), m_viewportRenderer("Viewport"),
  m_miniViewportRenderer("Mini Viewport"), m_editorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 10000.0f)
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "RendererLayer instance already created");
    
    // Copy the single instance of application
    s_instance = this;
    
    IK_LOG_INFO("Kreator Layer", "Creating Kreator Renderer Layer instance");
    
    // Increase Line render limit for Colliders
    Renderer2D::AddLineData(100000);
    
    // Set debug renderer callback
    m_viewportRenderer.SetDebugRenderer([this]() { DebugRenderer(); });
    
    // Load Textures --------------------
    // Set the Application Icon
    m_applicationIcon = TextureFactory::Create(KreatorResourcePath("Textures/Logo/IKan.png"));
    m_welcomeIcon = TextureFactory::Create(KreatorResourcePath("Textures/Logo/WelcomeIKan.png"));
    
    // Other Textures
    m_newProjectIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/NewProject.png"));
    m_folderIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Folder.png"));
    m_gridTexture = TextureFactory::Create(KreatorResourcePath("Textures/Editor/Grid.png"));
    
    // Shadow Image
    m_shadowTexture = TextureFactory::Create(KreatorResourcePath("Textures/Icons/ShadowLineTop.png"));
    
    // Project Icon
    m_projectIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Project.png"));
    
    // Window Icons
    m_iconClose = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Close.png"));
    m_iconMinimize = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Minimize.png"));
    m_iconMaximize = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Maximize.png"));
    m_iconRestore = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Restore.png"));
    
    // Guizmo Button
    m_selectToolTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Pointer.png"));
    m_moveToolTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Move.png"));
    m_rotateToolTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Rotate.png"));
    m_scaleToolTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Scale.png"));
    m_gizmoModeTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/GuizmoMode.png"));
    
    // Scene Button
    m_playButtonTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Play.png"));
    m_stopButtonTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Stop.png"));
    m_simulateButtonTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Simulate.png"));
    m_pauseButtonTex = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Pause.png"));
    
    // Camera Editor Icon
    m_XIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/X.png"));
    m_YIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Y.png"));
    m_ZIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Z.png"));
    m_cameraIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Camera.png"));
  }
  
  KreatorLayer::~KreatorLayer()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void KreatorLayer::OnAttach()
  {

  }
  
  void KreatorLayer::OnDetach()
  {
  }
  
  void KreatorLayer::OnUpdate(TimeStep ts)
  {

  }
  
  void KreatorLayer::OnEvent(Event& event)
  {
  }
  
  void KreatorLayer::OnImGuiRender()
  {
  }

  void KreatorLayer::DebugRenderer()
  {
  }
} // namespace Kreator
