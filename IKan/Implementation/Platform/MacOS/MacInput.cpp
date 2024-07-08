//
//  Input.cpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#include "Input.hpp"

#include <GLFW/glfw3.h>

#include "Core/Application.hpp"

namespace IKan
{
  bool Input::IsKeyPressed(Key key)
  {
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetNativeWindow());
    const int32_t state = glfwGetKey(window, static_cast<int32_t>(key));
    return ((state == GLFW_PRESS) or (state == GLFW_REPEAT));
  }
  
  bool Input::IsMouseButtonPressed(MouseButton button)
  {
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetNativeWindow());
    const int32_t state = glfwGetMouseButton(window, static_cast<int32_t>(button));
    return state == GLFW_PRESS;
  }
  
  bool Input::IsKeyReleased(Key key)
  {
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetNativeWindow());
    const int32_t state = glfwGetKey(window, static_cast<int32_t>(key));
    return state == GLFW_RELEASE;
  }
  
  bool Input::IsMouseButtonReleased(MouseButton button)
  {
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetNativeWindow());
    const int32_t state = glfwGetMouseButton(window, static_cast<int32_t>(button));
    return state == GLFW_RELEASE;
  }
  
  glm::vec2 Input::GetMousePosition()
  {
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetNativeWindow());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    return { (float)xpos, (float)ypos };
  }
  
  float Input::GetMouseX()
  {
    return GetMousePosition().x;
  }
  
  float Input::GetMouseY()
  {
    return GetMousePosition().y;
  }
  
  void Input::SetCursorMode(CursorMode mode)
  {
    GLFWwindow* window  = static_cast<GLFWwindow*>(Application::Get().GetNativeWindow());
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
  }
  
  CursorMode Input::GetCursorMode()
  {
    Window* window = Application::Get().GetWindow();
    return (CursorMode)(glfwGetInputMode(static_cast<GLFWwindow*>(window->GetNativeWindow()), GLFW_CURSOR) - GLFW_CURSOR_NORMAL);
  }
} // namespace IKan
