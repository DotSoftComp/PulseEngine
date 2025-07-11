/**
 * @file common.h
 * @author Dorian LEXTERIAQUE (dlexteriaque@gmail.com)
 * @brief Common.h - Shared includes, types, and configuration flags for the engine
 * @brief This header centralizes commonly used includes and definitions across
 * @brief the engine and editor builds. It helps to avoid redundancy and ensures
 * @brief consistent access to shared utilities (e.g., math types, rendering APIs).
 * @version 0.1
 * @date 2025-06-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef COMMON_H
#define COMMON_H

// --------------------
// Standard Library Includes
// --------------------
#include <cstddef>      // for std::size_t
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// --------------------
// Engine Math Utilities
// --------------------
#include "PulseEngine/core/Math/Vector.h"
#include "PulseEngine/core/Math/Mat4.h"
#include "PulseEngine/core/Math/Mat3.h"
#include "PulseEngine/core/Math/Color.h"

// --------------------
// Engine Core Includes
// --------------------
#include "shader.h"
#include "camera.h"
#include "PulseEngine/core/Material/Texture.h"
#include "PulseEngine/ModuleLoader/ModuleLoader.h"
#include "PulseEngine/ModuleLoader/IModule/IModule.h"

// --------------------
// Platform-Specific Includes
// --------------------
#include <Windows.h>    // Windows API access (window creation, dialogs, etc.)

// --------------------
// Editor/Build Mode Flags
// --------------------
#include "Common/EditorDefines.h"

// --------------------
// Graphics Includes (only in engine or editor build with OpenGL)
// --------------------
#ifdef WINDOW_PULSE_EXPORT
    #include <glad.h>                       // OpenGL function loader
    #include <GLFW/glfw3.h>                 // Cross-platform windowing/input
    #include <glm/glm.hpp>                  // GLM core
    #include <glm/gtc/type_ptr.hpp>         // GLM pointer helpers
    #include <glm/gtc/matrix_transform.hpp> // GLM transformation helpers
    #include <GL/gl.h>                      // OpenGL headers
#endif

// --------------------
// Editor UI Integration (ImGui)
// --------------------
#ifdef ENGINE_EDITOR
    #include "imgui.h"
    #include "backends/imgui_impl_glfw.h"
    #include "backends/imgui_impl_opengl3.h"
#endif

#include "PulseEngine/core/WindowContext/WindowContext.h"

// --------------------
// Asset Path Configuration
// --------------------
#ifdef ENGINE_EDITOR
    #define ASSET_PATH "PulseEngineEditor/"  // Editor-specific asset path
#else
    #define ASSET_PATH "assets/"             // Runtime asset path
#endif

// --------------------
// Engine Types
// --------------------
// Globally unique identifier type used across the engine
using GUID_PULSE = std::size_t;

#endif // COMMON_H
