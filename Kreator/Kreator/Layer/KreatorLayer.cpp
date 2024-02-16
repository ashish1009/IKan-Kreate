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

  KreatorLayer::KreatorLayer(const std::filesystem::path& clientResourcePath, Ref<UserPreferences> userPreferences)
  : Layer("Kreator Renderer"), m_clientResourcePath(clientResourcePath), m_userPreferences(userPreferences)
  {
    IK_PROFILE();
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
  }
  
  // Project API ---------------------------------------
  void KreatorLayer::CreateProject(const std::filesystem::path& projectDir)
  {
    IK_PROFILE();
    if (!std::filesystem::exists(projectDir))
    {
      IK_LOG_INFO("Kreator Layer", "Creating Project at {0} ", Utils::FileSystem::IKanAbsolute(projectDir));
    }
  }
} // namespace Kreator
