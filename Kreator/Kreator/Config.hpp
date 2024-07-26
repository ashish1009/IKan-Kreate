//
//  Config.hpp
//  Kreator
//
//  Created by Ashish . on 26/07/24.
//

#pragma once

namespace Kreator
{
  /// This strcture stores the Kreator client paths
  struct KreatorDirectories
  {
    std::filesystem::path systemUserPath{};
    std::filesystem::path kreatorPath{};
    std::filesystem::path clientResourcePath{};
  };
} // namespace Kreator
