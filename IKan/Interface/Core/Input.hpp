//
//  Input.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

namespace IKan
{
  enum class CursorMode
  {
    Normal = 0,
    Hidden = 1,
    Locked = 2
  };
  
  /// This class is interface for handling the Input. Resposible for checking the type of input given to Window or
  /// application (Either Key or Mouse).
  /// - Note: Polling to be done each frame
  class Input
  {
  public:
    /// This function checks if a specific key is pressed, return true if pressed else false
    /// - Parameter keyCode: keycode to be checked
    static bool IsKeyPressed(Key keyCode);
    /// This function checks if a specific Mouse button is pressed, return true if pressed else false
    /// - Parameter button: mouse button code to be checked
    static bool IsMouseButtonPressed(MouseButton button);
    
    /// This function checks if a specific key is released, return true if released else false
    /// - Parameter keyCode: keycode to be checked
    static bool IsKeyReleased(Key keyCode);
    /// This function checks if a specific key is released, return true if released else false
    /// - Parameter button: mouse button code to be checked
    static bool IsMouseButtonReleased(MouseButton button);
    
    /// This function returns the Mouse position (X, Y) in pair
    static glm::vec2 GetMousePosition();
    /// This function returns the mouse position X
    static float GetMouseX();
    /// This function returns the mouse position Y
    static float GetMouseY();
    
    /// This function set the cursor mode
    static void SetCursorMode(CursorMode mode);
    /// This function returns the cursor mode
    static CursorMode GetCursorMode();
    
  private:
    DELETE_ALL_CONSTRUCTORS(Input);
  };
} // namespace IKan
