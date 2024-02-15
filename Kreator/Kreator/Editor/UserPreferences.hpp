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
    std::string name {};
    std::filesystem::path filePath {};
    time_t lastOpened;
  };
  
  struct UserPreferences
  {
    bool showWelcomeScreen {true};
    enum class Theme
    {
      Dark
    };
    Theme theme {Theme::Dark};
    
    std::filesystem::path startupProject {};
    std::map<time_t, RecentProject, std::greater<time_t>> recentProjects;
    
    // Not Serialized
    std::filesystem::path filePath {};
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
