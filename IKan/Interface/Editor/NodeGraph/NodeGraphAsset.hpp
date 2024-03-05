//
//  NodeGraphAsset.hpp
//  IKan
//
//  Created by Ashish . on 05/03/24.
//

#pragma once

#include "Asset/Asset.hpp"

namespace IKan
{
  class NodeGraph : public Asset
  {
  public:
    std::vector<Node> nodes;
    std::vector<Link> links;

    ASSET_TYPE(NodeGraph)
  };
} // namespace IKan
