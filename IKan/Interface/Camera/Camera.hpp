//
//  Camera.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace IKan
{
  /// This class is the Base class of camera. Both the cameras should derived from this base class
  class Camera
  {
  public:
    Camera() = default;
    /// Parameter Camera constructor
    /// - Parameters:
    ///   - width: Camera width
    ///   - height: Camera Height
    ///   - nearP: Near Plane
    ///   - farP: Far Plane
    Camera(const float width, const float height, const float nearP, const float farP);
    /// Parameter Camera constructor
    /// - Parameters:
    ///   - projection: Camera projection matrix
    ///   - unReversedProjection: un reversed projection matrix
    Camera(const glm::mat4& projection, const glm::mat4& unReversedProjection);
    /// Parameter Camera constructor
    /// - Parameters:
    ///   - degFov: Degree of FOV
    ///   - width: Camera width
    ///   - height: Camera Height
    ///   - nearP: Near Plane
    ///   - farP: Far Plane
    Camera(const float degFov, const float width, const float height, const float nearP, const float farP);
    
    /// Default Camera destructor
    virtual ~Camera() = default;
    
    // Setters -------------------------------------------------------------------------------------------------------
    /// This function update the projection metrix of camera
    /// - Parameters:
    ///   - projection: New Projection value
    ///   - unReversedProjection: Un reversed projection matrix value
    void SetProjectionMatrix(const glm::mat4 projection, const glm::mat4 unReversedProjection);
    /// This function update the projection metrix of camera
    /// - Parameters:
    ///   - degFov: Degree of FOV
    ///   - width: Camera width
    ///   - height: Camera Height
    ///   - nearP: Near Plane
    ///   - farP: Far Plane
    void SetPerspectiveProjectionMatrix(const float radFov, const float width, const float height,
                                        const float nearP, const float farP);
    /// This function update the projection metrix of camera
    /// - Parameters:
    ///   - width: Camera width
    ///   - height: Camera Height
    ///   - nearP: Near Plane
    ///   - farP: Far Plane
    void SetOrthoProjectionMatrix(const float width, const float height, const float nearP, const float farP);
    
    // Getters -------------------------------------------------------------------------------------------------------
    /// This function returns the Projection Matrix of Camera
    const glm::mat4& GetProjectionMatrix() const;
    /// This function returns the Un Reversed Projection Matrix of Camera
    const glm::mat4& GetUnReversedProjectionMatrix() const;
    
  private:
    glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
    glm::mat4 m_unReversedProjectionMatrix = glm::mat4(1.0f);
  };
} // namespace IKan
