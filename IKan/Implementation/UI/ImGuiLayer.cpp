//
//  ImGuiLayer.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "ImGuiLayer.hpp"
#include "Core/Application.hpp"
#include "Event/Event.h"

#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>

#include <GLFW/glfw3.h>
#include <ImGuizmo.h>

namespace IKan::UI
{
  ImGuiLayer::ImGuiLayer(Ref<Window> window)
  : Layer("ImGui Layer"), m_window(window)
  {
    IK_LOG_INFO(LogModule::ImGui, "Creating ImGui Layer instance");
  }
  
  ImGuiLayer::~ImGuiLayer()
  {
    IK_LOG_INFO(LogModule::ImGui, "Destroying ImGui Layer instance");
  }
  
  void ImGuiLayer::OnAttach() 
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::ImGui, "Attaching '{0}' to application", GetName());
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform
    // windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    
    GLFWwindow* window = static_cast<GLFWwindow*>(m_window->GetNativeWindow());
    
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
  }
  
  void ImGuiLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::ImGui, "Detatching '{0}' from application", GetName());
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
  
  void ImGuiLayer::OnEvent(Event& event)
  {
    if (m_blockEvents)
    {
      ImGuiIO& io = ImGui::GetIO();
      event.m_isHandled |= event.IsInCategory(Event::EventCategoryMouse) & io.WantCaptureMouse;
      event.m_isHandled |= event.IsInCategory(Event::EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
  }
  
  void ImGuiLayer::Begin()
  {
    IK_PERFORMANCE("ImGuiLayer::Begin");
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
  }
  
  void ImGuiLayer::End()
  {
    IK_PERFORMANCE("ImGuiLayer::End");
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)m_window->GetWidth(), (float)m_window->GetHeight());
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backupCurrentContext);
    }
  }
  
  void ImGuiLayer::LoadFonts(const std::vector<UI::ImGuiFont>& otherFonts)
  {
    IK_PROFILE();
    RETURN_IF (otherFonts.size() == 0)
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->ClearFonts();
    
    IK_LOG_INFO(LogModule::ImGui, "Loading ImGui Fonts");
    for (const auto& font : otherFonts)
    {
      io.Fonts->AddFontFromFileTTF(font.filePath.c_str(), font.size);
      IK_LOG_INFO(LogModule::ImGui, "  Font    {0} Size {1}", Utils::FileSystem::IKanAbsolute(font.filePath).string(), font.size);
    }
    
    // Default font is Regular
    io.FontDefault = io.Fonts->Fonts[0];
  }
} // namespace IKan::UI
