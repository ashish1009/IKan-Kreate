//
//  ImGuiProperty.hpp
//  IKan
//
//  Created by Ashish . on 04/08/23.
//

#pragma once

namespace IKan::UI
{
  /// This function render property header
  /// - Parameter name: header name
  bool PropertyGridHeader(const std::string& name, bool openByDefault = true);
  /// This function Ends the tree node
  void PropertyGridHeaderEnd();
} // namespace IKan::UI
