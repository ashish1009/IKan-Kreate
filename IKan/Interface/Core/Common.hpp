//
//  Common.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

namespace IKan
{
  // Typedefs --------------------------------------------------------------------------------------------------------
  template<typename T> using Scope = std::unique_ptr<T>;
  template<typename T, typename ... Args> constexpr Scope<T>
  CreateScope(Args&& ... args)
  {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }
  
  template<typename T> using Ref = std::shared_ptr<T>;
  template<typename T, typename ... Args> constexpr Ref<T>
  CreateRef(Args&& ... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }
} // namespace IKan
