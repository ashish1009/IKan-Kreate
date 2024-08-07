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
#include <fstream>

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
#include <ImGuizmo.h>

// YAML
#include <yaml-cpp/yaml.h>

// Engine Files
#include <Base/Configurations.h>
#include <Base/AssertAPIs.h>
#include <Base/DesignHelper.h>
#include <Base/Buffer.hpp>

#include <Debug/Logger.hpp>
#include <Debug/LoggerSpecificationBuilder.hpp>
#include <Debug/LoggerAPIs.h>
#include <Debug/Timer.hpp>

#include <Utils/FileSystemUtils.hpp>
#include <Utils/StringUtils.hpp>
#include <Utils/MathUtils.hpp>

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
#include <Core/AABB.hpp>
#include <Core/UUID.hpp>
#include <Core/HashGenerator.hpp>
#include <Core/Ray.hpp>

#include <UI/ImGuiLayer.hpp>

#include <Renderer/Renderer.hpp>
#include <Renderer/RendererStats.hpp>
#include <Renderer/Renderer2D.hpp>
#include <Renderer/Mesh.hpp>
#include <Renderer/Materials.hpp>
#include <Renderer/SceneRenderer.hpp>

#include <Camera/Camera.hpp>
#include <Camera/SceneCamera.hpp>
#include <Camera/EditorCamera.hpp>
#include <Camera/FixedCamera.hpp>

#include <Assets/AssetType.hpp>
#include <Assets/Asset.hpp>
#include <Assets/AssetMetadata.hpp>
#include <Assets/AssetRegistry.hpp>
#include <Assets/AssetManager.hpp>

#include <Project/ProjectConfig.hpp>
#include <Project/Project.hpp>
#include <Project/ProjectSerializer.hpp>

#include <Editor/EditorPanel.hpp>
#include <Editor/EditorPanelManager.hpp>
#include <Editor/AssetEditor.hpp>
#include <Editor/Console/EditorConsoleMessage.hpp>
#include <Editor/Console/EditorConsolePanel.hpp>
#include <Editor/Console/EditorConsoleSink.hpp>

#include <Scene/Scene.hpp>
#include <Scene/Entity.hpp>
#include <Scene/Component.hpp>
#include <Scene/SceneSerializer.hpp>
#include <Scene/EntitySerializer.hpp>
#include <Scene/ECS_Utils.hpp>
