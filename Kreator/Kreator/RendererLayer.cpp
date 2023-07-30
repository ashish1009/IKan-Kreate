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

  RendererLayer::RendererLayer(Ref<UserPreferences> userPreference, const std::filesystem::path& clientDirPath)
  : Layer("Kreator Renderer"), m_editorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 1000.0f)
  , m_userPreferences(userPreference), m_clientDirPath(clientDirPath)
  {
    IK_LOG_TRACE("Kreator Layer", "Creating Kreator Renderer Layer instance");
    
    m_projectNameBuffer = iknew char[MAX_PROJECT_NAME_LENGTH];
    m_projectFilePathBuffer = iknew char[MAX_PROJECT_FILEPATH_LENGTH];
    
    m_templateProjectDir = m_clientDirPath / "Resources/TemplateProject";
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
    IK_LOG_TRACE("Kreator Layer", "Creating Project {0} at ", projectDir.string().c_str());
    
    // Close the current Project
    if (Project::GetActive())
    {
      CloseProject();
    }
    
    // Copy the template files
    Utils::FileSystem::Copy(m_templateProjectDir, projectDir);
    
    // Open Template Project file
    std::ifstream stream(projectDir / "TemplateProject.ikproj");
    std::stringstream ss;
    ss << stream.rdbuf();
    stream.close();

    // Rename the Project name in file
    std::string str = ss.str();
    KreatorUtils::ReplaceProjectName(str, m_projectNameBuffer);
    
    // Open Projecy file again
    std::ofstream ostream(projectDir / "TemplateProject.ikproj");
    ostream << str;
    ostream.close();

    // Rename the file name
    std::string newProjectFileName = std::string(m_projectNameBuffer) + ProjectExtension;
    Utils::FileSystem::Rename(projectDir / "TemplateProject.ikproj", projectDir / newProjectFileName);
    
    // Create Required Direcotries
    Utils::FileSystem::CreateDirectory(projectDir / "Assets" / "Textures");
    Utils::FileSystem::CreateDirectory(projectDir / "Assets" / "Fonts");
    Utils::FileSystem::CreateDirectory(projectDir / "Assets" / "Scenes");
  }
  
  void RendererLayer::OpenProject(const std::string &filepath)
  {
    IK_LOG_TRACE("Kreator Layer", "Opening Project {0}", filepath.c_str());

    // Reset Project Name and directory
    memset(m_projectNameBuffer, 0, MAX_PROJECT_NAME_LENGTH);
    memset(m_projectFilePathBuffer, 0, MAX_PROJECT_FILEPATH_LENGTH);
    
    // Check if File exists
    if (!Utils::FileSystem::Exists(filepath))
    {
      IK_LOG_ERROR("Kreator Layer", "Tried to open a project that doesn't exist. Project path: {0}", filepath);
      
      // TODO: Remove Assert after testing. may be this will never hit just precaution check
      IK_ASSERT(false);
      return;
    }

    // Close the current Project
    if (Project::GetActive())
    {
      CloseProject();
    }

    // Create new project fill the config with file data
    Ref<Project> project = CreateRef<Project>();
    ProjectSerializer serializer(project);
    serializer.Deserialize(filepath);
    
    Project::SetActive(project);
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
    if (!Project::GetActive())
    {
      // FIXME: (Kreator) Fix Later
      IK_ASSERT(false);
    }
    
    Ref<Project> project = Project::GetActive();
    ProjectSerializer serializer(project);
    serializer.Serialize(project->GetConfig().projectDirectory + "/" + project->GetConfig().projectFileName);
  }
} // namespace Kreator
