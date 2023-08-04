//
//  ImGuiProperty.hpp
//  IKan
//
//  Created by Ashish . on 04/08/23.
//

#pragma once

namespace Kreator_UI
{
  /// This function render property header
  /// - Parameter name: header name
  bool PropertyGridHeader(const std::string& name, bool openByDefault = true, float height = 6, float rounding = 15);
  /// This function Ends the tree node
  void PropertyGridHeaderEnd();
  /// This function starts the property header
  void BeginPropertyGrid(uint32_t columns = 2, float height = 4);
  /// This function ends the property header
  void EndPropertyGrid();
  
  /// Tooltip as help marker
  void HelpMarker(const char* desc);
  
  // Proeprty --------------------------------------------------------------------------------------------------------
  bool Property(const char* label, std::string& value);

} // namespace Kreator_UI
