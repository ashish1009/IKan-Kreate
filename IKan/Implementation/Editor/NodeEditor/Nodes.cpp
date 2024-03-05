//
//  Nodes.cpp
//  IKan
//
//  Created by Ashish . on 05/03/24.
//

#include "text/choc_StringUtilities.h"

#include "Nodes.hpp"

#define NODE_CATEGORY(name) std::string { #name }
#define DECLARE_NODE(category, name) {#name, category::name}

namespace IKan
{
  //==========================================================================
  /// Demo Nodes
  class DemoNodes
  {
  public:
    static Node* Some_Float()
    {
      const std::string nodeName = choc::text::replace(__func__, "_", " ");
      
      auto* node = new Node(0, nodeName.c_str());
      node->category = "DemoNodes";
      
      node->type = NodeType::Blueprint;
      node->size = ImVec2(300, 200);
      
      node->inputs.emplace_back(0, "In", PinType::Float);
      node->inputs.emplace_back(0, "In", PinType::Float);
      node->outputs.emplace_back(0, "Result", PinType::Float);
      
      return node;
    }
    
    static Node* All_Pin_Types()
    {
      const std::string nodeName = choc::text::replace(__func__, "_", " ");
      
      auto* node = new Node(0, nodeName.c_str());
      node->category = "DemoNodes";
      
      node->type = NodeType::Blueprint;
      node->size = ImVec2(300, 200);
      
      node->inputs.emplace_back(0, "Flow In",    PinType::Flow);
      node->inputs.emplace_back(0, "Function In", PinType::Function);
      node->inputs.emplace_back(0, "Float In",  PinType::Float);
      node->inputs.emplace_back(0, "Int In",    PinType::Int);
      node->inputs.emplace_back(0, "Bool In",    PinType::Bool);
      node->inputs.emplace_back(0, "String In",  PinType::String);
      node->inputs.emplace_back(0, "Object In",  PinType::Object);
      
      node->outputs.emplace_back(0, "Flow Out",    PinType::Flow);
      node->outputs.emplace_back(0, "Function Out",  PinType::Function);
      node->outputs.emplace_back(0, "Float Out",    PinType::Float);
      node->outputs.emplace_back(0, "Int Out",    PinType::Int);
      node->outputs.emplace_back(0, "Bool Out",    PinType::Bool);
      node->outputs.emplace_back(0, "String Out",    PinType::String);
      node->outputs.emplace_back(0, "Object Out",    PinType::Object);
      
      return node;
    }
    
    static Node* All_Pin_Type_Arrays()
    {
      const std::string nodeName = choc::text::replace(__func__, "_", " ");
      
      auto* node = new Node(0, nodeName.c_str());
      node->category = "DemoNodes";
      
      node->type = NodeType::Blueprint;
      node->size = ImVec2(300, 200);
      
      node->inputs.emplace_back(0, "Flow In",    PinType::Flow, StorageKind::Array);
      node->inputs.emplace_back(0, "Function In", PinType::Function, StorageKind::Array);
      node->inputs.emplace_back(0, "Float In",  PinType::Float, StorageKind::Array);
      node->inputs.emplace_back(0, "Int In",    PinType::Int, StorageKind::Array);
      node->inputs.emplace_back(0, "Bool In",    PinType::Bool, StorageKind::Array);
      node->inputs.emplace_back(0, "String In",  PinType::String, StorageKind::Array);
      node->inputs.emplace_back(0, "Object In",  PinType::Object, StorageKind::Array);
      
      node->outputs.emplace_back(0, "Flow Out",    PinType::Flow, StorageKind::Array);
      node->outputs.emplace_back(0, "Function Out",  PinType::Function, StorageKind::Array);
      node->outputs.emplace_back(0, "Float Out",    PinType::Float, StorageKind::Array);
      node->outputs.emplace_back(0, "Int Out",    PinType::Int, StorageKind::Array);
      node->outputs.emplace_back(0, "Bool Out",    PinType::Bool, StorageKind::Array);
      node->outputs.emplace_back(0, "String Out",    PinType::String, StorageKind::Array);
      node->outputs.emplace_back(0, "Object Out",    PinType::Object, StorageKind::Array);
      
      return node;
    }
    
    static Node* Simple_All_Pin_Types()
    {
      const std::string nodeName = choc::text::replace(__func__, "_", " ");
      
      auto* node = new Node(0, nodeName.c_str());
      node->category = "DemoNodes";
      
      node->type = NodeType::Simple;
      node->size = ImVec2(300, 200);
      
      node->inputs.emplace_back(0, "Flow In",    PinType::Flow);
      node->inputs.emplace_back(0, "Function In", PinType::Function);
      node->inputs.emplace_back(0, "Float In",  PinType::Float);
      node->inputs.emplace_back(0, "Int In",    PinType::Int);
      node->inputs.emplace_back(0, "Bool In",    PinType::Bool);
      node->inputs.emplace_back(0, "String In",  PinType::String);
      node->inputs.emplace_back(0, "Object In",  PinType::Object);
      
      node->outputs.emplace_back(0, "Flow Out",    PinType::Flow);
      node->outputs.emplace_back(0, "Function Out",  PinType::Function);
      node->outputs.emplace_back(0, "Float Out",    PinType::Float);
      node->outputs.emplace_back(0, "Int Out",    PinType::Int);
      node->outputs.emplace_back(0, "Bool Out",    PinType::Bool);
      node->outputs.emplace_back(0, "String Out",    PinType::String);
      node->outputs.emplace_back(0, "Object Out",    PinType::Object);
      
      return node;
    }
    
    static Node* Simple_All_Pin_Type_Arrays()
    {
      const std::string nodeName = choc::text::replace(__func__, "_", " ");
      
      auto* node = new Node(0, nodeName.c_str());
      node->category = "DemoNodes";
      
      node->type = NodeType::Simple;
      node->size = ImVec2(300, 200);
      
      node->inputs.emplace_back(0, "Flow In", PinType::Flow, StorageKind::Array);
      node->inputs.emplace_back(0, "Function In", PinType::Function, StorageKind::Array);
      node->inputs.emplace_back(0, "Float In", PinType::Float, StorageKind::Array);
      node->inputs.emplace_back(0, "Int In", PinType::Int, StorageKind::Array);
      node->inputs.emplace_back(0, "Bool In", PinType::Bool, StorageKind::Array);
      node->inputs.emplace_back(0, "String In", PinType::String, StorageKind::Array);
      node->inputs.emplace_back(0, "Object In", PinType::Object, StorageKind::Array);
      
      node->outputs.emplace_back(0, "Flow Out", PinType::Flow, StorageKind::Array);
      node->outputs.emplace_back(0, "Function Out", PinType::Function, StorageKind::Array);
      node->outputs.emplace_back(0, "Float Out", PinType::Float, StorageKind::Array);
      node->outputs.emplace_back(0, "Int Out", PinType::Int, StorageKind::Array);
      node->outputs.emplace_back(0, "Bool Out", PinType::Bool, StorageKind::Array);
      node->outputs.emplace_back(0, "String Out", PinType::String, StorageKind::Array);
      node->outputs.emplace_back(0, "Object Out", PinType::Object, StorageKind::Array);
      
      return node;
    }
  };

  //==========================================================================
  /// Utility Nodes
  class Utility
  {
  public:
    static Node* Comment()
    {
      const std::string nodeName = choc::text::replace(__func__, "_", " ");
      
      auto* node = new Node(0, nodeName.c_str());
      node->category = "Utility";
      
      node->type = NodeType::Comment;
      node->size = ImVec2(300, 200);
      
      return node;
    }
    
    static Node* Message()
    {
      const std::string nodeName = choc::text::replace(__func__, "_", " ");
      
      auto* node = new Node(0, nodeName.c_str(), ImColor(128, 195, 248));
      node->category = "Utility";
      
      node->type = NodeType::Simple;
      node->outputs.emplace_back(0, "Message", PinType::String);
      
      return node;
    }
    
    static Node* Dummy_Node()
    {
      const std::string nodeName = choc::text::replace(__func__, "_", " ");
      
      auto* node = new Node(0, nodeName.c_str(), ImColor(128, 195, 248));
      node->category = "Utility";
      
      node->type = NodeType::Blueprint;
      
      node->inputs.emplace_back(0, "Message", PinType::String);
      
      node->outputs.emplace_back(0, "Message", PinType::String);
      
      return node;
    }
  };
  
  namespace Nodes
  {
    Node* Nodes::DemoFactory::SpawnNodeStatic(const std::string& category, const std::string& type)
    {
      auto cat = s_registry.find(category);
      if (cat != s_registry.end())
      {
        auto nodes = cat->second.find(type);
        if (nodes != cat->second.end())
        {
          return nodes->second();
        }
        else
        {
          IK_LOG_ERROR("SpawnNodeStatic", "Category {0} does not contain node type {1}", category, type);
          return nullptr;
        }
      }
      
      IK_LOG_ERROR("SpawnNodeStatic",  "Category {0} does not exist", category);
      return nullptr;
    }

    template <typename DemoFactory> const Registry Factory<DemoFactory>::s_registry =
    {
      {NODE_CATEGORY(Utility),  {
        DECLARE_NODE(Utility, Comment),
        DECLARE_NODE(Utility, Message),
        DECLARE_NODE(Utility, Dummy_Node)
      }},
      {NODE_CATEGORY(DemoNodes), {
        DECLARE_NODE(DemoNodes, Some_Float),
        DECLARE_NODE(DemoNodes, All_Pin_Types),
        DECLARE_NODE(DemoNodes, All_Pin_Type_Arrays),
        DECLARE_NODE(DemoNodes, Simple_All_Pin_Types),
        DECLARE_NODE(DemoNodes, Simple_All_Pin_Type_Arrays)
      }},
    };
  } // namespace Nodes

} // namespace IKan
