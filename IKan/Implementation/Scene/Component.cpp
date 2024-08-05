//
//  Component.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "Component.hpp"

namespace IKan
{
#define ComponentLog 0
#if ComponentLog
#define COMP_LOG(...)       IK_LOG_TRACE(LogModule::Component, __VA_ARGS__);
#define COMP_COPY_LOG(...)  IK_LOG_TRACE(LogModule::Component, __VA_ARGS__);
#else
#define COMP_LOG(...)
#define COMP_COPY_LOG(...)
#endif
  
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

  TagComponent::operator std::string& ()
  {
    return tag;
  }
  TagComponent::operator const std::string& () const
  {
    return tag;
  }
  
  // Transform Component --------------------------------------------------------------------------------------------
#define UPDATE_TRANSFORM(param) \
switch (axis) { \
case Utils::Math::Axis::X: param.x = value; break; \
case Utils::Math::Axis::Y: param.y = value; break; \
case Utils::Math::Axis::Z: param.z = value; break; \
default: IK_ASSERT(false) \
} \
transform = Utils::Math::GetTransformMatrix(position, rotation, scale);
  
#define ADD_TRANSFORM(param) \
switch (axis) { \
case Utils::Math::Axis::X: param.x += value; break; \
case Utils::Math::Axis::Y: param.y += value; break; \
case Utils::Math::Axis::Z: param.z += value; break; \
default: IK_ASSERT(false) \
} \
transform = Utils::Math::GetTransformMatrix(position, rotation, scale);
  
  TransformComponent::TransformComponent()
  {
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
  
  void TransformComponent::UpdateTransform(const glm::mat4& transform)
  {
    this->transform = transform;
    Utils::Math::DecomposeTransform(this->transform, position, rotation, scale);
  }
  
  void TransformComponent::UpdatePosition(Utils::Math::Axis axis, float value)
  {
    UPDATE_TRANSFORM(position)
  }
  void TransformComponent::UpdateRotation(Utils::Math::Axis axis, float value)
  {
    UPDATE_TRANSFORM(rotation)
  }
  void TransformComponent::UpdateScale(Utils::Math::Axis axis, float value)
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
  
  void TransformComponent::AddPosition(const glm::vec3& value)
  {
    position += value;
    transform = Utils::Math::GetTransformMatrix(position, rotation, scale);
  }
  void TransformComponent::AddRotation(const glm::vec3& value)
  {
    rotation += value;
    transform = Utils::Math::GetTransformMatrix(position, rotation, scale);
  }
  void TransformComponent::AddScale(const glm::vec3& value)
  {
    scale += value;
    transform = Utils::Math::GetTransformMatrix(position, rotation, scale);
  }
  
  void TransformComponent::AddPosition(Utils::Math::Axis axis, float value)
  {
    ADD_TRANSFORM(position)
  }
  void TransformComponent::AddRotation(Utils::Math::Axis axis, float value)
  {
    ADD_TRANSFORM(rotation)
  }
  void TransformComponent::AddScale(Utils::Math::Axis axis, float value)
  {
    ADD_TRANSFORM(scale)
  }
  
  // Camera Component -------------------------------------------------------------------------------------
  CameraComponent::operator SceneCamera& ()
  {
    return camera;
  }
  CameraComponent::operator const SceneCamera& () const
  {
    return camera;
  }

} // namespace IKan
