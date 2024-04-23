//
//  ContactListener.cpp
//  IKan
//
//  Created by Ashish . on 02/03/24.
//

#include "ContactListener.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components.hpp"

using namespace reactphysics3d;

namespace IKan
{
  void ContactListener::onContact(const CollisionCallback::CallbackData& callbackData)
  {
    IK_PERFORMANCE("ContactListener::onContact");
    // For each contact pair
    for (uint8_t pairIdx = 0; pairIdx < callbackData.getNbContactPairs(); pairIdx++)
    {
      // Get the contact pair
      CollisionCallback::ContactPair contactPair = callbackData.getContactPair(pairIdx);
      
      // For each contact point of the contact pair
      for (uint8_t contactPointIdx = 0; contactPointIdx < contactPair.getNbContactPoints(); contactPointIdx++)
      {
        // Get the contact point
        CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(contactPointIdx);
        
        // Get the Collided Entities
        IKan::Entity* entityA = (IKan::Entity*)(contactPair.getCollider1()->getUserData());
        IKan::Entity* entityB = (IKan::Entity*)(contactPair.getCollider2()->getUserData());
               
        // Collider callback for Entity A
        if (entityA)
        {
          if (entityA->HasComponent<NativeScriptComponent>())
          {
            entityA->GetComponent<NativeScriptComponent>().script->OnContact(entityB, contactPoint);
          }
        }
        
        // Collider callback for Entity B
        if (entityB)
        {
          if (entityB->HasComponent<NativeScriptComponent>())
          {
            entityB->GetComponent<NativeScriptComponent>().script->OnContact(entityA, contactPoint);
          }
        }

        // Get the contact point on the first collider and convert it in world-space
        [[maybe_unused]] Vector3 worldPoint = contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();
      }
    }
  }
  void ContactListener::onTrigger(const OverlapCallback::CallbackData& callbackData)
  {
    IK_PERFORMANCE("ContactListener::onTrigger");
  }
} // namespace IKan
