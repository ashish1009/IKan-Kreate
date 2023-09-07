//
//  EntitySelectionContext.cpp
//  Kreator
//
//  Created by Ashish . on 07/09/23.
//

#include "EntitySelectionContext.hpp"

namespace Kreator
{
  bool SelectionContext::Find(Entity entity)
  {
    for (const auto& e : selections)
    {
      if (e == entity)
      {
        return true;
      }
    }
    return false;
  }
  void SelectionContext::Erase(Entity entity)
  {
    auto it = std::find(selections.begin(), selections.end(), entity);
    if (it != selections.end())
    {
      selections.erase(it);
    }
  }
  void SelectionContext::PushBack(Entity entity)
  {
    selections.push_back(entity);
  }
  void SelectionContext::Clear()
  {
    selections.clear();
  }
  Entity& SelectionContext::operator[](size_t index)
  {
    return selections[index];
  }
  Entity& SelectionContext::At(size_t index)
  {
    return selections.at(index);
  }
  size_t SelectionContext::Size() const
  {
    return selections.size();
  }

} // namespace Kreator
