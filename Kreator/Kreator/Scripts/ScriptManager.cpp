//
//  ScriptManager.cpp
//  Kreator
//
//  Created by Ashish . on 23/04/24.
//

#include "ScriptManager.hpp"
#include "Scripts/PlayerController.hpp"

namespace Kreator
{
  void ScriptManager::Initialize()
  {
    IKan::ScriptManager::LoadScript("Kreator::PlayerController", ScriptLoader(Kreator::PlayerController));
  }
} // namespace Kreator
