//
//  ImGuiLayer.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "ImGuiLayer.hpp"
#include "Core/Application.hpp"
#include "Event/Event.h"

#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>

#include <GLFW/glfw3.h>

namespace IKan::UI
{
  ImguiLayer::ImguiLayer(Ref<Window> window)
  : Layer("Imgui Layer"), m_window(window)
  {
    IK_LOG_TRACE(LogModule::Imgui, "Creating Imgui Layer instance ...");
  }
  
  ImguiLayer::~ImguiLayer() {
    IK_LOG_WARN(LogModule::Imgui, "Destroying Imgui Layer instance !!!");
  }
  
  void ImguiLayer::OnAttach() {
    IK_LOG_TRACE(LogModule::Imgui, "'{0}' is attached to application", GetName());
    
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
  
  void ImguiLayer::OnDetach()
  {
    IK_LOG_WARN(LogModule::Imgui, "'{0}' is detached to application", GetName());
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
  
  void ImguiLayer::OnEvent(Event& event)
  {
    if (m_blockEvents)
    {
      ImGuiIO& io = ImGui::GetIO();
      event.m_isHandled |= event.IsInCategory(Event::EventCategoryMouse) & io.WantCaptureMouse;
      event.m_isHandled |= event.IsInCategory(Event::EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
  }
  
  void ImguiLayer::Begin()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    
    ImGui::NewFrame();
  }
  
  void ImguiLayer::End()
  {
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
  
  void ImguiLayer::SetFont(const std::vector<UI::Font> otherFonts)
  {
    if (otherFonts.size() == 0)
    {
      return;
    }
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->ClearFonts();
    
    IK_LOG_INFO(LogModule::Imgui, "Imgui Font changed");
    for (const auto& font : otherFonts)
    {
      io.Fonts->AddFontFromFileTTF(font.filePath.c_str(), font.size);
      IK_LOG_INFO(LogModule::Imgui, "  Font    {0} Size {1}", font.filePath.c_str(), font.size);
    }
    
    // Default font is Regular
    io.FontDefault = io.Fonts->Fonts[0];
  }
  
  void ImguiLayer::SetIniFilePath(const std::string& iniFilePath)
  {
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.IniFilename = iniFilePath.c_str();
    m_iniPath = iniFilePath;
  }
  
  void ImguiLayer::ReloadIni()
  {
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.IniFilename = m_iniPath.c_str();
  }
  
} // namespace IKan
