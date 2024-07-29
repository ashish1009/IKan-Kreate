//
//  ImGuiInputs.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

namespace Kreator::UI
{
  [[maybe_unused]] static bool DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0)
  {
    return ImGui::DragScalar(label, ImGuiDataType_Float, v, v_speed, &v_min, &v_max, format, flags);
  }
  
  [[maybe_unused]] static bool DragDouble(const char* label, double* v, float v_speed = 1.0f, double v_min = 0.0, double v_max = 0.0, const char* format = "%.3f", ImGuiSliderFlags flags = 0)
  {
    return ImGui::DragScalar(label, ImGuiDataType_Double, v, v_speed, &v_min, &v_max, format, flags);
  }
  
  [[maybe_unused]] static bool DragInt8(const char* label, int8_t* v, float v_speed = 1.0f, int8_t v_min = 0, int8_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
  {
    return ImGui::DragScalar(label, ImGuiDataType_S8, v, v_speed, &v_min, &v_max, format, flags);
  }
  
  [[maybe_unused]] static bool DragInt16(const char* label, int16_t* v, float v_speed = 1.0f, int16_t v_min = 0, int16_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
  {
    return ImGui::DragScalar(label, ImGuiDataType_S16, v, v_speed, &v_min, &v_max, format, flags);
  }
  
  [[maybe_unused]] static bool DragInt32(const char* label, int32_t* v, float v_speed = 1.0f, int32_t v_min = 0, int32_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
  {
    return ImGui::DragScalar(label, ImGuiDataType_S32, v, v_speed, &v_min, &v_max, format, flags);
  }
  
  [[maybe_unused]] static bool DragInt64(const char* label, int64_t* v, float v_speed = 1.0f, int64_t v_min = 0, int64_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
  {
    return ImGui::DragScalar(label, ImGuiDataType_S64, v, v_speed, &v_min, &v_max, format, flags);
  }
  
  [[maybe_unused]] static bool DragUInt8(const char* label, uint8_t* v, float v_speed = 1.0f, uint8_t v_min = 0, uint8_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
  {
    return ImGui::DragScalar(label, ImGuiDataType_U8, v, v_speed, &v_min, &v_max, format, flags);
  }
  
  [[maybe_unused]] static bool DragUInt16(const char* label, uint16_t* v, float v_speed = 1.0f, uint16_t v_min = 0, uint16_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
  {
    return ImGui::DragScalar(label, ImGuiDataType_U16, v, v_speed, &v_min, &v_max, format, flags);
  }
  
  [[maybe_unused]] static bool DragUInt32(const char* label, uint32_t* v, float v_speed = 1.0f, uint32_t v_min = 0, uint32_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
  {
    return ImGui::DragScalar(label, ImGuiDataType_U32, v, v_speed, &v_min, &v_max, format, flags);
  }
  
  [[maybe_unused]] static bool DragUInt64(const char* label, uint64_t* v, float v_speed = 1.0f, uint64_t v_min = 0, uint64_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
  {
    return ImGui::DragScalar(label, ImGuiDataType_U64, v, v_speed, &v_min, &v_max, format, flags);
  }
  
  [[maybe_unused]] static bool InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0)
  {
    return ImGui::InputScalar(label, ImGuiDataType_Float, v, &step, &step_fast, format, flags);
  }
  
  [[maybe_unused]] static bool InputDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0)
  {
    return ImGui::InputScalar(label, ImGuiDataType_Double, v, &step, &step_fast, format, flags);
  }
  
  [[maybe_unused]] static bool InputInt8(const char* label, int8_t* v, int8_t step = 1, int8_t step_fast = 1, ImGuiInputTextFlags flags = 0)
  {
    return ImGui::InputScalar(label, ImGuiDataType_S8, v, &step, &step_fast, "%d", flags);
  }
  
  [[maybe_unused]] static bool InputInt16(const char* label, int16_t* v, int16_t step = 1, int16_t step_fast = 10, ImGuiInputTextFlags flags = 0)
  {
    return ImGui::InputScalar(label, ImGuiDataType_S16, v, &step, &step_fast, "%d", flags);
  }
  
  [[maybe_unused]] static bool InputInt32(const char* label, int32_t* v, int32_t step = 1, int32_t step_fast = 100, ImGuiInputTextFlags flags = 0)
  {
    return ImGui::InputScalar(label, ImGuiDataType_S32, v, &step, &step_fast, "%d", flags);
  }
  
  [[maybe_unused]] static bool InputInt64(const char* label, int64_t* v, int64_t step = 1, int64_t step_fast = 1000, ImGuiInputTextFlags flags = 0)
  {
    return ImGui::InputScalar(label, ImGuiDataType_S64, v, &step, &step_fast, "%d", flags);
  }
  
  [[maybe_unused]] static bool InputUInt8(const char* label, uint8_t* v, uint8_t step = 1, uint8_t step_fast = 1, ImGuiInputTextFlags flags = 0)
  {
    return ImGui::InputScalar(label, ImGuiDataType_U8, v, &step, &step_fast, "%d", flags);
  }
  
  [[maybe_unused]] static bool InputUInt16(const char* label, uint16_t* v, uint16_t step = 1, uint16_t step_fast = 10, ImGuiInputTextFlags flags = 0)
  {
    return ImGui::InputScalar(label, ImGuiDataType_U16, v, &step, &step_fast, "%d", flags);
  }
  
  [[maybe_unused]] static bool InputUInt32(const char* label, uint32_t* v, uint32_t step = 1, uint32_t step_fast = 100, ImGuiInputTextFlags flags = 0)
  {
    return ImGui::InputScalar(label, ImGuiDataType_U32, v, &step, &step_fast, "%d", flags);
  }
  
  [[maybe_unused]] static bool InputUInt64(const char* label, uint64_t* v, uint64_t step = 1, uint64_t step_fast = 1000, ImGuiInputTextFlags flags = 0)
  {
    return ImGui::InputScalar(label, ImGuiDataType_U64, v, &step, &step_fast, "%d", flags);
  }
} // namespace Kreator::UI
