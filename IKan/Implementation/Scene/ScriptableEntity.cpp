//
//  ScriptableEntity.cpp
//  IKan
//
//  Created by Ashish . on 23/04/24.
//

#include "ScriptableEntity.hpp"
#include "Scene/Components.hpp"

namespace IKan {
  
  void ScriptableEntity::Create(Entity entity)
  {
    m_entity = entity;
  }
  
  void ScriptManager::UpdateScript(NativeScriptComponent* sc, const std::string& scriptName)
  {
    ScriptLoaderFn loaderFuncion = nullptr;
    if (s_scripts.find(scriptName) != s_scripts.end())
    {
      loaderFuncion = s_scripts.at(scriptName);
    }
    if (loaderFuncion)
    {
      IK_ASSERT(loaderFuncion(sc, scriptName), "Invalid Script name");
    }
    else
    {
      sc->Bind<IKan::ScriptableEntity>();
    }
  }
  
  void ScriptManager::LoadScript(const std::string& scriptName, ScriptLoaderFn loaderFunction)
  {
    if (s_scripts.find(scriptName) == s_scripts.end())
    {
      s_scripts[scriptName] = loaderFunction;
    }
  }
  
  const std::unordered_map<std::string, ScriptLoaderFn>& ScriptManager::GetScripts()
  {
    return s_scripts;
  }
} // namespace IKan
