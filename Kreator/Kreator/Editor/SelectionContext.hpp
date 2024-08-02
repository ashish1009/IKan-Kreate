//
//  SelectionContext.hpp
//  Kreator
//
//  Created by Ashish . on 02/08/24.
//

#pragma once

namespace Kreator
{
  /// This structure store selected entities
  struct SelectionContext : public VectorIterator<Entity>
  {
    /// Default constructor
    SelectionContext();
    
    // Member Variable
    std::vector<Entity> selections;
    
    // Member functions
    /// This function search the entity from container
    /// - Parameter entity: entity
    bool Find(const Entity& entity) const;
    /// This function erases the entity from container
    /// - Parameter entity: entity
    void Erase(const Entity& entity);
    /// This function insert the entity from container
    /// - Parameter entity: entity
    void PushBack(const Entity& entity);
    /// This function clears all the entity
    void Clear();
    /// This function returns the entity at index
    /// - Parameter index: index
    Entity& operator[](size_t index);
    /// This function returns the entity at index
    /// - Parameter index: index
    Entity& At(size_t index);
    /// This function returns the size
    size_t Size() const;
  };
} // namespace Kreator
