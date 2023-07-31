//
//  Component.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Camera/SceneCamera.hpp"

namespace IKan
{
  // ADDING A NEW COMPONENT
  // ----------------------
  // If you add a new type of component, there are several pieces of code that need updating:
  //  1) Add new component here (obviously).
  //  2) Create Proper Copy and Move Constructors
  //  3) Add in ALL_COPY_COMPONENTS Macro
  //  3) Add in Scene Duplicate Function
  
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
  
  struct RelationshipComponent
  {
    UUID parentHandle = 0;
    std::vector<UUID> children;
    
    void Copy(const RelationshipComponent& other);
    
    RelationshipComponent();
    RelationshipComponent(UUID parent);
    ~RelationshipComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(RelationshipComponent);
  };
  
  struct TransformComponent
  {
    enum Axis : uint8_t
    {
      X, Y, Z
    };
    
    const glm::mat4& Transform() const;
    const glm::vec3& Position() const;
    const glm::vec3& Rotation() const;
    const glm::vec3& Scale() const;
    const glm::quat& Quaternion() const;
    
    void UpdatePosition(Axis axis, float value);
    void UpdateRotation(Axis axis, float value);
    void UpdateScale(Axis axis, float value);
    
    void UpdatePosition(const glm::vec3& value);
    void UpdateRotation(const glm::vec3& value);
    void UpdateScale(const glm::vec3& value);
    
    void AddPosition(Axis axis, float value);
    void AddRotation(Axis axis, float value);
    void AddScale(Axis axis, float value);
    
    void Copy(const TransformComponent& other);
    
    TransformComponent();
    ~TransformComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(TransformComponent);
    
  private:
    glm::quat quaternion;
    glm::mat4 transform;
    glm::vec3 position{0.0f}, rotation{0.0f}, scale{1.0f};
  };
  
  struct CameraComponent
  {
    SceneCamera camera;
    bool primary = true;
    
    CameraComponent();
    ~CameraComponent();
    
    void Copy(const CameraComponent& other);
    
    operator SceneCamera& ();
    operator const SceneCamera& () const;
    
    DEFINE_COPY_MOVE_CONSTRUCTORS(CameraComponent);
  };

  struct SpriteRendererComponent
  {
    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    AssetHandle texture = AssetHandle(0);
    float tilingFactor = 1.0f;
    
    SpriteRendererComponent();
    ~SpriteRendererComponent();
    
    void Copy(const SpriteRendererComponent& other);
    DEFINE_COPY_MOVE_CONSTRUCTORS(SpriteRendererComponent);
  };

  template<typename... Component>
  struct ComponentGroup
  {
    
  };
  
#define ALL_COPY_COMPONENTS TransformComponent, RelationshipComponent, CameraComponent, SpriteRendererComponent
  
  // Stores all the components present in Engine
  using AllComponents =
  ComponentGroup<IDComponent, TagComponent, ALL_COPY_COMPONENTS>;

  // Stores the components that need to be copied
  using AllCopyComponents =
  ComponentGroup<ALL_COPY_COMPONENTS>;
} // namespace IKan
