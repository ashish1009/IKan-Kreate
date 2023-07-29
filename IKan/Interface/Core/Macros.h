//
//  Macros.h
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

// This MACRO Shifts the LSB bit of number 1 at position x
#define BIT(x) (1 << x)

// Enum Creation Macro -----------------------------------------------------------------------------------------------
/// Helper MACRO for Enum creation
#define VAL(name) name ,
#define STRING(name) #name,

// This MACRO Cast uint32_t into void pointer
#define INT2VOIDP(i)    (void*)(uintptr_t)(i)

/// This MACRO Creates enum also store all the element as string in an array
/// e.g. if Name is passed as "Test" then enum would be "enum class Test" and
/// the Array that stores the enum element as string is "TestString[]", just
/// pass the enum element as index in array,
#define CreateEnum(Name) \
enum class Name { Name(VAL) }; \
static const char* Name##String[] = { Name(STRING) }; \

// Constructors and Assignment Operators Defination ------------------------------------------------------------------
/// This MACRO Deletes the Constructors (Default, Copy and Move ) and Operator = (Copy and Move) for any class to make
/// pure static class
#define MAKE_PURE_STATIC(x) \
x() = delete; \
x(const x&) = delete; \
x(x&&) = delete; \
x& operator =(const x&) = delete; \
x& operator =(x&&) = delete; \

/// This MACRO deletes the Constructors (Copy and Move ) and Operator = (Copy and Move) for any class x to make a
/// class singleton.
#define DELETE_COPY_MOVE_CONSTRUCTORS(x) \
x(const x&) = delete; \
x(x&&) = delete; \
x& operator=(const x&) = delete; \
x& operator =(x&&) = delete; \

/// This MACRO Defines the Constructors (Copy and Move ) and Operator = (Copy and Move) for any class x
#define DEFINE_COPY_MOVE_CONSTRUCTORS(x) \
x(const x&); \
x(x&&); \
x& operator=(const x&); \
x& operator =(x&&); \
