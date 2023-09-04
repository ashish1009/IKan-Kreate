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
  //  3) Add in ALL_COPY_COMPONENTS Macro
  //  4) Add in Scene Copy Function
  //  5) Update Scene::DuplicateEntity() to deal with the new component in whatever way is appropriate.
  //  6) Update SceneSerializer to (de)serialize the new component.
  //  7) Update SceneHierarchy Panel
  
  struct IDComponent
  {
    UUID ID = 0;
    void Copy(const IDComponent& other);
    IDComponent(const UUID& id = UUID());
    ~IDComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(IDComponent);
  };

  struct TagComponent
  {
    std::string tag;
    
    void Copy(const TagComponent& other);
    operator std::string& ();
    operator const std::string& () const;
    
    TagComponent(const std::string& tag);
    ~TagComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(TagComponent);
  };

  template<typename... Component>
  struct ComponentGroup
  {
    
  };
  
  // Stores all the components present in Engine
  using AllComponents =
  ComponentGroup<IDComponent, TagComponent>;
} // namespace IKan
