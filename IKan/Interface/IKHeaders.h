//
//  IKHeaders.h
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#pragma once

// To remove documentation warning in entt::entity
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wformat-security"

// C++ Files
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <map>

// glm math library
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

// Engine Files
#include <Base/Configurations.h>
#include <Base/DesignHelper.h>

#include <Debug/Logger.hpp>
#include <Debug/LoggerBuilder.hpp>
#include <Debug/Timer.hpp>

#include <Core/AssertAPI.h>
#include <Core/TimeStep.hpp>
#include <Core/Application.hpp>
#include <Core/Layer.hpp>
#include <Core/Window.hpp>

#include <Renderer/Renderer.hpp>
