//
//  SceneSerializer.cpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#include <yaml-cpp/yaml.h>
#include "SceneSerializer.hpp"
#include "Scene/Entity.hpp"
#include "Utils/YAMLSerializerHelper.h"

namespace IKan {
  
  SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
  : m_scene(scene)
  {
  }
  
  void SceneSerializer::Serialize(const std::string& filepath)
  {
    IK_LOG_TRACE(LogModule::SceneSerializer, "Serializing the Scene {0}", Utils::String::GetFileNameFromPath(filepath));
  }
  
  bool SceneSerializer::Deserialize(const std::string& filepath)
  {
    return true;
  }  
} // namespace IKan
