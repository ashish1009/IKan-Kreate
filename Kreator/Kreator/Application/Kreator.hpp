//
//  Kreator.hpp
//  Kreator
//
//  Created by Ashish . on 07/02/24.
//

#pragma once

using namespace IKan;

namespace Kreator
{
  class KreatorApp : public Application
  {
  public:
    KreatorApp(const ApplicationSpecification& appSpec);
    ~KreatorApp();
    
    void OnInit() override;
    void OnShutdown() override;
    
  private:
    Ref<Layer> m_kreatorLayer;
  };
} // namespace Kreator
