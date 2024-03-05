//
//  NodeGraphEditor.cpp
//  Kreator
//
//  Created by Ashish . on 05/03/24.
//

#include "NodeGraphEditor.hpp"

namespace Kreator
{
  NodeGraphEditor::NodeGraphEditor()
  : AssetEditor("Graph Editor")
  {
    IK_PROFILE();
  }
  
  NodeGraphEditor::~NodeGraphEditor()
  {
    IK_PROFILE();
  }
  
  void NodeGraphEditor::SetAsset(const Ref<Asset>& asset)
  {
    IK_PROFILE();
  }
  void NodeGraphEditor::OnClose()
  {
    IK_PROFILE();
  }
  void NodeGraphEditor::Render()
  {
    IK_PERFORMANCE("NodeGraphEditor::Render")
  }

} // namespace Kreator
