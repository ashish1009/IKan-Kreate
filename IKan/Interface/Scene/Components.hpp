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
  
  struct TransformComponent
  {
    bool enable = true;
    enum Axis : uint8_t
    {
      X, Y, Z
    };
    
    const glm::mat4& Transform() const;
    const glm::vec3& Position() const;
    const glm::vec3& Rotation() const;
    const glm::vec3& Scale() const;
    const glm::quat& Quaternion() const;
    
    void UpdateTransform(const glm::mat4& transform);
    
    void UpdatePosition(Axis axis, float value);
    void UpdateRotation(Axis axis, float value);
    void UpdateScale(Axis axis, float value);
    
    void UpdatePosition(const glm::vec3& value);
    void UpdateRotation(const glm::vec3& value);
    void UpdateScale(const glm::vec3& value);
    
    void AddPosition(Axis axis, float value);
    void AddRotation(Axis axis, float value);
    void AddScale(Axis axis, float value);
    
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
  ComponentGroup<IDComponent, TagComponent, TransformComponent>;

} // namespace IKan
