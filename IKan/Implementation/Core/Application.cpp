//
//  Application.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "Application.hpp"
#include "Renderer/RendererStats.hpp"

// TODO: To Move rendere layer later
#include "Renderer2D.hpp"
#include "Texture.hpp"
#include "EditorCamera.hpp"
#include "Font.hpp"

namespace IKan
{
  // TODO: To Move rendere layer later
  static Ref<Image> m_image;
  static Ref<EditorCamera> m_camera;
  
  Application::Application(const Specification& spec)
  : m_specificaion(spec)
  {
    IK_PROFILE();

    IK_ASSERT(!s_instance, "Application instance already created");
    // Copy the single instance of application
    s_instance = this;

    IK_LOG_TRACE(LogModule::Application, "Creating Core Application Instance : {0}", m_specificaion.name);
    
    // Create Renderer Instance before any GLFW or Renderer Context
    // NOTE: Creating the Renderer Data Memory in very begining as this will setup the Renderer API to be used to
    //       create any Renderer Implementation
    Renderer::CreateRendererData(m_specificaion.renderingApi);

    // Update the core engine directory path
    CoreDirectory::SetPath(m_specificaion.engineInstallPath);

    // Create the window
    m_window = Window::Create(m_specificaion.windowSpecification);
    
    // Set the application callback to window
    m_window->SetEventFunction(IK_BIND_EVENT_FN(Application::HandleEvents));
    
    // Control Window
    m_window->SetResizable(m_specificaion.resizable);
    if (m_specificaion.startMaximized)
    {
      m_window->Maximize();
    }

    // Initialise the Core Renderer
    Renderer::Initialize();
    
    // TODO: To Move Renderer Layer later
    {
      m_image = Image::Create(CoreAssetPath("Textures/Default/NoTexture.png"));
      m_camera = CreateRef<EditorCamera>(45, 1600, 900);
    }

    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                     Core Application Initialised                         ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
  }
  
  Application::~Application()
  {
    IK_PROFILE();

    // TODO: To Move Renderer Layer later
    {
      m_image.reset();
    }

    // Reset the window
    m_window.reset();

    // Shutdown the Renderer
    Renderer::Shutdown();

    IK_LOG_WARN(LogModule::Application, "Destroying Core Application Instance : {0}", m_specificaion.name);
  }

  void Application::Run()
  {
    {
      IKan::ScopedTimer onShutdown("Client Application Initialize()");
      // Initialise the client application
      OnInit();
    }
    // GAME LOOP -----------------------------------------------------------------------------------------------------
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                          Starting Game Loop                              ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    
    while (m_isRunning)
    {
      // Update the window swap buffers
      m_window->Update();

      // Store the frame time difference
      m_timeStep = m_window->GetTimestep();

      // TODO: To Move Renderer Layer later
      {
        m_camera->SetActive(true);
        m_camera->OnUpdate(m_timeStep);
        
        Renderer2D::BeginRenderPass();
        Renderer::Clear({0.12f, 0.12f, 0.18f, 1.0f});

        Renderer2D::BeginBatch(m_camera->GetUnReversedViewProjection());

        Renderer2D::DrawQuad({1, 1, 0}, {1, 1, 1}, {0, 0, 0}, m_image);
        Renderer2D::DrawCircle({0, 0, 0});
        Renderer2D::DrawRect({0, 0, 0}, {4, 4}, {1, 0, 0.5, 1});
        Renderer2D::RenderText("Sample Test Text", Font::GetDefaultFont(), {0, -1, 0}, {1, 1}, {1, 1, 1, 1}, -1);

        Renderer2D::EndBatch();
        Renderer2D::EndRenderPass();
        
        Renderer2D::DrawFullscreenQuad(Renderer2D::GetFinalImage());
      }
      
      // Update the client application
      OnUpdate(m_timeStep);
      
      // Render the Gui for Application
      ImguiRender();
      
      // Clear Runtime Performance profiler
      PerformanceProfiler::Get()->Clear();
      
      // Reset Statistics each frame
      RendererStatistics::Get().ResetEachFrame();
    }
    
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                           Ending Game Loop                               ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");

    {
      IKan::ScopedTimer onShutdown("Client Application Shutdown()");
      // Shutdown the client application
      OnShutdown();
    }
  }
  
  void Application::HandleEvents(Event &event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(IK_BIND_EVENT_FN(Application::WindowClose));
        
    // Client side event handler funciton
    OnEvent(event);
    
    // TODO: To Move Renderer Layer later
    {
      m_camera->OnEvent(event);
    }
  }
  
  bool Application::WindowClose([[maybe_unused]] WindowCloseEvent& window_close_event)
  {
    Close();
    return false;
  }
  
  void Application::Close()
  {
    m_isRunning = false;
    IK_LOG_WARN(LogModule::Application, "Closing the Application");
  }
  
  void Application::ImguiRender()
  {
    // Rendering Imgui for Client
    OnImguiRender();
  }

  void* Application::GetWindowPtr() const
  {
    return m_window->GetNativeWindow();
  }
  
  Window& Application::GetWindow()
  {
    return *(m_window.get());
  }

  Application& Application::Get()
  {
    return *s_instance;
  }
  
} // namespace IKan
