//
//  UserPreferences.hpp
//  Kreator
//
//  Created by Ashish . on 15/02/24.
//

#pragma once

using namespace IKan;

namespace Kreator
{
  /// This structure stores the recent project data
  struct RecentProject
  {
    std::string name;
    std::filesystem::path filePath;
    time_t lastOpened;
  };
  
  struct UserPreferences
  {
    bool showWelcomeScreen = true;
    enum class Theme
    {
      Dark
    };
    Theme theme = Theme::Dark;
    
    std::string startupProject;
    std::map<time_t, RecentProject, std::greater<time_t>> recentProjects;
    
    // Not Serialized
    std::string filePath;
  };

} // namespace Kreator
