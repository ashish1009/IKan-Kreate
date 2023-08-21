//
//  MeshFactory.cpp
//  IKan
//
//  Created by Ashish . on 21/08/23.
//

#include "MeshFactory.hpp"
#include "Asset/AssetManager.hpp"

namespace IKan
{
  AssetHandle MeshFactory::CreateBox(const glm::vec3& size)
  {
    std::vector<StaticVertex> vertices;
    vertices.resize(8);
    vertices[0].position = { -size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
    vertices[1].position = {  size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
    vertices[2].position = {  size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
    vertices[3].position = { -size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
    vertices[4].position = { -size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
    vertices[5].position = {  size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
    vertices[6].position = {  size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };
    vertices[7].position = { -size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };
    
    vertices[0].normal = { -1.0f, -1.0f,  1.0f };
    vertices[1].normal = {  1.0f, -1.0f,  1.0f };
    vertices[2].normal = {  1.0f,  1.0f,  1.0f };
    vertices[3].normal = { -1.0f,  1.0f,  1.0f };
    vertices[4].normal = { -1.0f, -1.0f, -1.0f };
    vertices[5].normal = {  1.0f, -1.0f, -1.0f };
    vertices[6].normal = {  1.0f,  1.0f, -1.0f };
    vertices[7].normal = { -1.0f,  1.0f, -1.0f };
    
    std::vector<Index> indices;
    indices.resize(12);
    indices[0] =  { 0, 1, 2 };
    indices[1] =  { 2, 3, 0 };
    indices[2] =  { 1, 5, 6 };
    indices[3] =  { 6, 2, 1 };
    indices[4] =  { 7, 6, 5 };
    indices[5] =  { 5, 4, 7 };
    indices[6] =  { 4, 0, 3 };
    indices[7] =  { 3, 7, 4 };
    indices[8] =  { 4, 5, 1 };
    indices[9] =  { 1, 0, 4 };
    indices[10] = { 3, 2, 6 };
    indices[11] = { 6, 7, 3 };
    
    return AssetManager::CreateMemoryOnlyAsset<MeshSource>(vertices, indices, glm::mat4(1.0f));
  }
  
  AssetHandle MeshFactory::CreateSphere(float radius)
  {
    std::vector<StaticVertex> vertices;
    std::vector<Index> indices;
    
    constexpr float latitudeBands = 30;
    constexpr float longitudeBands = 30;
    
    for (float latitude = 0.0f; latitude <= latitudeBands; latitude++)
    {
      const float theta = latitude * (float)M_PI / latitudeBands;
      const float sinTheta = glm::sin(theta);
      const float cosTheta = glm::cos(theta);
      
      for (float longitude = 0.0f; longitude <= longitudeBands; longitude++)
      {
        const float phi = longitude * 2.f * (float)M_PI / longitudeBands;
        const float sinPhi = glm::sin(phi);
        const float cosPhi = glm::cos(phi);
        
        StaticVertex vertex;
        vertex.normal = { cosPhi * sinTheta, cosTheta, sinPhi * sinTheta };
        vertex.position = { radius * vertex.normal.x, radius * vertex.normal.y, radius * vertex.normal.z };
        vertices.push_back(vertex);
      }
    }
    
    for (uint32_t latitude = 0; latitude < (uint32_t)latitudeBands; latitude++)
    {
      for (uint32_t longitude = 0; longitude < (uint32_t)longitudeBands; longitude++)
      {
        const uint32_t first = (latitude * ((uint32_t)longitudeBands + 1)) + longitude;
        const uint32_t second = first + (uint32_t)longitudeBands + 1;
        
        indices.push_back({ first, second, first + 1 });
        indices.push_back({ second, second + 1, first + 1 });
      }
    }
    
    return AssetManager::CreateMemoryOnlyAsset<MeshSource>(vertices, indices, glm::mat4(1.0f));
  }
  
  static void CalculateRing(size_t segments, float radius, float y, float dy, float height, float actualRadius,
                            std::vector<StaticVertex>& vertices)
  {
  }
  
  AssetHandle MeshFactory::CreateCapsule(float radius, float height)
  {
    return AssetHandle();
  }
} // namespace IKan
