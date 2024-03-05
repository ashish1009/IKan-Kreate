//
//  Node.hpp
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
  
  enum class NodeType
  {
    Blueprint,
    Simple,
    Comment
  };

  struct Node;
  
  struct Pin
  {
    UUID ID;
    Node* node;
    std::string name;
    PinType type;
    PinKind kind;
    StorageKind storage;
    
    choc::value::Value value;
    
    Pin(UUID id, const char* name, PinType type, StorageKind storageKind = StorageKind::Value, choc::value::Value defaultValue = choc::value::Value()) :
    ID(id), node(nullptr), name(name), type(type), kind(PinKind::Input), storage(storageKind), value(defaultValue)
    {
    }
  };
  
  struct Node
  {
    UUID ID;
    std::string Category, name;
    std::vector<Pin> inputs;
    std::vector<Pin> outputs;
    ImColor color;
    NodeType type;
    ImVec2 size;
    
    std::string state;
    std::string savedState;
    
    Node(UUID id, const char* name, ImColor color = ImColor(255, 255, 255)) :
    ID(id), name(name), color(color), type(NodeType::Blueprint), size(0, 0)
    {
    }
  };

  struct Link
  {
    UUID ID;
    
    UUID startPinID;
    UUID endPinID;
    
    ImColor Color;
    
    Link(UUID id, UUID startPinId, UUID endPinId) :
    ID(id), startPinID(startPinId), endPinID(endPinId), color(255, 255, 255)
    {
    }
  };

} // namespace IKan
