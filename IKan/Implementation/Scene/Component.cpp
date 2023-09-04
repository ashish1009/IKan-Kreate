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
} // namespace IKan
