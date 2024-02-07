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
