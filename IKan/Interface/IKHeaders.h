//
//  IKHeaders.h
//  IKan
//
//  Created by Ashish . on 05/07/24.
//

#pragma once

// To remove documentation warning in entt::entity
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wformat-security"

// C++ Files
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <map>

// glm math library
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

// UI Vendor files
#include <imgui.h>
#include <imgui_internal.h>

// Engine Files
#include <Base/Configurations.h>
#include <Base/AssertAPIs.h>
#include <Base/DesignHelper.h>

#include <Debug/Logger.hpp>
#include <Debug/LoggerSpecificationBuilder.hpp>
#include <Debug/LoggerAPIs.h>
#include <Debug/Timer.hpp>

#include <Utils/FileSystemUtils.hpp>
#include <Utils/StringUtils.hpp>

#include <Events/Events.h>
#include <Events/ApplicationEvents.h>
#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>

#include <Core/Application.hpp>
#include <Core/MouseCodes.h>
#include <Core/KeyCodes.h>
#include <Core/TimeStep.hpp>
#include <Core/Layer.hpp>
#include <Core/LayerStack.hpp>
#include <Core/Window.hpp>
#include <Core/Input.hpp>

#include <UI/ImGuiLayer.hpp>

#include <Renderer/Renderer.hpp>
