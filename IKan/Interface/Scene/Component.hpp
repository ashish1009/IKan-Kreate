//
//  Component.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Core/UUID.hpp"

namespace IKan
{
  struct IDComponent
  {
    UUID ID = 0;
  };

  template<typename... Component>
  struct ComponentGroup
  {
    
  };
  
  // Stores all the components present in Engine
  using AllComponents =
  ComponentGroup<IDComponent>;
} // namespace IKan
