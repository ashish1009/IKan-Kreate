//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#include "RendererLayer.hpp"

namespace Kreator
{
  // Kretor Resource Path
#define KreatorResourcePath(path) Utils::FileSystem::Absolute(m_clientResourcePath / path)

  RendererLayer* RendererLayer::s_instance = nullptr;
  
  RendererLayer& RendererLayer::Get()
  {
    return *s_instance;
  }
  
  RendererLayer::RendererLayer(Ref<UserPreferences> userPreference, const std::filesystem::path& clientResourcePath)
  : Layer("Kreator Renderer"), m_userPreferences(userPreference), m_clientResourcePath(clientResourcePath)
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "RendererLayer instance already created");
    // Copy the single instance of application
    s_instance = this;
    
    IK_LOG_TRACE("Kreator Layer", "Creating Kreator Renderer Layer instance");
    
    // Save All Project Path
#ifdef DEBUG
    m_allProjectsPath = m_clientResourcePath / "../Projects";
#else
    m_allProjectsPath = m_clientResourcePath / "../Projects";
#endif

    // Save the template project dir
    m_templateProjectDir = m_clientResourcePath / "TemplateProject";
  }
  
  RendererLayer::~RendererLayer()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void RendererLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("Kreator Layer", "Attaching Kreator Renderer Layer to application");
    
    // Decorate the Theme ------------------------------------------------------------------------------------------
    // Set all the required Fonts
    UI::Font regularFontFilePath = {KreatorResourcePath("Fonts/Opensans/Regular.ttf"), 14};
    UI::Font boldFontFilePath = {KreatorResourcePath("Fonts/Opensans/ExtraBold.ttf"), 14};
    UI::Font italicFontFilePath = {KreatorResourcePath("Fonts/Opensans/Italic.ttf"), 14};
    UI::Font sameWidthFont = {KreatorResourcePath("Fonts/HfMonorita/Regular.ttf"), 10};
    UI::Font hugeheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 40};
    UI::Font semiheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 18};
    UI::Theme::ChangeFont({regularFontFilePath, boldFontFilePath, italicFontFilePath, sameWidthFont, hugeheader, semiheader});
    
    // Set the Theme of ImGui as user preference
    switch (m_userPreferences->theme)
    {
      case UserPreferences::Theme::KreatorDark:
        Kreator_UI::SetDarkTheme();
        break;
        
      default:
      case UserPreferences::Theme::Default:
        break;
    }
  }
  
  void RendererLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Detaching Kreator Renderer Layer from application");
  }
  
  void RendererLayer::OnUpdate(TimeStep ts)
  {

  }
  
  void RendererLayer::OnEvent(Event& event)
  {

  }
  
  void RendererLayer::OnImGuiRender()
  {
    
  }

  std::filesystem::path RendererLayer::GetClientResorucePath() const
  {
    return m_clientResourcePath;
  }
} // namespace Kreator
