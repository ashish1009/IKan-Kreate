//
//  ImGuiScoped.cpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#include "ImGuiScoped.hpp"
#include "UI/ImGuiUtils.hpp"

namespace IKan::UI
{
  ScopedDisable::ScopedDisable(bool disabled)
  {
    UI::BeginDisabled(disabled);
  }
  ScopedDisable::~ScopedDisable()
  {
    UI::EndDisabled();
  }
} // namespace IKan::UI
