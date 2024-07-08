//
//  Application.hpp
//  IKan
//
//  Created by Ashish . on 06/07/24.
//

#pragma once

#include "Core/TimeStep.hpp"
#include "Events/ApplicationEvents.h"

namespace IKan
{
  /// This structure stores the application specification data
  struct ApplicationSpecification
  {
    
  };
  
  /// This class is the base Application for IKan Engine.
  /// Responsible for
  ///   - Initialize the Window
  ///   - Initialize the Renderers
  ///   - Manage all the layers (Core and client)
  ///   - Handles the events through Window
  ///   - Runs the Game loop
  /// - Note: This is the base application class. Client can derive this application and override the methods
  /// - Important: Application is single instance class.
  class Application
  {
  public:
    /// This constructur instantiates the core base application with application specification data
    /// - Parameter appSpec: application specification data
    Application(const ApplicationSpecification& appSpec);
    /// This is the default virtual destructor of core application
    virtual ~Application();
    
    // Fundamental APIs -----------------------------------------------------------------------------------------------
    /// This function runs the Game loop of the application. This function is responsible for:
    ///   - Updating the Application
    ///   - Updating layers
    ///   - Updating the window and swap the buffers
    ///   - Render the GUI 'RenderGui()'
    void Run();
    /// This function closes the current applciation (IKan::Applciation) and ends the game loop
    void Close();

    // Virtual APIs ---------------------------------------------------------------------------------------------------
    /// This function calls before game loop
    virtual void OnInit() {}
    /// This function calls after game loop
    virtual void OnShutdown() {}
    /// This function calls inside Game Loop Running
    /// - Parameter ts: Time step for each frame
    virtual void OnUpdate(TimeStep ts) {}
    /// This function calls when an event triggers
    /// - Parameter event: Triggered Events
    virtual void OnEvent(Event& event) {}
    /// This function calls inside Game Loop under ImGui Layer
    virtual void OnImGuiRender() {}

    // Getters --------------------------------------------------------------------------------------------------------
    /// This function returns the specification of application
    const ApplicationSpecification& GetSpecification() const;
    /// This function returns if application is running
    bool IsRunning() const;
    /// This function returns if application window is minimized
    bool IsMinimized() const;
    /// This function returns the time step of frame
    TimeStep GetTimestep() const;

    // Static APIs ----------------------------------------------------------------------------------------------------
    /// This function returns the IKan version
    static const std::string& GetVersion();
    
    /// This function returns the single application instance
    static Application& Get();

    /// This function creates the application instance of type T class
    /// - Parameter args: arguments to be passed in application class
    /// - Returns: unique pointer of Application <T>
    template<typename T, typename ...Args> [[nodiscard("Application created nerver used")]]
    static Scope<Application> CreateApplication(Args&& ...args)
    {
      static_assert(std::is_base_of<Application, T>::value, "Class is not Application !!");
      return Scope<T>(new T(std::forward<Args>(args)...));
    }

    DELETE_COPY_MOVE_CONSTRUCTORS(Application);

  private:
    // Member Functions -----------------------------------------------------------------------------------------------
    /// This function flushes the pending task before game loop starts
    void FlushBeforeGameLoop();
    /// This function flushes the pending task after game loop ends
    void FlushAfterGameLoop();

    // Member Variables -----------------------------------------------------------------------------------------------
    bool m_isRunning {false};
    bool m_minimized {false};
    ApplicationSpecification m_specification;
    TimeStep m_timeStep;

    // Single Instance
    inline static Application* s_instance {nullptr};
  };
} // namespace IKan
