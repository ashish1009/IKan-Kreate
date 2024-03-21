//
//  CameraController.hpp
//  IKan
//
//  Created by Ashish . on 03/03/24.
//

#pragma once

#include "Scene/Entity.hpp"

namespace IKan
{
  class Scene;
  
  class CameraController
  {
  public:
    struct Orbit
    {
      float radius;
      float height;
    };
    
    enum class ViewType
    {
      TPP, FPP, FlyCam
    };
    
    /// Default constructor
    CameraController();
    
    /// Copy constructor to avoid copy of entity from edit to runtime scene
    /// - Parameter other: other controller
    /// - Note: We are not copying the entity and scene as we need to initialize this again for different scenes
    ///         (Edit scene and runtime scene)
    CameraController(const CameraController& other);
    
    /// This funciton initialize the camera controller
    /// - Parameters:
    ///   - entity: Entity of camera
    ///   - scene: scene pointer
    void Initialize(Entity entity, Scene* scene);
    /// This funcion updates the controller
    /// - Parameter ts: time step
    void OnUpdate(TimeStep ts);
    /// This funcion handles the events in camera controller
    /// - Parameter event: event
    void EventHandler(Event& event);
    
    /// This funcion reset the view of camera
    void ResetView();
    
    /// This funcion sets the Follow entity
    void SetFollowEntity(UUID uuid);
    
    /// This funcion returns the Follow entity
    UUID GetFollowEntity() const;
    
    /// This funcion returns the entity
    const Entity& GetEntity() const;
    
    /// This function returns the  front vector of camera
    const glm::vec3& GetFrontVector() const;
    /// This function returns the  uo vector of camera
    const glm::vec3& GetUpVector() const;
    /// This function returns the  right vector of camera
    const glm::vec3& GetRightVector() const;
    /// This function returns the  position of camera
    const glm::vec3& GetPosition() const;
    
    /// This function returns the top radius
    const Orbit& GetTopOrbit() const;
    /// This function returns the mid radius
    const Orbit& GetMidOrbit() const;
    /// This function returns the bottom radius
    const Orbit& GetBottomOrbit() const;
    
    /// This function returns the SetSensitivity
    float GetSensitivity() const;
    
    /// This function returns the CameraViewType
    ViewType GetCameraViewType() const;
    
    /// This function sets the top radius
    /// - Parameter value: Orbit value
    void SetTopOrbit(const Orbit& value);
    /// This function sets the mid radius
    /// - Parameter value: Orbit value
    void SetMidOrbit(const Orbit& value);
    /// This function sets the bottom radius
    /// - Parameter value: Orbit value
    void SetBottomOrbit(const Orbit& value);
    
    /// This function sets the SetSensitivity
    /// - Parameter value: sensitivity value
    void SetSensitivity(float value);
    
    /// This function sets the camera view type
    /// - Parameter type: type
    void SetCameraViewType(ViewType type);
    
  private:
    /// This function updates the vector of camera
    void UpdateVectors();
    
    // Data to be copied
    UUID m_followEntity;
    
    Orbit m_topOrbit {3, 1}, m_midOrbit {5, 0.5}, m_bottomOrbit {4, 0};
    
    glm::vec2 m_centerPosition {};
    glm::vec2 m_windowSize {};
    float m_halfWindowHeight = 0.0f;
    
    // No need to copy, gets initialized for each scene
    Scene* m_scene;
    Entity m_entity {};
    
    // No need to copy, gets updated in runtime
    glm::vec3 m_position = {0, 0, 0};
    glm::vec3 m_frontVector = {0.0f, 0.0f, -1.0f};
    glm::vec3 m_upVector = {0.0f, 1.0f, 0.0f};
    glm::vec3 m_worldUpVector = {0.0f, 1.0f, 0.0f};
    glm::vec3 m_rightVector;
    
    float m_anglePerMouseMoveX {};
    float m_sensitivity = 45.0f;
    
    ViewType m_cameraViewType {ViewType::TPP};
  };
} // namespace IKan
