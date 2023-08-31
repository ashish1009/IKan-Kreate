//
//  UserPreference.hpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#pragma once

namespace Kreator
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
    enum class Theme
    {
      Default, KreatorDark
    };
    Theme theme = Theme::Default;

    std::string startupProject;
    std::map<time_t, RecentProject, std::greater<time_t>> recentProjects;
    
    // .....
    // Some Settings ?
    
    // Not Serialized
    std::string filePath;
  };
  
  /// This class serialise and deserialise the user preference
  class UserPreferencesSerializer
  {
  public:
    /// Project User preference Serializer constructor
    /// - Parameter preferences: user preference
    UserPreferencesSerializer(const Ref<UserPreferences>& preferences);
    /// Project User preference Serializer Destructor
    ~UserPreferencesSerializer() = default;
    
    /// This function serialize the User prefernce data in file
    /// - Parameter filepath: file path
    void Serialize(const std::filesystem::path& filepath);
    /// This function deserialize the User prefernce data in file
    /// - Parameter filepath: file path
    void Deserialize(const std::filesystem::path& filepath);
    
  private:
    Ref<UserPreferences> m_preferences;
  };
} // namespace Kreator
