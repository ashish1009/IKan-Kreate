//
//  Application.hpp
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#pragma once

namespace IKan
{
  /// This structure stores the specifications to create the application
  struct ApplicationSpecification
  {
    
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

    // Static APIs ---------------------------------------------------------------------------------------------------
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

    inline static Application* s_instance = nullptr;
  };
} // namespace IKan
