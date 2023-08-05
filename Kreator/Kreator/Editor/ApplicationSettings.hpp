//
//  ApplicationSettings.hpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#pragma once

namespace Kreator
{
  /// This structure stores the applications settings
  struct ApplicationSettings
  {
    bool ContentBrowserShowAssetTypes = true;
    int ContentBrowserThumbnailSize = 64;
    
    static ApplicationSettings& Get();
  };
  
  /// This structure serialise and deserialise the applications settings
  class ApplicationSettingsSerializer
  {
  public:
    /// This funciton Initilaises the serialiser
    static void Initialize();
    /// This funciton Loads the settings
    static void LoadSettings();
    /// This funciton Saves the settings
    static void SaveSettings();
  };
} // namespace Kreator
