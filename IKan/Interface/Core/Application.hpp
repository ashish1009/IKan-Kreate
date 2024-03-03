//
//  Application.hpp
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#pragma once

#include "Core/Layer.hpp"
#include "Core/Window.hpp"
#include "Renderer/Renderer.hpp"
#include "UI/ImGuiLayer.hpp"

namespace IKan
{
  static const std::string IKanVersion = "7.1";
  
  /// This structure stores the specifications to create the application
  struct ApplicationSpecification
  {
    std::string title {"New Application"};
    std::string coreAssetPath {};
    RendererType rendererType {RendererType::Invalid};
    WindowSpecification windowSpecificaion;
    bool startMaximized {false};
    bool resizable {true};
  };

  /// This class is the core application for engine.
  ///   - Initializes the GLFW Window
  ///   - Initializes the Renderers
  ///   - APIs to manage all the layers (Core and client)
  /// - Note: This is the base application class. Client can derive this parent application and override the Methods
  /// - Important: Application is single instance class.
  class Application
  {
  public:
    /// This is the constructure to instantiate the application
    /// - Parameter appSpec: application specification
    Application(const ApplicationSpecification& appSpec);
    /// This is the default destructor of application
    virtual ~Application();

    // Fundamental APIs -----------------------------------------------------------------------------------------------
    /// This function runs the Game loop of the application. This function is responsible for:
    ///   - Updating the Application
    ///   - Updating each Layer frame
    ///   - Updating the window and Swap the buffers
    ///   - Render the GUI by calling 'RenderGui()'
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
    
    // Virtual APIs ---------------------------------------------------------------------------------------------------
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

    // Getters --------------------------------------------------------------------------------------------------------
    /// This function returns the specification of application
    const ApplicationSpecification& GetSpecification() const;
    /// This function returns the time step of frame
    TimeStep GetTimestep() const;
    /// This fuinction returns the native pointer Window native as void
    void* GetWindowPtr() const;
    /// This fuinction returns the iKan Window Instance as reference
    Window& GetWindow();
    /// This fuinction returns the ImGui Layer pointer Reference
    UI::ImGuiLayer& GetImGuiLayer() const;

    // Static APIs ----------------------------------------------------------------------------------------------------
    /// This function returns the reference of application instance.
    static Application& Get();

    /// This function creates the application instance of type T class
    /// - Parameter appSpec: application specificaions
    /// - Note: T should derived from IKan::Application
    template<typename T, typename ...Args>
    [[nodiscard("Application created nerver used")]] static Scope<Application> CreateApplication(Args&& ...args)
    {
      static_assert(std::is_base_of<Application, T>::value, "Class is not Application !!");
      return Scope<T>(new T(std::forward<Args>(args)...));
    }

    DELETE_COPY_MOVE_CONSTRUCTORS(Application);
    
  private:
    class Impl;
    Pimpl<Impl> m_applicationImpl;

    inline static Application* s_instance {nullptr};
  };
} // namespace IKan
