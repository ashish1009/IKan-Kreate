//
//  Components.hpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include "Core/UUID.hpp"
#include "Utils/MathUtils.hpp"
#include "Asset/Asset.hpp"
#include "Renderer/MaterialAsset.hpp"
#include "Camera/SceneCamera.hpp"
#include "Camera/CameraController.hpp"
#include "Scene/Entity.hpp"

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
  
  struct CameraComponent
  {
    bool enable = true;
    SceneCamera camera;
    CameraController controller;

    bool primary = true;
    operator SceneCamera& ();
    operator const SceneCamera& () const;
  };
  
  struct MeshComponent
  {
    bool enable {true};
    AssetHandle mesh;
    
    float tilingFactor = 1.0f;
    Ref<MaterialTable> materialTable {CreateRef<MaterialTable>()};
    
    MeshComponent(AssetHandle m = AssetHandle()) : mesh(m) {}
  };
  
  struct RigidBodyComponent
  {
    bool enable = true;
    enum class BodyType { Static, Kinametic, Dynamic };
    BodyType bodyType = BodyType::Static;
    
    bool allowSleep = true;
    bool enableGravity = true;
    float linearDamping = 0.0f;
    float angularDamping = 0.0f;
    glm::vec3 angularAxisMove = glm::vec3(1.0f);
    
    // Storage : No Need to serialze. Need to decide later to copy or not
    void* runtimeBody = nullptr;
    
    static reactphysics3d::BodyType ReactPhysicsBodyType(BodyType type);
  };
  
  struct CommonColliderData
  {
    bool enable = true;
    glm::vec3 positionOffset;
    glm::quat quaternionOffset = {0, 0, 0, 1};
    float frictionCoefficient = 0.2;
    float bounciness = 0.1;
    float massDensity = 1.0;
    
    // User data pointer
    void* userData;
    
    virtual ~CommonColliderData();
  };
  
  struct Box3DColliderComponent : CommonColliderData
  {
    glm::vec3 size = glm::vec3(0.5f);
  };
  
  struct SphereColliderComponent : CommonColliderData
  {
    float radius = 0.5f;
  };
  
  struct CapsuleColliderComponent : CommonColliderData
  {
    float radius = 0.5f;
    float height = 1.0f;
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
    bool enable = true;
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
  };
  
  struct PrefabComponent
  {
    UUID PrefabID = 0;
    UUID EntityID = 0;
  };

  template<typename... Component>
  struct ComponentGroup
  {
    
  };
  
  // Stores all the components present in Engine
  using AllComponents =
  ComponentGroup<IDComponent, VisibilityComponent, TagComponent, TransformComponent, CameraComponent, MeshComponent,
  RigidBodyComponent, Box3DColliderComponent, SphereColliderComponent, CapsuleColliderComponent, JointComponent,
  PrefabComponent>;

} // namespace IKan
