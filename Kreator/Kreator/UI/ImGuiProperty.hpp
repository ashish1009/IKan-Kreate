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
  void BeginPropertyGrid(uint32_t columns = 2, float height = 2, float spacing = 5);
  /// This function ends the property header
  void EndPropertyGrid();

  /// Tooltip as help marker
  void HelpMarker(const char* desc);

  // Proeprty --------------------------------------------------------------------------------------------------------
  bool Property(const char* label, std::string& value);
  bool PropertyMultiline(const char* label, std::string& value);
  void Property(const char* label, const std::string& value);
  void Property(const char* label, const char* value);
  bool Property(const char* label, char* value, size_t length);
  bool Property(const char* label, bool& value, const char* helpText = "");
  bool Property(const char* label, int8_t& value, int8_t min = 0, int8_t max = 0);
  bool Property(const char* label, int16_t& value, int16_t min = 0, int16_t max = 0);
  bool Property(const char* label, int32_t& value, int32_t min = 0, int32_t max = 0);
  bool Property(const char* label, int64_t& value, int64_t min = 0, int64_t max = 0);
  bool Property(const char* label, uint8_t& value, uint8_t minValue = 0, uint8_t maxValue = 0);
  bool Property(const char* label, uint16_t& value, uint16_t minValue = 0, uint16_t maxValue = 0);
  bool Property(const char* label, uint32_t& value, uint32_t minValue = 0, uint32_t maxValue = 0);
  bool Property(const char* label, uint64_t& value, uint64_t minValue = 0, uint64_t maxValue = 0);
  bool Property(const char* label, float& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f, const char* helpText = "");
  bool Property(const char* label, glm::vec2& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f);
  bool Property(const char* label, glm::vec3& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f);
  bool Property(const char* label, glm::vec4& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f);
  bool PropertySlider(const char* label, int& value, int min = -INT_MAX, int max = INT_MAX);
  bool PropertySlider(const char* label, float& value, float min = -FLT_MAX, float max = FLT_MAX);
  bool PropertySlider(const char* label, glm::vec2& value, float min = -FLT_MAX, float max = FLT_MAX);
  bool PropertySlider(const char* label, glm::vec3& value, float min = -FLT_MAX, float max = FLT_MAX);
  bool PropertySlider(const char* label, glm::vec4& value, float min = -FLT_MAX, float max = FLT_MAX);
  bool PropertyInput(const char* label, int8_t& value, int8_t step = 1, int8_t stepFast = 1);
  bool PropertyInput(const char* label, int16_t& value, int16_t step = 1, int16_t stepFast = 1);
  bool PropertyInput(const char* label, int32_t& value, int32_t step = 1, int32_t stepFast = 1);
  bool PropertyInput(const char* label, int64_t& value, int64_t step = 1, int64_t stepFast = 1);
  bool PropertyInput(const char* label, uint8_t& value, uint8_t step = 1, uint8_t stepFast = 1);
  bool PropertyInput(const char* label, uint16_t& value, uint16_t step = 1, uint16_t stepFast = 1);
  bool PropertyInput(const char* label, uint32_t& value, uint32_t step = 1, uint32_t stepFast = 1);
  bool PropertyInput(const char* label, uint64_t& value, uint64_t step = 1, uint64_t stepFast = 1);
  bool PropertyColor(const char* label, glm::vec3& value);
  bool PropertyColor(const char* label, glm::vec4& value);
  bool PropertyDropdown(const char* label, const char** options, int32_t optionCount, int32_t* selected);
  bool PropertyDropdownNoLabel(const char* strID, const char** options, int32_t optionCount, int32_t* selected);
  bool PropertyDropdown(const char* label, const std::vector<std::string>& options, int32_t optionCount, int32_t* selected);
  bool PropertyDropdownNoLabel(const char* strID, const std::vector<std::string>& options, int32_t optionCount, int32_t* selected);
} // namespace Kreator::UI
