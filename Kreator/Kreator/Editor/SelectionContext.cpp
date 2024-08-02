//
//  SelectionContext.cpp
//  Kreator
//
//  Created by Ashish . on 02/08/24.
//

#include "SelectionContext.hpp"

namespace Kreator
{
  SelectionContext::SelectionContext()
  : VectorIterator<Entity>(selections)
  {
    IK_PROFILE();
  }
  
  bool SelectionContext::Find(const Entity& entity) const
  {
    IK_PROFILE();
    for (const auto& e : selections)
    {
      if (e == entity)
      {
        return true;
      }
    }
    return false;
  }
  void SelectionContext::Erase(const Entity& entity)
  {
    IK_PROFILE();
    auto it = std::find(selections.begin(), selections.end(), entity);
    if (it != selections.end())
    {
      selections.erase(it);
    }
  }
  void SelectionContext::PushBack(const Entity& entity)
  {
    selections.push_back(entity);
  }
  void SelectionContext::Clear()
  {
    selections.clear();
  }
  Entity& SelectionContext::operator[](size_t index)
  {
    IK_ASSERT(index >= selections.size() and index < selections.size(), "Index out of range !!!");
    return selections[index];
  }
  Entity& SelectionContext::At(size_t index)
  {
    IK_ASSERT(index >= 0 and index < selections.size(), "Index out of range !!!");
    return selections.at(index);
  }
  size_t SelectionContext::Size() const
  {
    return selections.size();
  }
} // namespace Kreator
