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
  
  /// This structure stores the project user preference for Application
  struct UserPreferences
  {
    bool showWelcomeScreen = true;
    std::string startupProject;
    std::map<time_t, RecentProject, std::greater<time_t>> recentProjects;
    
    // Theme ...
    // .....
    // Some Settings ?
    
    // Not Serialized
    std::string filePath;
  };
  
  
} // namespace IKan
