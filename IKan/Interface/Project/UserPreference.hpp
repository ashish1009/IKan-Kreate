//
//  UserPreference.hpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#pragma once

namespace IKan
{
  /// This structure stores the recent project data
  struct RecentProject
  {
    std::string name;
    std::string filePath;
    time_t lastOpened;
  };
} // namespace IKan
