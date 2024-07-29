//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/07/24.
//

#include "RendererLayer.hpp"

namespace Kreator
{
  Ref<Mesh> meshCube;
  Ref<Material> meshMaterial;
  Ref<Image> texImage;

  namespace KreatorUtils
  {
    /// This function replace the project name
    /// - Parameters:
    ///   - str: Project file content
    ///   - projectName: New project name
    static void ReplaceProjectName(std::string& str, const std::string& projectName)
    {
      static const char* projectNameToken = "$PROJECT_NAME$";
      size_t pos = 0;
      while ((pos = str.find(projectNameToken, pos)) != std::string::npos)
      {
        str.replace(pos, strlen(projectNameToken), projectName);
        pos += strlen(projectNameToken);
      }
    }
  } // namespace Utils

  RendererLayer* RendererLayer::s_instance = nullptr;
  RendererLayer& RendererLayer::Get()
  {
    return *s_instance;
  }

  RendererLayer::RendererLayer(const KreatorDirectories& directories)
  : Layer("Kreator Renderer"), m_directories(directories), m_viewportRenderer("Primary Viewport"), m_miniViewportRenderer("Mini Viewport"),
  m_secondaryViewportRenderer("Secondary Viewport")
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "RendererLayer instance already created");
    
    // Copy the single instance of application
    s_instance = this;
    
    IK_LOG_INFO("RendererLayer", "Creating '{0} Layer' instance", GetName());
  }
  
  RendererLayer::~RendererLayer()
  {
    IK_PROFILE();
    IK_LOG_WARN("RendererLayer", "Destroying '{0} Layer' instance", GetName());
  }
  
  void RendererLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("RendererLayer", "Attaching '{0} Layer' to application", GetName());
    
    // Create the Editor logger
    // Profiler logger
    Logger::Create(LoggerSpecification::Create().
                   Type(LogType::Editor).
                   Level(LogLevel::Trace).
                   Name("PROFILER").
                   OverrideSink(CreateRef<EditorConsoleSink>(1)));
    
    meshCube = Mesh::Create("/Users/ashish./iKan_storage/Github/Product/Kreator/Kreator/Projects/3D/TestProject/Assets/Meshes/Default/Cube.fbx");
    meshMaterial = Material::Create("/Users/ashish./iKan_storage/Github/Product/IKan-Kreate/IKan/Assets/Shaders/PBR_StaticShader.glsl");
    texImage = TextureFactory::Create("/Users/ashish./iKan_storage/Github/Product/Kreator/Kreator/Outputs/WelcomeScreen.png");
    
    m_viewportRenderer.SetDebugRenderer([this](){
      Renderer2D::BeginBatch(m_editorCamera.GetUnReversedViewProjection(), m_editorCamera.GetViewMatrix());
      Renderer2D::SubmitQuad({0.5, 0.5, 0}, {0.5, 0.5}, {0, 0, 0}, {1, 1, 1, 1}, texImage);
      Renderer2D::SubmitRect({0.0, 0.0, 0}, {0.5, 0.5});
      Renderer2D::SubmitText("Sample Text", {0, 0, 0}, {0.1, 0.1});
      Renderer2D::SubmitCircle({-0.5, -0.5, 0}, 0.5);
      Renderer2D::EndBatch();
      
      meshCube->GetPipeline()->Bind();
      static glm::mat4 transform = Utils::Math::UnitMat4;
      meshMaterial->Set("u_ViewProjection", m_editorCamera.GetUnReversedViewProjection());
      for (const SubMesh& submesh : meshCube->GetSubMeshes())
      {
        meshMaterial->Set("u_Transform", transform * submesh.transform);
        meshMaterial->Bind();
        Renderer::DrawIndexedBaseVertex(submesh.indexCount, (void*)(sizeof(uint32_t) * submesh.baseIndex), submesh.baseVertex);
      }
    });
    
    // Create new project
    CreateProject("../../../Kreator/Projects/FirstProject");
  }
  
  void RendererLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("RendererLayer", "Detaching '{0} Layer' from application", GetName());
    
    meshCube.reset();
    meshMaterial.reset();
    texImage.reset();
  }
  
  void RendererLayer::OnUpdate([[maybe_unused]] TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    
    m_editorCamera.OnUpdate(ts);
    m_editorCamera.SetActive(true);

    m_viewportRenderer.BeginScene();
    m_viewportRenderer.EndScene();
  }
  
  void RendererLayer::OnEvent([[maybe_unused]] Event& event)
  {
    m_editorCamera.OnEvent(event);
  }
  
  void RendererLayer::OnImGuiRender()
  {
    // Docked Windows-----------
    UI_StartMainWindowDocking();
    UI_Viewport();
        
    // Should be rendered last inside docker
    UI_StatisticsPanel();

    UI_EndMainWindowDocking();
  }
  
  void RendererLayer::UpdateViewportSize()
  {
    IK_PROFILE()
        
    FixedCamera::SetViewport(m_primaryViewport.width, m_primaryViewport.height);
    
    m_editorCamera.SetViewportSize(m_primaryViewport.width, m_primaryViewport.height);
    
    m_viewportRenderer.SetViewportSize(m_primaryViewport.width, m_primaryViewport.height);
    m_miniViewportRenderer.SetViewportSize(m_primaryViewport.width, m_primaryViewport.height);
    m_secondaryViewportRenderer.SetViewportSize(m_primaryViewport.width, m_primaryViewport.height);
  }
  
  void RendererLayer::CreateProject(const std::filesystem::path &newProjectFilePath)
  {
    IK_PROFILE();
    
    // If file do not exist, create the directory
    if (!std::filesystem::exists(newProjectFilePath))
    {
      IK_LOG_INFO("RendererLayer", "Creating Project at {0} ", Utils::FileSystem::IKanAbsolute(newProjectFilePath).string());
      
      // Close current project
      if (Project::GetActive())
      {
        CloseProject();
      }

      // Create project directory
      // Copy the template files
      std::filesystem::path templateProjectDir = m_directories.clientResourcePath / "TemplateProject";
      if (Utils::FileSystem::Copy(templateProjectDir, newProjectFilePath))
      {
        // Open Template Project file
        std::ifstream stream(newProjectFilePath / "TemplateProject.ikproj");
        std::stringstream ss;
        ss << stream.rdbuf();
        stream.close();
        
        // Rename the Project name in file
        std::string str = ss.str();
        KreatorUtils::ReplaceProjectName(str, newProjectFilePath.filename());
        
        // Open Project file again
        std::ofstream ostream(newProjectFilePath / "TemplateProject.ikproj");
        ostream << str;
        ostream.close();
        
        // Rename the file name
        std::string newProjectFileName = std::string(newProjectFilePath.filename()) + ProjectExtension;
        Utils::FileSystem::Rename(newProjectFilePath / "TemplateProject.ikproj", newProjectFilePath / newProjectFileName);
        
        // Create Required Directories
        std::filesystem::create_directory(newProjectFilePath / "Assets/Textures");
        std::filesystem::create_directory(newProjectFilePath / "Assets/Fonts");
        std::filesystem::create_directory(newProjectFilePath / "Assets/Scenes");
        std::filesystem::create_directory(newProjectFilePath / "Assets/Meshes");
        std::filesystem::create_directory(newProjectFilePath / "Assets/Materials");
        std::filesystem::create_directory(newProjectFilePath / "Assets/Physics");
        std::filesystem::create_directory(newProjectFilePath / "Assets/Prefabs");
        
        // Add more based on Project config
      }
      else
      {
        IK_LOG_CRITICAL("RendererLayer", "Directory could not be opened. Source directory : {0}. Destination directory : {1}",
                        templateProjectDir.string(), newProjectFilePath.string());
      }
    }
    
    // Open project if directory exists
    std::filesystem::path projectFile = newProjectFilePath / (std::string(newProjectFilePath.filename()) + ProjectExtension);
    OpenProject(projectFile);
  }
  
  void RendererLayer::OpenProject(const std::filesystem::path &projectFilePath)
  {
    IK_PROFILE();
    IK_LOG_INFO("RendererLayer", "Opening Project {0}", Utils::FileSystem::IKanAbsolute(projectFilePath).string());
    
    // Check if File exists
    if (!std::filesystem::exists(projectFilePath))
    {
      IK_ASSERT(false, "Tried to open a project that doesn't exist. Need to fix ...");
    }
    
    // Check if File is valid kreator project
    if (projectFilePath.extension() != ProjectExtension)
    {
      IK_ASSERT(false, "Invalid project file. Need to fix ...");
    }
    
    // Close the current Project
    if (Project::GetActive())
    {
      CloseProject();
    }

    // Create new project fill the config with file data
    Ref<Project> project = CreateRef<Project>();
    ProjectSerializer serializer(project);
    serializer.Deserialize(projectFilePath);
    Project::SetActive(project);
  }
  
  void RendererLayer::CloseProject()
  {
    IK_PROFILE();
    IK_ASSERT(false, "Implement Later");
  }
  
  // UI APIs ---------------------------------------------------------------------------------------------------------
  void RendererLayer::UI_StartMainWindowDocking()
  {
    IK_PERFORMANCE("RendererLayer::UI_StartMainWindowDocking");
    static bool optFullscreenPersistant = true;
    static ImGuiDockNodeFlags optFlags = ImGuiDockNodeFlags_None;
    bool optFullscreen = optFullscreenPersistant;
    
    // ImGui Input Output Fonts
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    
    io.ConfigWindowsResizeFromEdges = io.BackendFlags & ImGuiBackendFlags_HasMouseCursors;
    
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
    if (optFullscreen)
    {
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
      windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
    ImGui::Begin("DockSpace Demo", nullptr, windowFlags);
    ImGui::PopStyleColor(); // MenuBarBg
    ImGui::PopStyleVar(2);
    
    if (optFullscreen)
    {
      ImGui::PopStyleVar(2);
    }
    
    // Dockspace
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 250.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
      ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), optFlags);
    }
    style.WindowMinSize.x = minWinSizeX;
  }
  
  void RendererLayer::UI_EndMainWindowDocking()
  {
    IK_PERFORMANCE("RendererLayer::UI_EndMainWindowDocking");
    ImGui::End();
  }
  
  void RendererLayer::UI_Viewport()
  {
    IK_PERFORMANCE("RendererLayer::UI_Viewport")
    
    UI::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");

    m_primaryViewport.isHovered = ImGui::IsWindowHovered();
    m_primaryViewport.isFocused = ImGui::IsWindowFocused();

    const ImVec2& size = ImGui::GetContentRegionAvail();

    // Updating the View port size and kreator data w.r.t. primary viewport
    bool zeroSizeViewport = 0 == size.x or 0 == size.y;
    bool sameSizeViewport = size.x == m_primaryViewport.width and size.x == m_primaryViewport.height;
    if(!zeroSizeViewport and !sameSizeViewport)
    {
      m_primaryViewport.width = static_cast<uint32_t>(size.x);
      m_primaryViewport.height = static_cast<uint32_t>(size.y);
      
      // Set Viewport of Dummy Data
      UpdateViewportSize();
    }
    
    // Render viewport image
    UI::Image(m_viewportRenderer.GetFinalImage(), size);

    ImGui::End();
  }
  
  void RendererLayer::UI_StatisticsPanel()
  {
    IK_PERFORMANCE("RendererLayer::UI_StatisticsPanel");
    if (ImGui::Begin("Statistics"))
    {
      const auto& perFrameData = PerformanceProfiler::Get().GetPerFrameData();
      for (auto&& [name, time] : perFrameData)
      {
        ImGui::Text("%s : %f", name, time);
      }

      ImGui::End();
    }
  }
} // namespace Kreator
