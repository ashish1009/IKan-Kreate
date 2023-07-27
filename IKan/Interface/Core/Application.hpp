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
    };

    /// This constructor instantiate the IKan Core application. Responsible to create Window, Renderer Context and
    /// Initialise all the Renderer. Also add the Imgui Layer for GUI Rendering
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
    virtual void OnImguiRender() {}

    // Getters -------------------------------------------------------------------------------------------------------
    /// This function returns the specification of application
    const Specification& GetSpecification() const;
    /// This function returns the time step of frame
    TimeStep GetTimestep() const;
    
    /// This function returns the single instance of application
    static Application& Get();
    
    DELETE_COPY_MOVE_CONSTRUCTORS(Application);

  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function dispatched in event dispatcher and trigger when window close event evoked
    /// - Parameter window_close_event: Window close event instacnce
    bool WindowClose([[maybe_unused]] WindowCloseEvent& window_close_event);
    /// This function begin the Imgui Renderer and render Imgui for all the layers and finally ends the imgui rendering
    void ImguiRender();

    // Member Variables ----------------------------------------------------------------------------------------------
    bool m_isRunning = true;
    TimeStep m_timeStep;
    Specification m_specificaion;
    
    inline static Application* s_instance;
  };
  
  /// Defination API for creating application instance.
  /// - Note: Only defination is defined in core. Declaration should be at client side based on the application they
  ///         want to create
  /// - Parameters:
  ///   - argc: Number of arguments
  ///   - argv: Arguments
  Scope<Application> CreateApplication(int argc, const char** argv);
} // namespace IKan
