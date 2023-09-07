//
//  EntitySelectionContext.hpp
//  Kreator
//
//  Created by Ashish . on 07/09/23.
//

#pragma once

namespace Kreator
{
  /// Store selected entity
  struct SelectionContext
  {
    // Member Variable
    std::vector<Entity> selections;
    
    // Member functions
    /// This function search the entity from container
    /// - Parameter entity: entity
    bool Find(Entity entity);
    /// This function erases the entity from container
    /// - Parameter entity: entity
    void Erase(Entity entity);
    /// This function insert the entity from container
    /// - Parameter entity: entity
    void PushBack(Entity entity);
    /// This function clears all the entity
    void Clear();
    /// This function returns the entity at index
    /// - Parameter index: index
    Entity& operator[](size_t index);
    /// This function returns the entity at index
    /// - Parameter index: index
    Entity& At(size_t index);
    size_t Size() const;
  };

} // namespace Kreator
