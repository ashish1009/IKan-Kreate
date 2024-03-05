//
//  NodeGraphEditor.hpp
//  Kreator
//
//  Created by Ashish . on 05/03/24.
//

#pragma once

namespace Kreator
{
  class NodeGraphEditor : public AssetEditor
  {
  public:
    /// Default Texture Viewer constructor
    NodeGraphEditor();
    ~NodeGraphEditor();
    
    /// @see AssetEditor
    virtual void SetAsset(const Ref<Asset>& asset) override;
    
  private:
    /// @see AssetEditor
    virtual void OnClose() override;
    /// @see AssetEditor
    virtual void Render() override;
  };
} // namespace Kreator
