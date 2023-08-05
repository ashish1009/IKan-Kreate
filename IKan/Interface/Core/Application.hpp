//
//  Application.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "Core/TimeStep.hpp"
#include "Core/CoreWindow.hpp"
#include "Core/LayerStack.hpp"
#include "Renderer/Renderer.hpp"
#include "UI/ImGuiLayer.hpp"

namespace IKan
{
  /// This is the Base Application class. This holds the Base functionality of IKan Engine.
  /// - Note: IKan Entry points instantiate this Application, so in order to use IKan Entry point client must Derive
  ///         the application from IKan::Application. Client can override the virtual methods to implement their
  ///         functionalities.
  class Application
  {
  public:
    /// This structure holds the specification of application provided by client
    struct Specification
    {
      std::string name = "iKan";
      std::string engineResourcesPath;
      std::string iniPath = "ikan.ini";

      Renderer::Api renderingApi = Renderer::Api::None;
      Window::Specification windowSpecification;

      bool startMaximized = false;
      bool resizable = true;
    };

    /// This constructor instantiate the IKan Core application. Responsible to create Window, Renderer Context and
    /// Initialize all the Renderer. Also add the ImGui Layer for GUI Rendering
    /// - Parameter spec: Application specifications
    Application(const Specification& spec);
    /// This destructor destroy the Application instance and shutdown all the Renderers and Window
    virtual ~Application();

    // Fundamental APIs ----------------------------------------------------------------------------------------------
    /// This function runs the game loop of the application. This function is responsible for:
    ///   - Updating the Application
    ///   - Updating each Layer frame
    ///   - Updating the window and Swap the buffers
    ///   - Render the GUI by calling 'RenderGui()'
    /// - Important: This API performs all the above actions for each API. In Core IKan::entry_point this funciton
    ///              is called by default
    void Run();
    /// This function handles all the events of window. Application is dispatching the events and perform the interupt
    /// actions
    /// - Parameter event: event abstract type
    void HandleEvents(Event& event);
    /// This Function responsible for closing the current applciation (IKan::Applciation) adn ends the game loop
    void Close();

    /// This function Push the layer of type IKan::Layer in Core Application layer stack.
    /// - Note: Also attach the layer (initialise it)
    /// - Parameter layer: Layer Reference pointer to be added
    void PushLayer(const Ref<Layer>& layer);
    /// This function Pop the layer of type IKan::Layer from Core Application layer stack.
    /// - Note: Also detach the layer (destroy it)
    /// - Parameter layer: Layer Reference pointer to be removed
    void PopLayer(const Ref<Layer>& layer);

    // Virtual APIs --------------------------------------------------------------------------------------------------
    /// This function calls before game loop
    virtual void OnInit() {}
    /// This function calls After game loop
    virtual void OnShutdown() {}
    /// This function calls in Game Loop Running
    virtual void OnUpdate(TimeStep ts) {}
    /// This function calls when an event triggers
    virtual void OnEvent(Event& event) {}
    /// This function calls in Game Loop under Gui Layer
    virtual void OnImGuiRender() {}

    // Getters -------------------------------------------------------------------------------------------------------
    /// This function returns the specification of application
    const Specification& GetSpecification() const;
    /// This function returns the time step of frame
    TimeStep GetTimestep() const;
    /// This fuinction returns the native pointer Window native as void
    void* GetWindowPtr() const;
    /// This fuinction returns the iKan Window Instance as reference
    Window& GetWindow();
    /// This fuinction returns the ImGui Layer pointer Reference
    UI::ImGuiLayer& GetImGuiLayer() const;

    /// This function returns the single instance of application
    static Application& Get();
    
    DELETE_COPY_MOVE_CONSTRUCTORS(Application);

  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function dispatched in event dispatcher and trigger when window close event evoked
    /// - Parameter window_close_event: Window close event instacnce
    bool WindowClose([[maybe_unused]] WindowCloseEvent& window_close_event);
    /// This function begin the ImGui Renderer and render ImGui for all the layers and finally ends the imgui rendering
    void ImGuiRender();

    // Member Variables ----------------------------------------------------------------------------------------------
    bool m_isRunning = true;
    TimeStep m_timeStep;
    Specification m_specificaion;
    Ref<Window> m_window;
    Scope<LayerStack> m_layers;
    Ref<UI::ImGuiLayer> m_imguiLayer;

    inline static Application* s_instance;
  };
  
  /// Defination API for creating application instance.
  /// - Note: Only defination is defined in core. Declaration should be at client side based on the application they
  ///         want to create
  /// - Parameter appData: Applcication Start Data
  Scope<Application> CreateApplication(const std::filesystem::path& startupProject);
} // namespace IKan
