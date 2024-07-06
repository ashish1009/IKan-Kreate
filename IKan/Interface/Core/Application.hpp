//
//  Application.hpp
//  IKan
//
//  Created by Ashish . on 06/07/24.
//

#pragma once

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

    // Virtual APIs ---------------------------------------------------------------------------------------------------

    // Getters --------------------------------------------------------------------------------------------------------

    // Static APIs ----------------------------------------------------------------------------------------------------
    static Application& Get();

    DELETE_COPY_MOVE_CONSTRUCTORS(Application);

  private:
    // Member Functions -----------------------------------------------------------------------------------------------

    // Member Variables -----------------------------------------------------------------------------------------------
    inline static Application* s_instance;
  };
} // namespace IKan
