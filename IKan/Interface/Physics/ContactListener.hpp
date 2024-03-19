//
//  ContactListener.hpp
//  IKan
//
//  Created by Ashish . on 02/03/24.
//

#pragma once

#include <reactphysics3d/reactphysics3d.h>

namespace IKan
{
  class ContactListener : public reactphysics3d::EventListener
  {
  public:
    /// This function calls when some contacts occur
    /// - Parameter callbackData: Contains information about all the contacts
    void onContact(const reactphysics3d::CollisionCallback::CallbackData& callbackData) override;
    /// This function calls when some trigger events occur
    /// - Parameter callbackData: Contains information about all the triggers that are colliding
    void onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData) override;
  };
} // namespace IKan
