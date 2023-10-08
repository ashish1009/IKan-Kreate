//
//  DummyApplication.hpp
//  Kreator
//
//  Created by Ashish . on 08/09/23.
//

#include <glad/glad.h>

namespace Kreator
{
  class DummyApp : public Application
  {
  public:
    DummyApp(const Application::Specification& appSpec, const std::string& clientResourcePath, const std::string& startupProject)
    : Application(appSpec)
    {
    }
    
    ~DummyApp()
    {
    }
    
    void OnInit() override
    {
      camera.SetViewportSize(1600, 900);
      FixedCamera::SetViewport(1600, 900 );
    }
    
    void OnShutdown() override
    {
      
    }
    
    void OnUpdate(TimeStep ts) override
    {
      Renderer::Clear({0.21, 0.21, 0.21, 1});
      static constexpr glm::vec3 position = { 155.0f, 155.0f, 0.3f };
      static constexpr glm::vec2 size = {0.3f, 0.3f};
      static constexpr glm::vec4 color = { 0.23f, 0.33f, 0.22f, 1.0f};
      
      TextRenderer::BeginBatch(FixedCamera::s_projection);
      TextRenderer::RenderFixedViewText(std::to_string((uint32_t)(ImGui::GetIO().Framerate)), Font::GetDefaultFont(),
                                        position, size, color, -1);
      TextRenderer::EndBatch();
    }
    
    void OnEvent(Event& e) override
    {
      camera.OnEvent(e);
    }

  private:
    EditorCamera camera = EditorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 10000.0f);
  };
} // namespace Kreator
