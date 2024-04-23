//
//  ScriptableEntity.hpp
//  IKan
//
//  Created by Ashish . on 23/04/24.
//

#pragma once

#include "Core/TimeStep.hpp"
#include "Scene/Entity.hpp"
#include <reactphysics3d/reactphysics3d.h>

namespace IKan
{
#define ScriptLoader(x, ...) \
[=](NativeScriptComponent* sc, const std::string& script_name) {  \
if (script_name == #x) {                                        \
sc->Bind<x>(__VA_ARGS__);                                     \
return true;                                                  \
}                                                               \
return false;                                                   \
}
  
  class NativeScriptComponent;
  class Entity;
  class Scene;
  
  // Using Typedefs
  using ScriptLoaderFn = std::function<bool(NativeScriptComponent* sc, const std::string& scriptName)>;
  
  class ScriptableEntity
  {
  public:
    virtual void EventHandler(Event& event) {}
    virtual void Copy(void* script) {}
    virtual void RenderGui() {}
    
    virtual void OnContact(Entity* collidedEntity, const reactphysics3d::Vector3& worldContactPoint) {}
    virtual void OnTrigger(Entity* collidedEntity, const reactphysics3d::Vector3& worldContactPoint) {}
    
  protected:
    virtual void Create(Entity entity);
    virtual void Destroy() {}
    virtual void Update(TimeStep ts) {}
    
  protected:
    Entity m_entity;
    Scene* m_scene;
    friend class Scene;
  };
  
  /// This file holds all the native scripts. For now we are using only C type Scripts so we have to pre define all of them.
  /// No Runtime script is supported yet
  class ScriptManager
  {
  public:
    /// This function update the scrip bind function pointer
    /// - Parameters:
    ///   - sc: Native script component pointer
    ///   - scriptName: Script name
    static void UpdateScript(NativeScriptComponent* sc, const std::string& scriptName);
    
    /// This funcion stores the scripts in map to load runtime dynamically
    /// - Parameters:
    ///   - scriptName: script name (class name)
    ///   - loaderFunction: Script loader funcion
    static void LoadScript(const std::string& scriptName, ScriptLoaderFn loaderFunction);
    
    /// This funciton returns the scripts map
    static const std::unordered_map<std::string, ScriptLoaderFn>& GetScripts();
    
    DELETE_ALL_CONSTRUCTORS(ScriptManager);
    
  private:
    inline static std::unordered_map<std::string, ScriptLoaderFn> s_scripts;
  };
  
} // namespace IKan
