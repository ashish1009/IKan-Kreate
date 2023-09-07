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
    /// This function returns the size
    size_t Size() const;
    
    // Iterators ---------------------------------------------------------------------------------------------------
    std::vector<Entity>::iterator begin() { return selections.begin(); }
    std::vector<Entity>::iterator end() { return selections.end(); }
    std::vector<Entity>::reverse_iterator rbegin() { return selections.rbegin(); }
    std::vector<Entity>::reverse_iterator rend() { return selections.rend(); }
    
    std::vector<Entity>::const_iterator begin() const { return selections.begin(); }
    std::vector<Entity>::const_iterator end() const { return selections.end(); }
    std::vector<Entity>::const_reverse_iterator rbegin() const { return selections.rbegin(); }
    std::vector<Entity>::const_reverse_iterator rend() const { return selections.rend(); }
  };

} // namespace Kreator
