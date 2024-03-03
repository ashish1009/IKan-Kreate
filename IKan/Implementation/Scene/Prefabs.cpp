//
//  Prefabs.cpp
//  IKan
//
//  Created by Ashish . on 03/03/24.
//

#include "Prefabs.hpp"
#include "Scene/Components.hpp"
#include "Asset/AssetImporter.hpp"

namespace IKan
{
  template<typename T>
  static void CopyComponentIfExists(entt::entity dst, entt::registry& dstRegistry, entt::entity src, entt::registry& srcRegistry)
  {
    if (srcRegistry.has<T>(src))
    {
      auto& srcComponent = srcRegistry.get<T>(src);
      dstRegistry.emplace_or_replace<T>(dst, srcComponent);
    }
  }
  
  Ref<Prefab> Prefab::Create()
  {
    return CreateRef<Prefab>();
  }

  Prefab::Prefab()
  {
    m_scene = Scene::Create();
  }
  
  Prefab::~Prefab()
  {
    m_scene.reset();
  }

  void Prefab::Create(Entity entity, bool serialize)
  {
    m_entity = m_scene->CreateEntity("PrefabEntity");
    m_entity.AddComponent<PrefabComponent>(handle, m_entity.GetComponent<IDComponent>().ID);
    
    CopyComponentIfExists<TagComponent>(m_entity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<TransformComponent>(m_entity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<CameraComponent>(m_entity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<MeshComponent>(m_entity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<RigidBodyComponent>(m_entity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<Box3DColliderComponent>(m_entity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<SphereColliderComponent>(m_entity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<CapsuleColliderComponent>(m_entity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<JointComponent>(m_entity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    
    for (auto childId : entity.Children())
    {
      Entity childDuplicate = CreatePrefabFromEntity(entity.m_scene->GetEntityWithUUID(childId));
      
      childDuplicate.SetParentUUID(m_entity.GetUUID());
      m_entity.Children().push_back(childDuplicate.GetUUID());
    }
    
    if (serialize)
    {
      AssetImporter::Serialize(Ref<Prefab>(this));
    }
  }
} // namespace IKan
