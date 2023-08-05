//
//  ImGuiLayer.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Core/Layer.hpp"
#include "Core/TimeStep.hpp"

namespace IKan
{
  class Window;
} // namespace IKan

namespace IKan::UI
{
  struct Font
  {
    std::string filePath;
    uint32_t size;
  };

  /// This class creates and manages the ImGui Layer in application
  /// - Note: If Not using iKan Application then create instance of ImGui Layer in Client Application and manage API
  ///         Accordingly, by default Application is taking care of all
  class ImGuiLayer : public Layer
  {
  public:
    /// This constructor creates the ImGui layer instance.
    /// - Parameter windowPointer: application window pointer
    ImGuiLayer(Ref<Window> windowPointer);
    /// This destrictor destroy the ImGui layer instance
    ~ImGuiLayer();
    
    /// This funciton initialises the imgui layer. Creates the imgui layer using imgui file library
    void OnAttach() override;
    /// This funciton destroy the imgui layer delete all the imgui created impl
    void OnDetach() override;
    /// This function do nothing for now
    void OnImGuiRender() override {}
    /// This function do nothing for now
    void OnUpdate([[maybe_unused]] TimeStep ts) override {}
    /// This function handles all the events triggered for the imgui layer. To be called whenever an event is triggered in the window
    /// - Note: Core ikan::Application is taking care to call the HandleEvents(event) API for all Layers
    /// - Parameter event: Event (Base class) intance. Dispatch event from Event Dispatcher
    void OnEvent(Event& event) override;
    
    /// This function begins the imgui rendering for each frame NOTE: Call once per frame
    void Begin();
    /// This function ends the rendering of imgui each frame NOTE: Call once per frame
    void End();
    
    /// This function overrides font of imgui with new font path
    /// - Parameter fonts: fonts
    void SetFont(const std::vector<UI::Font> fontsW);
    /// This function the save location of ini file path
    /// - Parameter iniFilePath: file path relative to executable
    /// - Note: Directory must exist.
    void SetIniFilePath(const std::string& iniFilePath);
    /// This function reload the Ini
    void ReloadIni();
    
    DELETE_COPY_MOVE_CONSTRUCTORS(ImGuiLayer);
    
  private:
    bool m_blockEvents = true;
    Ref<Window> m_window;
    std::string m_iniPath = "ikan.ini";
  };
} // namespace IKan
