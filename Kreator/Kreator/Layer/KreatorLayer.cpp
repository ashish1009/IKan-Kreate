//
//  KreatorLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/02/24.
//

#include "KreatorLayer.hpp"

namespace Kreator
{
  // Kretor Resource Path
#define KreatorResourcePath(path) std::filesystem::absolute(m_clientResourcePath / path)

  KreatorLayer* KreatorLayer::s_instance = nullptr;
  KreatorLayer& KreatorLayer::Get()
  {
    return *s_instance;
  }

  KreatorLayer::KreatorLayer(const std::filesystem::path& clientResourcePath, const std::filesystem::path& systemUserPath, const std::filesystem::path& iKanKreatePath, Ref<UserPreferences> userPreferences)
  : Layer("Kreator Renderer"), m_clientResourcePath(clientResourcePath), m_systemUserPath(systemUserPath), m_iKanKreatePath(iKanKreatePath), m_userPreferences(userPreferences)
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "RendererLayer instance already created");
    
    // Copy the single instance of application
    s_instance = this;
    
    IK_LOG_INFO("Kreator Layer", "Creating Kreator Renderer Layer instance");
    
    // Decorate the Application --------------------------------------------------------------------------------------
    // Set all the required Fonts
    IKan::UI::ImGuiFont regularFontFilePath = {KreatorResourcePath("Fonts/Opensans/Regular.ttf"), 14};
    IKan::UI::ImGuiFont boldFontFilePath = {KreatorResourcePath("Fonts/Opensans/ExtraBold.ttf"), 14};
    IKan::UI::ImGuiFont italicFontFilePath = {KreatorResourcePath("Fonts/Opensans/Italic.ttf"), 14};
    IKan::UI::ImGuiFont sameWidthFont = {KreatorResourcePath("Fonts/HfMonorita/Regular.ttf"), 10};
    IKan::UI::ImGuiFont hugeheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 40};
    IKan::UI::ImGuiFont semiheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 18};
    
    // Note: This API should be called before any other ImGui Decoration API 
    Kreator::UI::LoadFonts({regularFontFilePath, boldFontFilePath, italicFontFilePath, sameWidthFont, hugeheader, semiheader});

    // Set the Theme of ImGui as user preference
    Kreator::UI::SetThemeColors(m_userPreferences->theme);

    // Save the default project path
    std::filesystem::path defulatPath =  Utils::FileSystem::IKanAbsolute(m_clientResourcePath);
    m_projectFilePathBuffer.MemCpy(defulatPath.string().data(), 0, defulatPath.string().size());

    // Open or Create Project ---------------------------------------------------------------------------------------
    if (std::filesystem::exists(m_userPreferences->startupProject))
    {
      if (m_userPreferences->showWelcomeScreen)
      {
        m_showWelcomePopup = true;
      }
      else
      {
        IK_ASSERT(false, "Open Project. TODO: Implement Later ...");
      }
    }
    else
    {
      m_showWelcomePopup = true;
    }
    
    // Load Textures --------------------
    // Set the Application Icon
    m_applicationIcon = TextureFactory::Create(KreatorResourcePath("Textures/Logo/IKan.png"));
    m_welcomeIcon = TextureFactory::Create(KreatorResourcePath("Textures/Logo/WelcomeIKan.png"));
    
    // Other Icons
    m_newProjectIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/NewProject.png"));
    m_folderIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Folder.png"));
  }
  
  KreatorLayer::~KreatorLayer()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void KreatorLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Attaching '{0} Layer' to application", GetName());
  }
  
  void KreatorLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Detaching '{0} Layer' from application", GetName());
  }
  
  void KreatorLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    Renderer::Clear({0.2f, 0.2f, 0.2f, 1.0f});
  }
  
  void KreatorLayer::OnEvent(Event& event)
  {

  }
  
  void KreatorLayer::OnImGuiRender()
  {
    UI_WelcomePopup();
    UI_NewProjectPopup();
    UI_FolderExplorer();
  }
  
  // Project API ---------------------------------------
  void KreatorLayer::CreateProject(const std::filesystem::path& projectDir)
  {
    IK_PROFILE();
    if (!std::filesystem::exists(projectDir))
    {
      IK_LOG_INFO("Kreator Layer", "Creating Project at {0} ", Utils::FileSystem::IKanAbsolute(projectDir).string());
    }
  }
  
  const std::filesystem::path& KreatorLayer::GetClientResorucePath() const
  {
    return m_clientResourcePath;
  }
  const std::filesystem::path& KreatorLayer::GetSystemUserPath() const
  {
    return m_systemUserPath;
  }
  const std::filesystem::path& KreatorLayer::GetIKanKreatorPath() const
  {
    return m_iKanKreatePath;
  }
} // namespace Kreator
