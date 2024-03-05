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
    std::string category, name;
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
    
    ImColor color;
    
    Link(UUID id, UUID startPinId, UUID endPinId) :
    ID(id), startPinID(startPinId), endPinID(endPinId), color(255, 255, 255)
    {
    }
  };
  
  namespace Nodes
  {
    using Registry = std::map<std::string, std::map<std::string, std::function<Node* ()>>>;
    
    class AbstractFactory
    {
    public:
      virtual ~AbstractFactory() = default;
      
      /// This funcion spawns the node
      /// - Parameters:
      ///   - categiory: node category
      ///   - type: type of node
      virtual Node* SpawnNode(const std::string& category, const std::string& type) = 0;
    };
    
    template<class T>
    class Factory : public AbstractFactory
    {
    public:
      static const Registry s_registry;
      static int a;

      /// This function Spawns the node
      /// - Parameters:
      ///   - categiory: node category
      ///   - type: type of node
      template<class ST> [[nodiscard]] static Node* SpawnNodeStatic(const std::string& category, const std::string& type)
      {
        return ST::SpawnNodeStatic();
      };
    };
    
    //! Create your own Factory subclass if you need a different selection of nodes for your custom graph
    //! Create and register custom node spawn functions in .cpp file
    class DemoFactory : public Factory<DemoFactory>
    {
    public:
      /// This function Spawns the node
      /// - Parameters:
      ///   - categiory: node category
      ///   - type: type of node
      [[nodiscard]] static Node* SpawnNodeStatic(const std::string& category, const std::string& type);
      /// @see AbstractFactory
      [[nodiscard]] Node* SpawnNode(const std::string& category, const std::string& type) override { return SpawnNodeStatic(category, type); }
    };
  } // namespace Nodes

} // namespace IKan
