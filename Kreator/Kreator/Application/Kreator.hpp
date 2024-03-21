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
  struct KreatorDirectories
  {
    std::filesystem::path systemUserPath{};
    std::filesystem::path iKanKreatePath{};
    std::filesystem::path clientResourcePath{};
  };
  class KreatorApp : public Application
  {
  public:
    KreatorApp(const ApplicationSpecification& appSpec, const KreatorDirectories& kreatorDirectories);
    ~KreatorApp();
    
    void OnInit() override;
    void OnShutdown() override;
    void OnUpdate(TimeStep ts) override;
    void OnImGuiRender() override;
    
  private:
    Ref<Layer> m_kreatorLayer;
    KreatorDirectories m_kreatorDirectories;
  };
} // namespace Kreator
