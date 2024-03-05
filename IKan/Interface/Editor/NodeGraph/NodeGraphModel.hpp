//
//  NodeGraphModel.hpp
//  IKan
//
//  Created by Ashish . on 05/03/24.
//

#pragma once

#include "Nodes.hpp"

namespace IKan
{
  class NodeEditorModel
  {
  public:
    NodeEditorModel() = default;
    virtual ~NodeEditorModel() = default;
    
    // Graph Model Interface -------------------------------------------------------------------------------
    /// This funciton returns the nodes
    virtual std::vector<Node>& GetNodes() = 0;
    /// This funciton returns the Links
    virtual std::vector<Link>& GetLinks() = 0;
    /// This funciton returns the nodes
    virtual const std::vector<Node>& GetNodes() const = 0;
    /// This funciton returns the Links
    virtual const std::vector<Link>& GetLinks() const = 0;
    /// This funciton returns Node Registry
    virtual const Nodes::Registry& GetNodeTypes() const = 0;
    
    // Graph Model -----------------------------------------------------------------------------------------
    /// This funciton returns the Node ID from String
    uintptr_t GetIDFromString(const std::string& idString) const;
    
    /// This funcion search node from ID
    /// - Parameter id: ID
    Node* FindNode(UUID id);
    /// This funcion search link from ID
    /// - Parameter id: ID
    Link* FindLink(UUID id);
    /// This funcion search Pin from ID
    /// - Parameter id: ID
    Pin* FindPin(UUID id);
    /// This funcion search node from ID
    /// - Parameter id: ID
    const Node* FindNode(UUID id) const;
    /// This funcion search link from ID
    /// - Parameter id: ID
    const Link* FindLink(UUID id) const;
    /// This funcion search node from ID
    /// - Parameter id: ID
    const Pin* FindPin(UUID id)   const;
    
    /// This funcion returns the link of pin
    /// - Parameter id: pin ID
    Link* GetLinkConnectedToPin(UUID id);
    /// This funcion returns the link of pin
    /// - Parameter id: pin ID
    const Link* GetLinkConnectedToPin(UUID id) const;
    /// This funcion checks if pin is linked
    bool IsPinLinked(UUID id);
    
    struct LinkQueryResult
    {
      enum class Reason
      {
        CanConnect,
        InvalidStartPin,
        InvalidEndPin,
        IncompatiblePinKind,
        IncompatibleStorageKind,
        IncompatibleType,
        SamePin,
        SameNode
      };
      Reason reason;
      
      LinkQueryResult(Reason value) : reason(value) {}
      
      explicit operator bool()
      {
        return reason == LinkQueryResult::Reason::CanConnect;
      }
    };
    
    /// This funcion checks if link can be created between the pins
    /// - Parameters:
    ///   - startPin: start pin
    ///   - endPin: end pin
    LinkQueryResult CanCreateLink(Pin* startPin, Pin* endPin);
    /// This funcion links the pins
    /// - Parameters:
    ///   - startPin: start pin
    ///   - endPin: end pin
    void CreateLink(Pin* startPin, Pin* endPin);
    /// This funcion removes links
    /// - Parameter linkID: link ID
    void RemoveLink(UUID linkId);
    
    /// This function creates the node
    /// - Parameters:
    ///   - category: category of node
    ///   - typeID: type ID
    Node* CreateNode(const std::string& category, const std::string& typeID);
    /// This function removes node
    /// - Parameter nodeId: node id
    void  RemoveNode(UUID nodeId);
    
    /// This funcion saves the data
    void SaveAll();
    /// This funcion Loads the data
    void LoadAll();
    
    /// This funcion compiles the graph
    void CompileGraph();
    
    // Graph Model Interface ----------------------------------------------------------------------------------------
    using NodeID = UUID;
    using PinID = UUID;
    
    std::function<void()> onNodeCreated = nullptr, onNodeDeleted = nullptr;
    std::function<void()> onLinkCreated = nullptr, onLinkDeleted = nullptr;
    
    // This is called from the node graph editor
    std::function<void(NodeID, PinID)> onPinValueChanged = nullptr;
    
    std::function<bool(Ref<Asset>& graphAsset)> onCompile = nullptr;
    std::function<void()> onPlay = nullptr, onStop = nullptr;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(NodeEditorModel);
    
  protected:
    // Assigns relationship between pins and nodes
    /// This funcion builds the node
    /// - Parameter node: node
    void BuildNode(Node* node);
    /// This funcion builds the nodes
    void BuildNodes();
    
    // Callbacks
    virtual void OnNodeCreated() { IK_ASSERT(false) }
    virtual void OnNodeDeleted() { IK_ASSERT(false) }
    virtual void OnLinkCreated() { IK_ASSERT(false) }
    virtual void OnLinkDeleted() { IK_ASSERT(false) }
    
    virtual void OnCompileGraph() { IK_ASSERT(false) };
    
  private:
    // Member Functions --------------------------------------------------------------------------------------------
    /// This function assign valid ID to node
    /// - Parameter node: node
    void AssignIds(Node* node);
    /// This function calls on spawn of node
    /// - Parameter node: node
    void OnNodeSpawned(Node* node);
    /// This function deletes dead links
    /// - Parameter nodeId: node
    void DeleteDeadLinks(UUID nodeId);
    
    // Virtual methods ---------------------------------------------------------------------------------------------
    virtual void Serialize() = 0;
    virtual void Deserialize() = 0;
    virtual Nodes::AbstractFactory* GetNodeFactory() = 0;
  };
} // namespace IKan
