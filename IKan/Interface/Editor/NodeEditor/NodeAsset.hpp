//
//  NodeAsset.hpp
//  IKan
//
//  Created by Ashish . on 05/03/24.
//

#include "Asset/Asset.hpp"

namespace IKan 
{
  struct NodeGraph : public Asset
  {
  public:
    std::vector<Node> nodes;
    std::vector<Link> links;
    
    NodeGraph() = default;
    ASSET_TYPE(NodeGraph);
  };
} // namespace IKan
