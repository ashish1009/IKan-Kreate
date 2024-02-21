//
//  Components.hpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#pragma once

#include "Core/UUID.hpp"

namespace IKan
{
  struct IDComponent
  {
    UUID ID = 0;
  };
  
  struct TagComponent
  {
    std::string tag;
    operator std::string& ();
    operator const std::string& () const;
  };
  
  struct RelationshipComponent
  {
    UUID parentHandle = 0;
    std::vector<UUID> children;
  };
  
  template<typename... Component>
  struct ComponentGroup
  {
    
  };
  
  // Stores all the components present in Engine
  using AllComponents =
  ComponentGroup<IDComponent, TagComponent>;

} // namespace IKan
