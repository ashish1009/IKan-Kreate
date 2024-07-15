//
//  Camera.hpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#pragma once

namespace IKan
{
  /// This class is the Base class of camera. Both the cameras should derived from this base class
  class Camera
  {
  public:
    /// Default camera constructor
    Camera() = default;
    /// This constructor creates the camera camera with projection matrixes
    /// - Parameters:
    ///   - projection: Camera projection matrix
    ///   - unReversedProjection: un reversed projection matrix
    Camera(const glm::mat4& projection, const glm::mat4& unReversedProjection);
    
    // Setters -------------------------------------------------------------------------------------------------------
    /// This function updates the projection metrix of camera
    /// - Parameters:
    ///   - projection: New Projection value
    ///   - unReversedProjection: Un reversed projection matrix value
    void SetProjectionMatrix(const glm::mat4& projection, const glm::mat4& unReversedProjection);
    /// This function updates the projection metrix of camera
    /// - Parameters:
    ///   - degFov: Degree of FOV
    ///   - width: Camera width
    ///   - height: Camera Height
    ///   - nearP: Near Plane
    ///   - farP: Far Plane
    void SetPerspectiveProjectionMatrix(float radFov, float width, float height, float nearP, float farP);
    /// This function updates the projection metrix of camera
    /// - Parameters:
    ///   - width: Camera width
    ///   - height: Camera Height
    ///   - nearP: Near Plane
    ///   - farP: Far Plane
    void SetOrthoProjectionMatrix(float width, float height, float nearP, float farP);
    
    // Getters -------------------------------------------------------------------------------------------------------
    /// This function returns the Projection Matrix of Camera
    const glm::mat4& GetProjectionMatrix() const;
    /// This function returns the Un Reversed Projection Matrix of Camera
    const glm::mat4& GetUnReversedProjectionMatrix() const;
    
  private:
    glm::mat4 m_projectionMatrix {Utils::Math::UnitMat4};
    glm::mat4 m_unReversedProjectionMatrix {Utils::Math::UnitMat4};
  };
} // namespace IKan
