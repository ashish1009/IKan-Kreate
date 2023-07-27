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

// glm math library
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// Engine Files
/// Shoule be included at top only ----------------------------------------------------------------------------------
#include <Core/Macros.h>
#include <Core/Common.hpp>
/// -----------------------------------------------------------------------------------------------------------------

#include <Utils/FileSystem.hpp>

#include <Debug/Logger.hpp>
#include <Debug/Timer.hpp>

#include <Core/AssertAPIs.h>
#include <Core/KeyCodes.h>
#include <Core/MouseCodes.h>

#include <Event/Event.h>
#include <Event/ApplicationEvent.h>
#include <Event/MouseEvent.h>
#include <Event/KeyEvents.h>
