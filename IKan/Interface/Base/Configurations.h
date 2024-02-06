//
//  Macros.hpp
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#pragma once

// Constructors and Assignment Operators Defination ------------------------------------------------------------------
/// This MACRO deletes the Constructors (Copy and Move ) and Operator = (Copy and Move) for any class x to make a
/// class singleton.
/// - Note: This only deletes the copy move constructors. Default constructur still need to be set as private and
///         follow other singleton rules
#define DELETE_COPY_MOVE_CONSTRUCTORS(x) \
x(const x&) = delete; \
x(x&&) = delete; \
x& operator=(const x&) = delete; \
x& operator =(x&&) = delete; \

/// This MACRO deletes all the constructors (default, copy and move ) and operator = (copy and move) for any class x
/// to make pure static class
#define DELETE_ALL_CONSTRUCTORS(x) \
x() = delete; \
x(const x&) = delete; \
x(x&&) = delete; \
x& operator =(const x&) = delete; \
x& operator =(x&&) = delete; \

// Typedefs --------------------------------------------------------------------------------------------------------
template<typename T> using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args> constexpr Scope<T>
CreateScope(Args&& ... args)
{
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T> using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args> constexpr Ref<T>
CreateRef(Args&& ... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}

