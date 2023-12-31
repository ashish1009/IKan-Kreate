//
//  Component.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "Camera/SceneCamera.hpp"

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
  
  struct QuadComponent : public SpriteRendererComponent
  {
    QuadComponent();
    ~QuadComponent();
    
    void Copy(const QuadComponent& other);
    DEFINE_COPY_MOVE_CONSTRUCTORS(QuadComponent);
  };
  
  struct CircleComponent : public SpriteRendererComponent
  {
    float thickness = 1.0f;
    float fade = 0.005f;
    
    CircleComponent();
    ~CircleComponent();
    
    void Copy(const CircleComponent& other);
    DEFINE_COPY_MOVE_CONSTRUCTORS(CircleComponent);
  };
  
  struct TextComponent
  {
    std::string textString = "Text";
    AssetHandle assetHandle;
    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    TextComponent();
    ~TextComponent();
    
    void Copy(const TextComponent& other);
    DEFINE_COPY_MOVE_CONSTRUCTORS(TextComponent);
  };
  
  struct StaticMeshComponent
  {
    AssetHandle staticMesh;
    
    StaticMeshComponent();
    StaticMeshComponent(AssetHandle staticMesh);
    ~StaticMeshComponent();
    
    void Copy(const StaticMeshComponent& other);
    DEFINE_COPY_MOVE_CONSTRUCTORS(StaticMeshComponent);
  };
  
  struct RigidBodyComponent
  {
    enum class BodyType { Static, Kinametic, Dynamic };
    BodyType bodyType = BodyType::Static;
    
    bool allowSleep = true;
    float liniarDamping = 0.0f;
    float angularDamping = 0.0f;
    
    // Storage : No Need to serialze. Need to decide later to copy or not
    void* runtimeBody = nullptr;
    
    static reactphysics3d::BodyType ReactPhysicsBodyType(BodyType type);
    
    RigidBodyComponent();
    ~RigidBodyComponent();
    
    void Copy(const RigidBodyComponent& other);
    DEFINE_COPY_MOVE_CONSTRUCTORS(RigidBodyComponent);
  };
  
  struct CommonColliderData
  {
    glm::vec3 positionOffset;
    glm::quat quaternionOffset;
    float frictionCoefficient = 0.2;
    float bounciness = 0.1;
    float massDensity;
  };
  
  struct Box3DColliderComponent : CommonColliderData
  {
    glm::vec3 size = glm::vec3(0.5f);
    
    Box3DColliderComponent();
    ~Box3DColliderComponent();
    void Copy(const Box3DColliderComponent& other);
    DEFINE_COPY_MOVE_CONSTRUCTORS(Box3DColliderComponent);
  };
  
  struct SphereColliderComponent : CommonColliderData
  {
    float radius = 0.5f;
    
    SphereColliderComponent();
    ~SphereColliderComponent();
    void Copy(const SphereColliderComponent& other);
    DEFINE_COPY_MOVE_CONSTRUCTORS(SphereColliderComponent);
  };
  
  struct CapsuleColliderComponent : CommonColliderData
  {
    float radius = 0.5f;
    float height = 1.0f;
    
    CapsuleColliderComponent();
    ~CapsuleColliderComponent();
    void Copy(const CapsuleColliderComponent& other);
    DEFINE_COPY_MOVE_CONSTRUCTORS(CapsuleColliderComponent);
  };
  
  struct BallSocketData
  {
    bool coneLimit = false;
    float coneAngle = M_PI / 4;
  };
  
  struct HingeData
  {
    glm::vec3 worldAxis = {0, 0, 0};
    glm::vec3 localAxis1 = {0, 0, 0};
    glm::vec3 localAxis2 = {0, 0, 0};
   
    bool limit = false;
    float initMinAngleLimit = -M_PI / 4, initMaxAngleLimit = M_PI / 4;
    
    bool motor = false;
    float initMotorSpeed = 0, initMaxMotorTorque = 0;
  };

  struct SliderData
  {
    glm::vec3 worldAxis = {0, 0, 0};
    glm::vec3 localAxis1 = {0, 0, 0};
    
    bool limit = false;
    float initMinTransLimit = -2.0f, initMaxTransLimit = 2.0f;
    
    bool motor = false;
    float initMotorSpeed = 0, initMaxMotorForce = 0;
  };

  struct JointComponent
  {
    enum class Type
    {
      Fixed, BallSocket, Hinge, Slider
    };
    
    UUID connectedEntity;
    Type type = Type::Fixed;
    bool isWorldSpace = true;
    bool isCollisionEnabled = true;
    glm::vec3 worldAnchorPoint = {0, 0, 0};
    glm::vec3 localAnchorPoint1 = {0, 0, 0};
    glm::vec3 localAnchorPoint2 = {0, 0, 0};
    
    BallSocketData ballSocketData;
    HingeData hingeData;
    SliderData sliderData;

    JointComponent();
    ~JointComponent();
    void Copy(const JointComponent& other);
    DEFINE_COPY_MOVE_CONSTRUCTORS(JointComponent);
  };
  
  struct CommonLightData
  {
    bool active = true;
    glm::vec3 radiance = glm::vec3(1.0f);
  };
  
  struct PointLightComponent : CommonLightData
  {
    PointLightComponent();
    ~PointLightComponent();
    void Copy(const PointLightComponent& other);
    DEFINE_COPY_MOVE_CONSTRUCTORS(PointLightComponent);
  };

  template<typename... Component>
  struct ComponentGroup
  {
    
  };
  
  // Stores all the components present in Engine
  using AllComponents =
  ComponentGroup<IDComponent, TagComponent, TransformComponent, RelationshipComponent, CameraComponent, SpriteRendererComponent, \
  QuadComponent, CircleComponent, TextComponent, StaticMeshComponent, RigidBodyComponent, Box3DColliderComponent, \
  CapsuleColliderComponent, JointComponent, PointLightComponent>;
} // namespace IKan
