//
//  Nodes.hpp
//  IKan
//
//  Created by Ashish . on 05/03/24.
//

#pragma once

#include "containers/choc_Value.h"

namespace IKan
{
  //! Add your custom pin / value types
  //! Don't forget to add to/from string functions in GraphSerializer.cpp
  enum class PinType
  {
    Flow,
    Bool,
    Int,
    Float,
    Audio,
    String,
    Object,
    Function,
    Delegate
  };
  
  enum class PinKind
  {
    Output,
    Input
  };
  
  enum class StorageKind
  {
    Value = 0,
    Array
  };
  
  struct Node;
  
  struct Pin
  {
    UUID ID;
    Node* Node;
    std::string Name;
    PinType     Type;
    PinKind     Kind;
    StorageKind Storage;
    
    choc::value::Value Value;
    
    Pin(UUID id, const char* name, PinType type, StorageKind storageKind = StorageKind::Value, choc::value::Value defaultValue = choc::value::Value()) :
    ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input), Storage(storageKind), Value(defaultValue)
    {
    }
  };
  
} // namespace IKan
