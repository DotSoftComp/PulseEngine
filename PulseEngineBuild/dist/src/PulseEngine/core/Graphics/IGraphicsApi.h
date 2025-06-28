/**
 * @file IGraphicsApi.h
 * @author Dorian LEXTERIAQUE (dlexteriaque@gmail.com)
 * @brief The most important part of the Pulse Engine, the graphics API interface.
 * @brief This interface defines all the necessary methods required to initialize, manage, and use a rendering backend for the Pulse engine.
 * @brief It is used by the PulseEngineBackend to communicate with the graphics API.
 * @version 0.1
 * @date 2025-06-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef IGRAPHICSAPI_H
#define IGRAPHICSAPI_H

#include "Common/common.h"
#include "Common/dllExport.h"
#include <string>

class PulseEngineBackend;

/**
 * @brief Interface for any graphics API backend (OpenGL, Vulkan, DirectX, etc.).
 * 
 * This abstract class defines all the necessary methods required to initialize,
 * manage, and use a rendering backend for the Pulse engine.
 */
class MA_DLL_API IGraphicsAPI
{
public:
    virtual ~IGraphicsAPI() = default;

    // ===== Initialization & Shutdown =====

    /**
     * @brief Initializes the graphics API and creates the main window.
     * 
     * @param title The window title.
     * @param width Pointer to the desired window width (can be modified).
     * @param height Pointer to the desired window height (can be modified).
     * @param engine Pointer to the Pulse engine backend instance.
     * @return true if initialization succeeds, false otherwise.
     */
    virtual bool Initialize(const char* title, int* width, int* height, PulseEngineBackend* engine) = 0;

    /**
     * @brief Shuts down the graphics API and releases all resources.
     */
    virtual void Shutdown() = 0;

    // ===== Window Management =====

    /**
     * @brief Polls input and window events (e.g., key presses, window resize).
     */
    virtual void PollEvents() const = 0;

    /**
     * @brief Swaps the front and back buffer (usually called once per frame).
     */
    virtual void SwapBuffers() const = 0;

    /**
     * @brief Checks if the window should close.
     * 
     * @return true if the window is closing, false otherwise.
     */
    virtual bool ShouldClose() const = 0;

    /**
     * @brief Sets the window size.
     * 
     * @param width The new width.
     * @param height The new height.
     */
    virtual void SetWindowSize(int width, int height) const = 0;

    /**
     * @brief Sets the window title.
     * 
     * @param title The new window title.
     */
    virtual void SetWindowTitle(const char* title) const = 0;

    /**
     * @brief Returns a handle to the native window (e.g., GLFWwindow*).
     * 
     * @return A void pointer to the native window.
     */
    virtual void* GetNativeHandle() const = 0;

    /**
     * @brief Gets the name of the current graphics API (e.g., "OpenGL", "Vulkan").
     * 
     * @return A string representing the API name.
     */
    virtual std::string GetApiName() const = 0;

    // ===== Rendering Flow =====

    /**
     * @brief Starts a new rendering frame. Should clear buffers and prepare for drawing.
     */
    virtual void StartFrame() const = 0;

    /**
     * @brief Ends the rendering frame. Usually includes buffer swapping or command submission.
     */
    virtual void EndFrame() const = 0;

    // ===== Shared Context Variables =====

    /// Pointer to current window width.
    int* width = nullptr;

    /// Pointer to current window height.
    int* height = nullptr;

    /// Pointer to the engine backend, for API-engine communication.
    PulseEngineBackend* engine = nullptr;
};

#endif // IGRAPHICSAPI_H
