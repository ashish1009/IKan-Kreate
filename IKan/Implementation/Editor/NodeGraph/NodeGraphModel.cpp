//
//  NodeEditorModel.cpp
//  IKan
//
//  Created by Ashish . on 05/03/24.
//

#include <text/choc_StringUtilities.h>
#include "NodeGraphModel.hpp"

namespace IKan
{
  // IDs ----------------------------------------------------------------------------------
  uintptr_t NodeEditorModel::GetIDFromString(const std::string& idString) const
  {
    std::stringstream stream(idString);
    uintptr_t ret;
    stream >> ret;
    return ret;
  }
  
  void NodeEditorModel::AssignIds(Node* node)
  {
    node->ID = UUID();
    
    for (auto& input : node->inputs)
    {
      input.ID = UUID();
    }
    
    for (auto& output : node->outputs)
    {
      output.ID = UUID();
    }
  }
  
  void NodeEditorModel::OnNodeSpawned(Node* node)
  {
    if (node)
    {
      AssignIds(node);
      GetNodes().push_back(*node);
    }
  }
  
  // Find Items ----------------------------------------------------------------------------------
  Node* NodeEditorModel::FindNode(UUID id)
  {
    for (auto& node : GetNodes())
    {
      if (node.ID == id)
      {
        return &node;
      }
    }
    
    return nullptr;
  }
  
  const Node* NodeEditorModel::FindNode(UUID id) const
  {
    for (auto& node : GetNodes())
    {
      if (node.ID == id)
      {
        return &node;
      }
    }
    
    return nullptr;
  }
  
  Link* NodeEditorModel::FindLink(UUID id)
  {
    for (auto& link : GetLinks())
    {
      if (link.ID == id)
      {
        return &link;
      }
    }
    
    return nullptr;
  }
  
  const Link* NodeEditorModel::FindLink(UUID id) const
  {
    for (auto& link : GetLinks())
    {
      if (link.ID == id)
      {
        return &link;
      }
    }
    
    return nullptr;
  }
  
  Pin* NodeEditorModel::FindPin(UUID id)
  {
    if (!id)
    {
      return nullptr;
    }
    
    for (auto& node : GetNodes())
    {
      for (auto& pin : node.inputs)
      {
        if (pin.ID == id)
        {
          return &pin;
        }
      }
      
      for (auto& pin : node.outputs)
      {
        if (pin.ID == id)
        {
          return &pin;
        }
      }
    }
    
    return nullptr;
  }
  
  const Pin* NodeEditorModel::FindPin(UUID id) const
  {
    if (!id)
      return nullptr;
    
    for (auto& node : GetNodes())
    {
      for (auto& pin : node.inputs)
      {
        if (pin.ID == id)
        {
          return &pin;
        }
      }
      
      for (auto& pin : node.outputs)
      {
        if (pin.ID == id)
        {
          return &pin;
        }
      }
    }
    
    return nullptr;
  }
  
  // Links ----------------------------------------------------------------------------------
  Link* NodeEditorModel::GetLinkConnectedToPin(UUID id)
  {
    if (!id)
    {
      return nullptr;
    }
    
    for (auto& link : GetLinks())
    {
      if (link.startPinID == id || link.endPinID == id)
      {
        return &link;
      }
    }
    
    return nullptr;
  }
  
  const Link* NodeEditorModel::GetLinkConnectedToPin(UUID id) const
  {
    if (!id)
    {
      return nullptr;
    }
    
    for (auto& link : GetLinks())
    {
      if (link.startPinID == id || link.endPinID == id)
      {
        return &link;
      }
    }
    
    return nullptr;
  }
  
  bool NodeEditorModel::IsPinLinked(UUID id)
  {
    return GetLinkConnectedToPin(id) != nullptr;
  }
  
  void NodeEditorModel::DeleteDeadLinks(UUID nodeId)
  {
    auto wasConnectedToTheNode = [&](const Link& link)
    {
      return (!FindPin(link.startPinID)) or (!FindPin(link.endPinID))
      or FindPin(link.startPinID)->node->ID == nodeId
      or FindPin(link.endPinID)->node->ID == nodeId;
    };
    
    auto& links = GetLinks();
    
    auto removeIt = std::remove_if(links.begin(), links.end(), wasConnectedToTheNode);
    const bool linkRemoved = removeIt != links.end();
    
    links.erase(removeIt, links.end());
    
    if (linkRemoved)
      OnLinkDeleted();
  }
  
  NodeEditorModel::LinkQueryResult NodeEditorModel::CanCreateLink(Pin* startPin, Pin* endPin)
  {
    if (!startPin)                                  return LinkQueryResult::Reason::InvalidStartPin;
    else if (!endPin)                               return LinkQueryResult::Reason::InvalidEndPin;
    else if (endPin == startPin)                    return LinkQueryResult::Reason::SamePin;
    else if (endPin->node == startPin->node)        return LinkQueryResult::Reason::SameNode;
    else if (endPin->kind == startPin->kind)        return LinkQueryResult::Reason::IncompatiblePinKind;
    else if (endPin->storage != startPin->storage)  return LinkQueryResult::Reason::IncompatibleStorageKind;
    else if (endPin->type != startPin->type)        return LinkQueryResult::Reason::IncompatibleType;
    
    return LinkQueryResult::Reason::CanConnect;
  }
  
  void NodeEditorModel::CreateLink(Pin* startPin, Pin* endPin)
  {
    auto& links = GetLinks();
    
    links.emplace_back(Link(UUID(), startPin->ID, endPin->ID));
    links.back().color = GetIconColor(startPin->type);
    
    OnLinkCreated();
  }
  
  void NodeEditorModel::RemoveLink(UUID linkId)
  {
    auto& links = GetLinks();
    
    auto id = std::find_if(links.begin(), links.end(), [linkId](auto& link) { return link.ID == linkId; });
    if (id != links.end())
    {
      links.erase(id);
      
      OnLinkDeleted();
    }
  }
  
  // Nodes ----------------------------------------------------------------------------------
  void NodeEditorModel::RemoveNode(UUID nodeId)
  {
    auto& nodes = GetNodes();
    
    auto id = std::find_if(nodes.begin(), nodes.end(), [nodeId](auto& node) { return node.ID == nodeId; });
    if (id != nodes.end())
    {
      DeleteDeadLinks(nodeId);
      nodes.erase(id);
      
      OnNodeDeleted();
    }
  }
  
  void NodeEditorModel::BuildNode(Node* node)
  {
    for (auto& input : node->inputs)
    {
      input.node = node;
      input.kind = PinKind::Input;
    }
    
    for (auto& output : node->outputs)
    {
      output.node = node;
      output.kind = PinKind::Output;
    }
  }
  
  void NodeEditorModel::BuildNodes()
  {
    for (auto& node : GetNodes())
      BuildNode(&node);
  }
  
  Node* NodeEditorModel::CreateNode(const std::string& category, const std::string& typeID)
  {
    std::string safeTypeName = choc::text::replace(typeID, " ", "_");
    std::string safeCategory = choc::text::replace(category, " ", "_");
    
    OnNodeSpawned(GetNodeFactory()->SpawnNode(safeCategory, safeTypeName));
    BuildNode(&GetNodes().back());
    
    OnNodeCreated();
    
    return &GetNodes().back();
  }
  
  // Serialization ----------------------------------------------------------------------------------
  void NodeEditorModel::SaveAll()
  {
    Serialize();
  }
  
  void NodeEditorModel::LoadAll()
  {
    Deserialize();
    BuildNodes();
  }
  
  // Model Interface ----------------------------------------------------------------------------------
  void NodeEditorModel::CompileGraph()
  {
    OnCompileGraph();
  }
} // namespace IKan
