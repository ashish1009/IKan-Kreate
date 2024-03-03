//
//  Prefabs.cpp
//  IKan
//
//  Created by Ashish . on 03/03/24.
//

#include "Prefabs.hpp"
#include "Scene/Components.hpp"
#include "Asset/AssetImporter.hpp"
#include "Asset/AssetManager.hpp"

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
    m_entity = CreatePrefabFromEntity(entity);
    
    if (serialize)
    {
      AssetImporter::Serialize(AssetManager::GetAsset<Prefab>(handle));
    }
  }
  
  Entity Prefab::CreatePrefabFromEntity(Entity entity)
  {
    Entity newEntity = m_scene->CreateEntity("PrefabEntity");
    newEntity.AddComponent<PrefabComponent>(handle, newEntity.GetComponent<IDComponent>().ID);
    
    CopyComponentIfExists<TagComponent>(newEntity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<TransformComponent>(newEntity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<CameraComponent>(newEntity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<MeshComponent>(newEntity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<RigidBodyComponent>(newEntity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<Box3DColliderComponent>(newEntity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<SphereColliderComponent>(newEntity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<CapsuleColliderComponent>(newEntity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    CopyComponentIfExists<JointComponent>(newEntity, m_scene->m_registry, entity, entity.m_scene->m_registry);
    
    for (auto childId : entity.Children())
    {
      Entity childDuplicate = CreatePrefabFromEntity(entity.m_scene->GetEntityWithUUID(childId));
      
      childDuplicate.SetParentUUID(newEntity.GetUUID());
      newEntity.Children().push_back(childDuplicate.GetUUID());
    }
    return newEntity;
  }
} // namespace IKan
