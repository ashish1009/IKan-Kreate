//
//  NodeGraphAsset.hpp
//  IKan
//
//  Created by Ashish . on 05/03/24.
//

#pragma once

#include "Editor/NodeGraph/Nodes.hpp"
#include "Asset/Asset.hpp"

namespace IKan
{
  class NodeGraph : public Asset
  {
  public:
    std::vector<Node> nodes;
    std::vector<Link> links;
    
    static Ref<NodeGraph> Create() {return CreateRef<NodeGraph>();}

    ASSET_TYPE(NodeGraph)
  };
} // namespace IKan
