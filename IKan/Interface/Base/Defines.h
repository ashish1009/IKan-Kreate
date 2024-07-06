//
//  Defines.h
//  IKan
//
//  Created by Ashish . on 06/07/24.
//

#pragma once

namespace IKan
{
  // Configurations --------------------------------------------------------------------------------------------------
#ifdef DEBUG
#define IK_ENABLE_LOG
#define IK_DEBUG
#endif

  // Enum Creation Macro ---------------------------------------------------------------------------------------------
  /// Helper MACRO for Enum creation
#define VAL(name) name ,
#define STRING(name) #name,
  
  /// This MACRO Creates enum. It also store all the element as string in an array e.g. if Name is passed as "Test" then
  /// enum would be "enum class Test" and the Array that stores the enum element as string is "TestString[]", just pass
  /// the enum element as index in array,
  /// e.g
  ///   #define Module(f) \
  ///   f(Elem1) f(Elem2) \
  ///   f(Elem3)
  ///
  ///   CreateEnum(Module);
  ///
  ///   To get the string name
  ///   std::string  name = ModuleString[static_cast<size_t>(Elem1)];
#define CreateEnum(Name) \
enum class Name { Name(VAL) }; \
static const char* Name##String[] = { Name(STRING) }; \

} // namespace IKan
