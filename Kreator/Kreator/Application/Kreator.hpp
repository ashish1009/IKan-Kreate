//
//  Kreator.hpp
//  Kreator
//
//  Created by Ashish . on 06/07/24.
//

#pragma once

using namespace IKan;

namespace Kreator
{
  /// This class stores the Kreator Applicaiton
  class KreatorApp : public Application
  {
  public:
    /// Kreator Application constructor
    /// - Parameters:
    ///   - appSpec: core application specificaions
    ///   - kreatorDirectories: kreator direcotries
    KreatorApp(const ApplicationSpecification& appSpec);
    /// Kreator Application Destructor
    ~KreatorApp();
    
    /// @see Application
    void OnInit() override;
    /// @see Application
    void OnShutdown() override;
    /// @see Application
    void OnUpdate(TimeStep ts) override;
    /// @see Application
    void OnImGuiRender() override;
    
    /// This function returns the Kreator application version
    static const std::string& GetVersion();

  private:
    Scope<Layer> m_rendererLayer;
  };
} // namespace Kreator
