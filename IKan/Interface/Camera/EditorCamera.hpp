//
//  EditorCamera.hpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#pragma once

#include <glm/detail/type_quat.hpp>

#include "Camera/Camera.hpp"
#include "Core/TimeStep.hpp"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"

namespace IKan
{
  /// This is the camera mode enum
  enum class CameraMode : uint8_t
  {
    NONE, FLYCAM, ARCBALL
  };
  
  /// This structure stores the pan speed of camera
  struct PanSpeed
  {
    float X, Y;
  };
  
  /// This class is the editor camera
  class EditorCamera : public Camera
  {
  public:
    /// This is the constructor for Editor camera with default parameters
    EditorCamera();
    /// This is the constructor for Editor camera
    /// - Parameters:
    ///   - degFov: FOV in Degree
    ///   - width: Camera width
    ///   - height: Camera Height
    ///   - nearP: Camera Near plane
    ///   - farP: Camera Far Plane
    EditorCamera(const float degFov, const float width, const float height,
                 const float nearP = 0.0001f, const float farP = 10000.0f);
    
    /// This function initialise the camera
    void Initialize();
    
    /// This function focus the camera at focal point
    /// - Parameter focusPoint: camera focal point
    void Focus(const glm::vec3& focusPoint);
    /// This function updates the camera
    /// - Parameter ts: time step
    void OnUpdate(TimeStep ts);
    /// This function handles the camera events
    /// - Parameter e: event
    void OnEvent(Event& e);
    
    // Setters -------------------------------------------------------------------------------------------------------
    /// This function updates the camera active flag
    /// - Parameter active: active flag
    void SetActive(bool active);
    /// This function updates the camera distance
    /// - Parameter distance: new distance
    void SetDistance(float distance);
    /// This function update the camera viewport size
    /// - Parameters:
    ///   - width: new width
    ///   - height: new height
    void SetViewportSize(const uint32_t width, const uint32_t height);
    /// This function set the normal camera speed
    /// - Parameter camSpeed: camera speed
    void SetNormalSpeed(float camSpeed);
    
    /// This funcion set the camera orientation at Front
    void SetFront();
    /// This funcion set the camera orientation at Top
    void SetTop();
    /// This funcion set the camera orientation at Right
    void SetRight();
    
    // Getters -------------------------------------------------------------------------------------------------------
    /// This function returns if camera is active
    bool IsActive() const;
    /// This function returns the camera mode
    CameraMode GetCurrentMode() const;
    /// This function returns the camera distance
    float GetDistance() const;
    /// This function returns the camera focal point
    const glm::vec3& GetFocalPoint() const;
    /// This function Caculate and return the camera position
    glm::vec3 CalculatePosition() const;
    /// This function returns the camera Pan Speed
    const PanSpeed PanSpeed() const;
    /// This function returns the camera Rotation speed
    float RotationSpeed() const;
    /// This function returns the camera Zoom speed
    float ZoomSpeed() const;
    
    /// This function returns the camera View Matrix
    const glm::mat4& GetViewMatrix() const;
    /// This function returns the camera View Projection Matrix
    glm::mat4 GetViewProjection() const;
    /// This function returns the camera reverse View Projection Matrix
    glm::mat4 GetUnReversedViewProjection() const;
    
    /// This function returns the camera Up Direction
    glm::vec3 GetUpDirection() const;
    /// This function returns the camera Right Direction
    glm::vec3 GetRightDirection() const;
    /// This function returns the camera Forward Direction
    glm::vec3 GetForwardDirection() const;
    
    /// This function returns the camera Position
    const glm::vec3& GetPosition() const;
    
    /// This function returns the camera Orientation
    glm::quat GetOrientation() const;
    
    /// This function returns the normal speed of camera
    float GetNormalSpeed() const;
    /// This function returns the Vertical FOV of camera
    float GetVerticalFOV() const;
    /// This function returns the Aspect Ratio of camera
    float GetAspectRatio() const;
    /// This function returns the Near Clip of camera
    float GetNearClip() const;
    /// This function returns the Far Clip of camera
    float GetFarClip() const;
    /// This function returns the Pitch of camera
    float GetPitch() const;
    /// This function returns the Yaw of camera
    float GetYaw() const;
    /// This function returns the speed of camera
    float GetCameraSpeed() const;
    
    /// This funcion returns the camera orientation at Front
    bool IsFront() const;
    /// This funcion returns the camera orientation at Top
    bool IsTop() const;
    /// This funcion returns the camera orientation at Right
    bool IsRight() const;
    /// This funcion returns the camera orientation is free
    bool IsFree() const;
    
  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function Updates the camera View
    void UpdateCameraView();
    
    /// This function handles the mouse scroll
    /// - Parameter e: Mouse event
    bool OnMouseScroll(MouseScrolledEvent& e);
    
    /// This function handle mouse pan
    /// - Parameter delta: pan delta
    void MousePan(const glm::vec2& delta);
    /// This function handle mouse rotation
    /// - Parameter delta: pan delta
    void MouseRotate(const glm::vec2& delta);
    /// This function handle mouse zoom
    /// - Parameter delta: pan delta
    void MouseZoom(float delta);
    
    // Member Variables ----------------------------------------------------------------------------------------------
    glm::mat4 m_viewMatrix;
    glm::vec3 m_position, m_direction, m_focalPoint{1, 0, 0};
    
    // Perspective projection params
    float m_verticalFOV, m_aspectRatio, m_nearClip, m_farClip;
    
    bool m_isActive {false};
    bool m_panning, m_rotating;
    glm::vec2 m_initialMousePosition {};
    glm::vec3 m_initialFocalPoint, m_initialRotation;
    
    float m_distance;
    float m_normalSpeed{ 0.002f };
    
    float m_pitch, m_yaw;
    float m_pitchDelta{}, m_yawDelta{};
    glm::vec3 m_positionDelta{};
    glm::vec3 m_rightDirection{};
    
    CameraMode m_cameraMode{ CameraMode::ARCBALL };
    
    float m_minFocusDistance{ 100.0f };
    
    uint32_t m_viewportWidth{ 1280 }, m_viewportHeight{ 720 };
    
    enum class CameraOrientation
    {
      Free, Top, Front, Right
    };
    CameraOrientation m_cameraOrientation = CameraOrientation::Free;
    
    constexpr static float MIN_SPEED{ 0.0005f }, MAX_SPEED{ 2.0f };
  };
  
} // namespace IKan
