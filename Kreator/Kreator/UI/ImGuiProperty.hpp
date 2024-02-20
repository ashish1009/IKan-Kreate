//
//  ImGuiProperty.hpp
//  Kreator
//
//  Created by Ashish . on 20/02/24.
//

#pragma once

namespace Kreator::UI
{
  /// This function render property header
  /// - Parameter name: header name
  bool PropertyGridHeader(const std::string& name, bool openByDefault = true, float height = 6, float rounding = 8);
  /// This function Ends the tree node
  void PropertyGridHeaderEnd();
  /// This function starts the property header
  void BeginPropertyGrid(uint32_t columns = 2, float height = 2);
  /// This function ends the property header
  void EndPropertyGrid();

  bool PropertyDropdown(const char* label, const char** options, int32_t optionCount, int32_t* selected);
  bool PropertyDropdownNoLabel(const char* strID, const char** options, int32_t optionCount, int32_t* selected);
  bool PropertyDropdown(const char* label, const std::vector<std::string>& options, int32_t optionCount, int32_t* selected);
  bool PropertyDropdownNoLabel(const char* strID, const std::vector<std::string>& options, int32_t optionCount, int32_t* selected);
} // namespace Kreator::UI
