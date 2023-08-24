//
//  Component.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Component.hpp"

namespace IKan
{
#define ComponentLog 1
  
#if ComponentLog
#define COMP_LOG(...)       IK_LOG_TRACE(LogModule::Component, __VA_ARGS__);
#define COMP_COPY_LOG(...)  IK_LOG_TRACE(LogModule::Component, __VA_ARGS__);
#else
#define COMP_LOG(...)
#define COMP_COPY_LOG(...)
#endif
  
#define UPDATE_TRANSFORM(param) \
switch (axis) { \
case Axis::X: param.x = value; break; \
case Axis::Y: param.y = value; break; \
case Axis::Z: param.z = value; break; \
default: IK_ASSERT(false) \
} \
transform = Utils::Math::GetTransformMatrix(position, rotation, scale);
  
  
#define ADD_TRANSFORM(param) \
switch (axis) { \
case Axis::X: param.x += value; break; \
case Axis::Y: param.y += value; break; \
case Axis::Z: param.z += value; break; \
default: IK_ASSERT(false) \
} \
transform = Utils::Math::GetTransformMatrix(position, rotation, scale);
  
#define COMP_COPY_MOVE_CONSTRUCTORS(x) \
x::x(const x& other) { \
Copy(other); \
COMP_COPY_LOG("Copying {0}", #x); \
} \
x::x(x&& other) { \
Copy(other); \
COMP_COPY_LOG("Moving {0}", #x); \
} \
x& x::operator=(const x& other) { \
COMP_COPY_LOG("Copying with = operator {0}", #x); \
Copy(other); \
return *this; \
} \
x& x::operator=(x&& other) { \
COMP_COPY_LOG("Moving with = operator {0}", #x); \
Copy(other); \
return *this; \
} \

  // ID Component ---------------------------------------------------------------------------------------------------
  IDComponent::IDComponent(const UUID& id)
  : ID(id)
  {
    COMP_LOG("Creating ID Component");
  }
  IDComponent::~IDComponent()
  {
    COMP_LOG("Destroying ID Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(IDComponent);
  void IDComponent::Copy(const IDComponent &other)
  {
    ID = other.ID;
  }
  
  // Tag Component --------------------------------------------------------------------------------------------------
  TagComponent::TagComponent(const std::string& tag)
  : tag(tag)
  {
    COMP_LOG("Creating Tag Component");
  }
  TagComponent::~TagComponent()
  {
    COMP_LOG("Destroying Tag Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(TagComponent);
  void TagComponent::Copy(const TagComponent &other)
  {
    tag = other.tag;
  }
  TagComponent::operator std::string& ()
  {
    return tag;
  }
  TagComponent::operator const std::string& () const
  {
    return tag;
  }
  
  // Relation Component ----------------------------------------------------------------------------------------------
  RelationshipComponent::RelationshipComponent()
  {
    COMP_LOG("Creating Relationship Component");
  }
  RelationshipComponent::RelationshipComponent(UUID parent)
  : parentHandle(parent)
  {
    COMP_LOG("Creating Relationship Component with parent {}", (uint64_t)parent);
  }
  RelationshipComponent::~RelationshipComponent()
  {
    COMP_LOG("Destroying Relationship Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(RelationshipComponent);
  void RelationshipComponent::Copy(const RelationshipComponent &other)
  {
    children.clear();
    
    parentHandle = other.parentHandle;
    for (const auto& child : children)
    {
      children.push_back(child);
    }
  }
  
  // Transform Component --------------------------------------------------------------------------------------------
  TransformComponent::TransformComponent()
  {
    transform = Utils::Math::GetTransformMatrix(position, rotation, scale);
    quaternion = glm::quat(rotation);
    COMP_LOG("Creating Transform Component");
  }
  TransformComponent::~TransformComponent()
  {
    COMP_LOG("Destroying Transform Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(TransformComponent);
  
  void TransformComponent::Copy(const TransformComponent &other)
  {
    position = other.Position();
    scale = other.Scale();
    rotation = other.Rotation();
    transform = Utils::Math::GetTransformMatrix(position, rotation, scale);
    quaternion = glm::quat(rotation);
  }
  const glm::mat4& TransformComponent::Transform() const
  {
    return transform;
  }
  const glm::vec3& TransformComponent::Position() const
  {
    return position;
  }
  const glm::vec3& TransformComponent::Rotation() const
  {
    return rotation;
  }
  const glm::vec3& TransformComponent::Scale() const
  {
    return scale;
  }
  const glm::quat& TransformComponent::Quaternion() const
  {
    return quaternion;
  }
  
  void TransformComponent::UpdatePosition(TransformComponent::Axis axis, float value)
  {
    UPDATE_TRANSFORM(position)
  }
  void TransformComponent::UpdateRotation(TransformComponent::Axis axis, float value)
  {
    UPDATE_TRANSFORM(rotation)
  }
  void TransformComponent::UpdateScale(TransformComponent::Axis axis, float value)
  {
    UPDATE_TRANSFORM(scale)
  }
  
  void TransformComponent::UpdatePosition(const glm::vec3& value)
  {
    position = value;
    transform = Utils::Math::GetTransformMatrix(position, rotation, scale);
  }
  void TransformComponent::UpdateRotation(const glm::vec3& value)
  {
    rotation = value;
    transform = Utils::Math::GetTransformMatrix(position, rotation, scale);
  }
  void TransformComponent::UpdateScale(const glm::vec3& value)
  {
    scale = value;
    transform = Utils::Math::GetTransformMatrix(position, rotation, scale);
  }
  
  void TransformComponent::AddPosition(Axis axis, float value)
  {
    ADD_TRANSFORM(position)
  }
  void TransformComponent::AddRotation(Axis axis, float value)
  {
    ADD_TRANSFORM(rotation)
  }
  void TransformComponent::AddScale(Axis axis, float value)
  {
    ADD_TRANSFORM(scale)
  }
  
  // Camera Component -----------------------------------------------------------------------------------------------
  CameraComponent::CameraComponent()
  {
    COMP_LOG("Creating Camera Component");
  }
  CameraComponent::~CameraComponent()
  {
    COMP_LOG("Destroying Camera Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(CameraComponent);
  
  void CameraComponent::Copy(const CameraComponent &other)
  {
    primary = other.primary;
    camera = other.camera;
  }
  CameraComponent::operator SceneCamera& ()
  {
    return camera;
  }
  CameraComponent::operator const SceneCamera& () const
  {
    return camera;
  }
  
  // Sprite Renderer Component --------------------------------------------------------------------------------------
  SpriteRendererComponent::SpriteRendererComponent()
  {
    COMP_LOG("Creating Sprite Renderer Component");
  }
  SpriteRendererComponent::~SpriteRendererComponent()
  {
    COMP_LOG("Destroying Sprite Renderer Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(SpriteRendererComponent);
  
  void SpriteRendererComponent::Copy(const SpriteRendererComponent &other)
  {
    texture = other.texture;
    color = other.color;
    tilingFactor = other.tilingFactor;
  }
  
  // Quad Component -------------------------------------------------------------------------------------------------
  QuadComponent::QuadComponent()
  {
    COMP_LOG("Creating Quad Component");
  }
  QuadComponent::~QuadComponent()
  {
    COMP_LOG("Destroying Quad Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(QuadComponent);
  
  void QuadComponent::Copy(const QuadComponent &other)
  {
    texture = other.texture;
    color = other.color;
    tilingFactor = other.tilingFactor;
  }
  
  // Circle Component -------------------------------------------------------------------------------------------------
  CircleComponent::CircleComponent()
  {
    COMP_LOG("Creating Circle Component");
  }
  CircleComponent::~CircleComponent()
  {
    COMP_LOG("Destroying Circle Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(CircleComponent);
  
  void CircleComponent::Copy(const CircleComponent &other)
  {
    texture = other.texture;
    color = other.color;
    tilingFactor = other.tilingFactor;
    
    thickness = other.thickness;
    fade = other.fade;
  }
  
  // Text Component ---------------------------------------------------------------------------------------------------
  TextComponent::TextComponent()
  {
    COMP_LOG("Creating Text Component");
  }
  TextComponent::~TextComponent()
  {
    COMP_LOG("Destroying Text Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(TextComponent);
  
  void TextComponent::Copy(const TextComponent &other)
  {
    textString = other.textString;
    assetHandle = other.assetHandle;
    color = other.color;
  }
  
  // Static Mesh Component -------------------------------------------------------------------------------------------
  StaticMeshComponent::StaticMeshComponent()
  {
    COMP_LOG("Creating Static Mesh Component");
  }
  
  StaticMeshComponent::StaticMeshComponent(AssetHandle staticMesh)
  : staticMesh(staticMesh)
  {
    COMP_LOG("Creating Static Mesh Component from mesh handle");
  }
  
  StaticMeshComponent::~StaticMeshComponent()
  {
    COMP_LOG("Destroying Static Mesh Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(StaticMeshComponent);
  
  void StaticMeshComponent::Copy(const StaticMeshComponent &other)
  {
    staticMesh = other.staticMesh;
  }
  
  // Rigid Body Component --------------------------------------------------------------------------------------------
  RigidBodyComponent::RigidBodyComponent()
  {
    COMP_LOG("Creating Rigid Body Component");
  }
  
  RigidBodyComponent::~RigidBodyComponent()
  {
    COMP_LOG("Destroying Rigid Body Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(RigidBodyComponent);
  
  void RigidBodyComponent::Copy(const RigidBodyComponent &other)
  {
    bodyType = other.bodyType;
  }
  
  reactphysics3d::BodyType RigidBodyComponent::ReactPhysicsBodyType(BodyType type)
  {
    switch (type)
    {
      case BodyType::Static: return reactphysics3d::BodyType::STATIC;
      case BodyType::Kinametic: return reactphysics3d::BodyType::KINEMATIC;
      case BodyType::Dynamic: return reactphysics3d::BodyType::DYNAMIC;
      default: IK_ASSERT(false);
    }
  }
  
  // Box 3D Colldier Component -------------------------------------------------------------------------------------
  Box3DColliderComponent::Box3DColliderComponent()
  {
    COMP_LOG("Creating Box 3D Component");
  }
  
  Box3DColliderComponent::~Box3DColliderComponent()
  {
    COMP_LOG("Destroying Box 3D Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(Box3DColliderComponent);
  
  void Box3DColliderComponent::Copy(const Box3DColliderComponent& other)
  {
    size = other.size;
    frictionCoefficient = other.frictionCoefficient;
    bounciness = other.bounciness;
    massDencity = other.massDencity;
    
    positionOffset = other.positionOffset;
    quaternionOffset = other.quaternionOffset;
  }
  
  // Sphere Colldier Component ---------------------------------------------------------------------------------------
  SphereColliderComponent::SphereColliderComponent()
  {
    COMP_LOG("Creating Sphere Component");
  }
  
  SphereColliderComponent::~SphereColliderComponent()
  {
    COMP_LOG("Destroying Sphere Component");
  }
  COMP_COPY_MOVE_CONSTRUCTORS(SphereColliderComponent);
  
  void SphereColliderComponent::Copy(const SphereColliderComponent& other)
  {
    radius = other.radius;
    frictionCoefficient = other.frictionCoefficient;
    bounciness = other.bounciness;
    massDencity = other.massDencity;
    
    positionOffset = other.positionOffset;
    quaternionOffset = other.quaternionOffset;
  }
} // namespace IKan
