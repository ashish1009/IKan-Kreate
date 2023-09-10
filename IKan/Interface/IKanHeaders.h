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
#include <unordered_set>

// glm math library
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// ImGui files
#include <imgui.h>
#include <ImGuizmo.h>
#include <imgui_internal.h>

// Engine Files
/// Shoule be included at top only ----------------------------------------------------------------------------------
#include <Core/Macros.h>
#include <Core/Common.hpp>
/// -----------------------------------------------------------------------------------------------------------------

#include <Utils/FileSystem.hpp>
#include <Utils/StringUtils.hpp>
#include <Utils/MathUtils.hpp>
#include <Utils/YAMLSerializerHelper.h>

#include <Debug/Logger.hpp>
#include <Debug/Timer.hpp>
#include <Debug/Table.hpp>

#include <Core/AssertAPIs.h>
#include <Core/KeyCodes.h>
#include <Core/MouseCodes.h>
#include <Core/TimeStep.hpp>
#include <Core/Buffers.hpp>
#include <Core/Application.hpp>
#include <Core/CoreWindow.hpp>
#include <Core/Input.hpp>
#include <Core/Layer.hpp>
#include <Core/UUID.hpp>
#include <Core/HashGenerator.hpp>
#include <Core/IKMemory.hpp>
#include <Core/Ray.hpp>

#include <Event/Event.h>
#include <Event/ApplicationEvent.h>
#include <Event/MouseEvent.h>
#include <Event/KeyEvent.h>

#include <Renderer/Renderer.hpp>
#include <Renderer/TextRenderer.hpp>
#include <Renderer/Renderer2D.hpp>
#include <Renderer/SceneRenderer.hpp>
#include <Renderer/RendererStats.hpp>
#include <Renderer/Mesh.hpp>
#include <Renderer/Material.hpp>
#include <Renderer/UI/Font.hpp>

#include <Renderer/Graphics/Shader.hpp>
#include <Renderer/Graphics/RendererBuffers.hpp>
#include <Renderer/Graphics/Pipeline.hpp>
#include <Renderer/Graphics/Texture.hpp>
#include <Renderer/Graphics/FrameBuffer.hpp>
#include <Renderer/Graphics/RenderPass.hpp>

#include <Camera/Camera.hpp>
#include <Camera/SceneCamera.hpp>
#include <Camera/EditorCamera.hpp>
#include <Camera/FixedCamera.hpp>

#include <Scene/Entity.hpp>
#include <Scene/Component.hpp>
#include <Scene/Scene.hpp>
#include <Scene/SceneSerializer.hpp>

#include <Asset/AssetType.hpp>
#include <Asset/Asset.hpp>
#include <Asset/AssetMetadata.hpp>
#include <Asset/AssetImporter.hpp>
#include <Asset/AssetSerializer.hpp>
#include <Asset/AssetManager.hpp>

#include <Project/Project.hpp>
#include <Project/ProjectSerializer.hpp>

#include <UI/ImGuiLayer.hpp>
#include <UI/ImGuiTheme.hpp>
#include <UI/ImGuiUtils.hpp>
#include <UI/ImGuiScoped.hpp>
#include <UI/ImGuiCustom.hpp>
#include <UI/UI_Buffer.hpp>

#include <Editor/EditorPanel.hpp>
#include <Editor/PanelManager.hpp>
#include <Editor/EditorConsolePanel.hpp>
#include <Editor/AssetEdtitor.hpp>

#include <Scene/Scene.hpp>
#include <Scene/SceneSerializer.hpp>
#include <Scene/Entity.hpp>
#include <Scene/Component.hpp>

#include <Physics/3D/PhysicsScene.hpp>
