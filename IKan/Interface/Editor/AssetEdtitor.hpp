//
//  AssetEdtitor.hpp
//  IKan
//
//  Created by Ashish . on 05/08/23.
//

#pragma once

#include "Asset/Asset.hpp"
#include "Core/TimeStep.hpp"
#include "Event/Event.h"

namespace IKan
{
  class AssetEditor
  {
  public:
    /// This is default destructor
    virtual ~AssetEditor() = default;
    
    /// This function handles update of asset editor
    /// - Parameter ts: time step
    virtual void OnUpdate(TimeStep ts) {}
    /// This function handles events for asset editor
    /// - Parameter e: event
    virtual void OnEvent(Event& e) {}
    /// This function handles imgui render of asset editor
    virtual void OnImGuiRender();
    /// This function handles Sett the asset
    /// - Parameter asset: asset
    virtual void SetAsset(const Ref<Asset>& asset) = 0;
    
    /// This function set the panel open
    /// - Parameter isOpen: open flag
    void SetOpen(bool isOpen);
    
  protected:
    /// This is the Base constructor pf panel
    /// - Parameter title: title
    AssetEditor(const char* title);
    
    /// This function set min size
    /// - Parameters:
    ///   - width: width
    ///   - height: height
    void SetMinSize(uint32_t width, uint32_t height);
    /// This function set max size
    /// - Parameters:
    ///   - width: width
    ///   - height: height
    void SetMaxSize(uint32_t width, uint32_t height);
    
    /// This function sets the title of panel
    /// - Parameter newTitle: New title
    void SetTitle(const std::string& newTitle);
    
    /// This function returns the title of panel
    const std::string& GetTitle() const;
    
  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function handles the close of panel
    virtual void OnClose() = 0;
    /// This function Renders the panel
    virtual void Render() = 0;
    /// This function returns the flags
    virtual ImGuiWindowFlags GetWindowFlags() const;
    
    // Member Variables ----------------------------------------------------------------------------------------------
    std::string m_title;
    bool m_isOpen = false;
    
    ImGuiWindowFlags m_flags = 0;
    ImVec2 m_minSize, m_maxSize;
  };
} // namespace IKan
