//
//  IKanHeaders.h
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

// This file includes any core headers that should be included in client side. Client should include just one file to
// compile Core engine library

// To remove documentation warning in entt::entity
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wformat-security"

// C++ Files
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <map>

// Engine Files
/// Shoule be included at top only ----------------------------------------------------------------------------------
#include <Core/Macros.h>
#include <Core/Common.hpp>
/// -----------------------------------------------------------------------------------------------------------------

#include <Utils/FileSystem.hpp>

#include <Debug/Logger.hpp>
