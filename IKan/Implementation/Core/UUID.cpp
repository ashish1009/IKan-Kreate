//
//  UUID.cpp
//  IKan
//
//  Created by Ashish . on 17/07/24.
//

#include "UUID.hpp"

#include <random>

namespace IKan
{
  static std::random_device s_RandomDevice;
  static std::mt19937_64 eng(s_RandomDevice());
  static std::uniform_int_distribution<uint64_t> s_UniformDistribution;
  
  static std::mt19937 eng32(s_RandomDevice());
  static std::uniform_int_distribution<uint32_t> s_UniformDistribution32;
  
  UUID::UUID()
  : uuid_(s_UniformDistribution(eng))
  {
    
  }
  UUID::UUID(uint64_t uuid)
  : uuid_(uuid)
  {
    
  }
  UUID::UUID(const UUID& other)
  : uuid_(other.uuid_)
  {
    
  }
  
  UUID32::UUID32()
  : uuid_(s_UniformDistribution32(eng32))
  {
    
  }
  UUID32::UUID32(uint32_t uuid)
  : uuid_(uuid)
  {
    
  }
  UUID32::UUID32(const UUID32& other)
  : uuid_(other.uuid_)
  {
    
  }
} // namespace IKan
