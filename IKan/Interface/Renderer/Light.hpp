//
//  Light.hpp
//  IKan
//
//  Created by Ashish . on 23/02/24.
//

namespace IKan
{
  struct DirectionLight
  {
    float enable = 1.0f;
    float intensity = 1.0f;
    
    glm::vec3 direction = {-0.2, -1, -0.3};
    glm::vec3 color = {1.0f, 1.0f, 1.0f};
  };
} // namespace IKan
