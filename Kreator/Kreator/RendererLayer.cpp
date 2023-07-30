//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#include "RendererLayer.hpp"

namespace Kreator
{
  // TODO: Temp
  static Ref<Image> m_image;
  
  // Kretor Resource Path
#define KreatorResourcePath(path) m_clientDirPath / "Resources" / path

  RendererLayer::RendererLayer(Ref<UserPreferences> userPreference, const std::filesystem::path& clientDirPath)
  : Layer("Kreator Renderer"), m_editorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 1000.0f)
  , m_userPreferences(userPreference), m_clientDirPath(clientDirPath)
  {
    IK_LOG_TRACE("Kreator Layer", "Creating Kreator Renderer Layer instance");
    
    m_projectNameBuffer = iknew char[MAX_PROJECT_NAME_LENGTH];
    m_projectFilePathBuffer = iknew char[MAX_PROJECT_FILEPATH_LENGTH];
  }
  
  RendererLayer::~RendererLayer()
  {
    ikdelete m_projectNameBuffer;
    ikdelete m_projectFilePathBuffer;
    
    IK_LOG_WARN("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void RendererLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("Kreator Layer", "Attaching Kreator Renderer Layer to application");
    
    // Open or Create Project
    if (Utils::FileSystem::Exists(m_userPreferences->startupProject))
    {
      OpenProject(m_userPreferences->startupProject);
    }
    else
    {
      auto projName = Utils::String::GetFileNameFromPath(m_userPreferences->startupProject);
      auto projDir = Utils::String::GetDirectoryFromPath(m_userPreferences->startupProject);
      
      memcpy(m_projectNameBuffer, projName.c_str(), projName.size());
      CreateProject(projDir);
    }

    // TODO: Temp
    {
      m_image = Image::Create(KreatorResourcePath("Textures/checkerboard.png"));
      m_editorScene = Scene::Create();
      [[maybe_unused]] auto quad = m_editorScene->CreateEntity("Quad");
      [[maybe_unused]] auto circle = m_editorScene->CreateEntity("Circle");
      [[maybe_unused]] auto text = m_editorScene->CreateEntity("Text");
      [[maybe_unused]] auto camera = m_editorScene->CreateEntity("Camera");
      camera.AddComponent<SceneCamera>();
    }
  }
  
  void RendererLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Detaching Kreator Renderer Layer from application");
    
    // TODO: Temp
    {
      m_image.reset();
    }
  }
  
  void RendererLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    
    // TODO: Temp
    {
      m_editorCamera.SetActive(true);
      m_editorCamera.OnUpdate(ts);

      Renderer2D::BeginRenderPass();
      Renderer::Clear({0.12f, 0.12f, 0.18f, 1.0f});
      
      Renderer2D::BeginBatch(m_editorCamera.GetUnReversedViewProjection());
      
      Renderer2D::DrawQuad({1, 1, 0}, {1, 1, 1}, {0, 0, 0}, m_image);
      Renderer2D::DrawCircle({0, 0, 0});
      Renderer2D::DrawRect({0, 0, 0}, {4, 4}, {1, 0, 0.5, 1});
      Renderer2D::RenderText("Sample Test Text", Font::GetDefaultFont(), {0, -1, 0}, {1, 1}, {1, 1, 1, 1}, -1);
      
      Renderer2D::EndBatch();
      Renderer2D::EndRenderPass();
    }
  }
  
  void RendererLayer::OnEvent(Event& event)
  {
    m_editorCamera.OnEvent(event);
  }
  
  void RendererLayer::OnImguiRender()
  {
    IK_PERFORMANCE("RendererLayer::OnImguiRender");
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
        
    auto viewportSize = ImGui::GetContentRegionAvail();
        
    // Render viewport image
    ImGui::Image(INT2VOIDP(Renderer2D::GetFinalImage()->GetRendererID()), viewportSize,
                 {0, 1}, {1, 0});
    
    ImGui::End();
    ImGui::PopStyleVar();
  }
  
  void RendererLayer::CreateProject(const std::filesystem::path &projectDir)
  {
    IK_ASSERT(false);
  }
  
  void RendererLayer::OpenProject(const std::string &filepath)
  {
    IK_ASSERT(false);
  }
  
  void RendererLayer::OpenProject()
  {
    IK_ASSERT(false);
  }
  
  void RendererLayer::CloseProject(bool unloadProject)
  {
    SaveProject();
  }
  
  void RendererLayer::SaveProject()
  {
    IK_ASSERT(false);
  }
} // namespace Kreator
