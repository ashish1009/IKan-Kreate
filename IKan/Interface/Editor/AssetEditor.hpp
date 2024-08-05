//
//  AssetEditor.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Assets/Asset.hpp"
#include "Core/TimeStep.hpp"
#include "Events/Events.h"

namespace IKan
{
  /// This class is the base to render the asset editor
  class AssetEditor
  {
  public:
    /// This is default destructor
    virtual ~AssetEditor() = default;
    
    /// This function handles update of asset editor
    /// - Parameter ts: time step
    virtual void OnUpdate([[maybe_unused]] TimeStep ts) {}
    /// This function handles events for asset editor
    /// - Parameter e: event
    virtual void OnEvent([[maybe_unused]] Event& e) {}
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
  
  /// This class manages all the asset editors in the engine
  class AssetEditorManager
  {
  public:
    /// This function unregisters the default Asset Editors
    static void UnregisterAllEditors();
    /// This function handles update of asset editor
    /// - Parameter ts: time step
    static void OnUpdate(TimeStep ts);
    /// This function handles events for asset editor
    /// - Parameter e: event
    static void OnEvent(Event& e);
    /// This function renders ImGui
    static void OnImGuiRender();
    /// This function Opens the Asset editor
    /// - Parameter asset asset
    static void OpenEditor(const Ref<Asset>& asset);
    /// This function closes the Asset editor
    /// - Parameter asset asset
    static void CloseEditor(const Ref<Asset>& asset);
    /// This function Opens the Asset editor
    static void Clear();
    
    /// This function register an asset in map
    template<typename T> static void RegisterEditor(AssetType type)
    {
      static_assert(std::is_base_of<AssetEditor, T>::value,
                    "AssetEditorPanel::RegisterEditor requires template type to inherit from AssetEditor");
      IK_ASSERT(s_editors.find(type) == s_editors.end(), "There's already an editor for that asset!");
      s_editors[type] = CreateScope<T>();
    }
    
  private:
    static std::unordered_map<AssetType, Scope<AssetEditor>> s_editors;
  };
} // namespace IKan
