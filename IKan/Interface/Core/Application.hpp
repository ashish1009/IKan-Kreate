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

    DELETE_COPY_MOVE_CONSTRUCTORS(Application);
    
  private:
    inline static Application* s_instance = nullptr;
  };
} // namespace IKan
