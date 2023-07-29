//
//  Component.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace IKan
{
  // ADDING A NEW COMPONENT
  // ----------------------
  // If you add a new type of component, there are several pieces of code that need updating:
  //  1) Add new component here (obviously).
  //  2) Create Proper Copy and Move Constructors
  //  3) Adding in ALL_COPY_COMPONENTS Macro
  
  template<typename... Component>
  struct ComponentGroup
  {
    
  };
  
#define ALL_COPY_COMPONENTS
  
  // Stores all the components present in Engine
  using AllComponents =
  ComponentGroup<ALL_COPY_COMPONENTS>;
  
  // Stores the components that need to be copied
  using AllCopyComponents =
  ComponentGroup<ALL_COPY_COMPONENTS>;
} // namespace IKan
