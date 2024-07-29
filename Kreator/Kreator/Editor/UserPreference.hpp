//
//  UserPreference.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

using namespace IKan;

namespace Kreator
{
  /// This structure stores the recent project data
  struct RecentProject
  {
    std::string name {};
    std::filesystem::path filePath {};
    time_t lastOpened;
  };
  
  /// This structure stores the user preference data
  struct UserPreferences
  {
    bool showWelcomeScreen {true};
    enum class Theme
    {
      Blue
    };
    Theme theme {Theme::Blue};
    
    std::filesystem::path startupProject {};
    std::map<time_t, RecentProject, std::greater<time_t>> recentProjects;
    
    // Not Serialized
    std::filesystem::path filePath {};
  };
  
  /// This class serialises and deserialises the user preference data
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
