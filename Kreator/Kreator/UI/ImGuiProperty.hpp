//
//  ImGuiProperty.hpp
//  Kreator
//
//  Created by Ashish . on 20/02/24.
//

#pragma once

namespace Kreator::UI
{
  bool PropertyDropdown(const char* label, const char** options, int32_t optionCount, int32_t* selected);
  bool PropertyDropdownNoLabel(const char* strID, const char** options, int32_t optionCount, int32_t* selected);
  bool PropertyDropdown(const char* label, const std::vector<std::string>& options, int32_t optionCount, int32_t* selected);
  bool PropertyDropdownNoLabel(const char* strID, const std::vector<std::string>& options, int32_t optionCount, int32_t* selected);
} // namespace Kreator::UI
