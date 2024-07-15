//
//  SceneCamera.hpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#pragma once

#include "Camera/Camera.hpp"

namespace IKan
{
  /// This class stores the camera active scene
  class SceneCamera : public Camera
  {
  public:
    /// This enum stores the projection type of scene camera
    /// - Perspective for 3D
    /// - Orthographics for 2D
    enum class ProjectionType : uint8_t
    {
      Perspective = 0, Orthographic = 1
    };
    
    /// This constructor creates the Scene Camera instance
    /// - Parameter projType: Projection type
    SceneCamera(ProjectionType projType = ProjectionType::Perspective);
    /// Default Scene Camera Desctructorr
    virtual ~SceneCamera() = default;
    
    // Setters -------------------------------------------------------------------------------------------------------
    /// This function updates the new Perspective Matrix (Only for Perspective camera)
    /// - Parameters:
    ///   - verticalFOV: new FOV
    ///   - nearClip: new near clip
    ///   - farClip: new far clip
    void SetPerspective(float verticalFOV, float nearClip = 0.1f, float farClip = 1000.0f);
    /// This function updates the new Orthographic Matrix (Only for Orthographic camera)
    /// - Parameters:
    ///   - size: new size of Camera
    ///   - nearClip: new near clip
    ///   - farClip: new far clip
    void SetOrthographic(float size, float nearClip = -1.0f, float farClip = 1.0f);
    
    /// This function updates the Viewport size of camera
    /// - Parameters:
    ///   - width: new width
    ///   - height: new height
    void SetViewportSize(const uint32_t width, const uint32_t height);
    
    /// This function updates FOV of Perspective Camera (Only for Perspective camera)
    /// - Parameter degVerticalFov: new FOV in degree
    void SetDegPerspectiveVerticalFOV(const float degVerticalFov);
    /// This function updates FOV of Perspective Camera (Only for Perspective camera)
    /// - Parameter degVerticalFov: new FOV in radians
    void SetRadPerspectiveVerticalFOV(const float degVerticalFov);
    /// This function updates the Near plane of camera
    /// - Parameter nearClip: New near plane
    void SetPerspectiveNearClip(const float nearClip);
    /// This function updates the Far plane of camera
    /// - Parameter farClip: new fat plane
    void SetPerspectiveFarClip(const float farClip);
    
    /// This function updates the Size of Orthographic Camera (Only for Orthographic Camera)
    /// - Parameter size: new size
    void SetOrthographicSize(const float size);
    /// This function updates the Near plane of camera
    /// - Parameter nearClip: New near plane
    void SetOrthographicNearClip(const float nearClip);
    /// This function updates the Far plane of camera
    /// - Parameter farClip: new fat plane
    void SetOrthographicFarClip(const float farClip);
    
    /// This function updates the Camera projection type
    /// - Parameter type: new projection type
    void SetProjectionType(ProjectionType type);
    
    // Getters -------------------------------------------------------------------------------------------------------
    /// This function returns FOV of Perspective Camera
    float GetDegPerspectiveVerticalFOV() const;
    /// This function returns FOV of Perspective Camera
    float GetRadPerspectiveVerticalFOV() const;
    /// This function returns the Near Plane value
    float GetPerspectiveNearClip() const;
    /// This function returns the Far Plane value
    float GetPerspectiveFarClip() const;
    
    /// This function returns size of Orthographic Camera
    float GetOrthographicSize() const;
    /// This function returns the Near Plane value
    float GetOrthographicNearClip() const;
    /// This function returns the Far Plane value
    float GetOrthographicFarClip() const;
    
    /// This function returns the Width of camera
    uint32_t GetWidth() const;
    /// This function returns the Height of camera
    uint32_t GetHeight() const;
    /// This function returns the Aspect Ratio of camera
    float GetAspectRatio() const;
    
    /// This function returns the Projection type of camera
    ProjectionType GetProjectionType() const;
    
  private:
    // Member functions
    /// This function recalculate the projection matrix
    void RecalculateProjection();
    
    // Member variables
    ProjectionType m_projectionType {ProjectionType::Perspective};
    
    uint32_t m_viewportWidth {1600}, m_viewportHeight {900};
    
    float m_degPerspectiveFOV {45.0f};
    float m_perspectiveNear {0.1f}, m_perspectiveFar {1000.0f};
    
    float m_orthographicSize {10.0f};
    float m_orthographicNear {-1.0f}, m_orthographicFar {1.0f};
  };
} // namespace IKan
