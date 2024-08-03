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
  // ADDING A NEW COMPONENT
  // ----------------------
  // If you add a new type of component, there are several pieces of code that need updating:
  //  1) Add new component here (obviously).
  //  2) Create Proper Copy and Move Constructors if needed as scene might copy the entities
  //  3) Add in AllComponents Macro
  //  4) Add in Scene CopyTo Function
  //  5) Update Scene::DuplicateEntity() to deal with the new component in whatever way is appropriate.
  //  6) Update SceneSerializer to (de)serialize the new component.
  //  7) Update SceneHierarchy Panel
  //  8) Update Prefab serializer

  // Menmdatory components -------------------------------------------------------------------------------------------
  struct IDComponent
  {
    UUID ID = 0;
  };
  
  struct VisibilityComponent
  {
    bool isVisible = true;
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
  
  struct TransformComponent
  {
    const glm::mat4& Transform() const;
    const glm::vec3& Position() const;
    const glm::vec3& Rotation() const;
    const glm::vec3& Scale() const;
    const glm::quat& Quaternion() const;
    
    void UpdateTransform(const glm::mat4& transform);
    
    void UpdatePosition(Utils::Math::Axis axis, float value);
    void UpdateRotation(Utils::Math::Axis axis, float value);
    void UpdateScale(Utils::Math::Axis axis, float value);
    
    void UpdatePosition(const glm::vec3& value);
    void UpdateRotation(const glm::vec3& value);
    void UpdateScale(const glm::vec3& value);
    
    void AddPosition(Utils::Math::Axis axis, float value);
    void AddRotation(Utils::Math::Axis axis, float value);
    void AddScale(Utils::Math::Axis axis, float value);
    
    void AddPosition(const glm::vec3& value);
    void AddRotation(const glm::vec3& value);
    void AddScale(const glm::vec3& value);
    
    TransformComponent();
    
  private:
    glm::quat quaternion;
    glm::mat4 transform;
    glm::vec3 position{0.0f}, rotation{0.0f}, scale{1.0f};
  };

  template<typename... Component>
  struct ComponentGroup
  {
    
  };
  
  // Stores all the components present in Engine
  using AllComponents =
  ComponentGroup<IDComponent, VisibilityComponent, TagComponent, TransformComponent>;
} // namespace IKan
