/**
 * @file OpenGLApi.h
 * @author Dorian LEXTERIAQUE (dlexteriaque@gmail.com)
 * @brief implementation of the OpenGL graphics API for Pulse Engine.
 * @brief This class implements the IGraphicsAPI interface and provides methods to initialize, shutdown...
 * @version 0.1
 * @date 2025-06-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include "PulseEngine/core/Graphics/IGraphicsApi.h"
#include <GLFW/glfw3.h>

class OpenGLAPI : public IGraphicsAPI
{
public:
    OpenGLAPI() = default;
    virtual ~OpenGLAPI() override;

    bool Initialize(const char* title, int* width, int* height, PulseEngineBackend* engine) override;
    void Shutdown() override;

    void PollEvents() const override;
    void SwapBuffers() const override;
    bool ShouldClose() const override;

    void SetWindowSize(int width, int height) const override;
    void SetWindowTitle(const char* title) const override;

    void* GetNativeHandle() const override;
    std::string GetApiName() const override { return "OpenGL"; }

    
    void StartFrame() const override;
    void EndFrame() const override;

    GLFWwindow* window = nullptr;
    GLuint fbo, fboTexture, rbo;
    int fboWidth = 1024;
    int fboHeight = 720;
private:
};

#endif // OPENGLAPI_H
