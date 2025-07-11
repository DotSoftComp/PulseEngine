#include "PulseEngine/core/WindowContext/WindowContext.h"
#include "PulseEngine/core/Graphics/OpenGLAPI/OpenGLApi.h"
#include "PulseEngine/core/Graphics/IGraphicsApi.h"
#include "PulseEngine/core/PulseEngineBackend.h"
#include "pulseEngine/core/Inputs/Mouse.h"
#include <iostream>
#include "OpenGLApi.h"

bool OpenGLAPI::InitializeApi(const char* title, int* width, int* height, PulseEngineBackend* engine)
{
        this->engine = engine;
    if (!glfwInit())
    {
        EDITOR_ERROR("Failed to initialize GLFW.");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->width = width;
    this->height = height;

    window = glfwCreateWindow(*width, *height, title, nullptr, nullptr);
    if (!window)
    {
        EDITOR_ERROR("Failed to create GLFW window.");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, PulseEngineBackend::FramebufferSizeCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        EDITOR_ERROR("Error while initializing GLAD.");
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    glfwMakeContextCurrent(window);

        // Create texture to render to
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fboWidth, fboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create framebuffer
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

    // Create Renderbuffer for depth and stencil
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fboWidth, fboHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        EDITOR_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void OpenGLAPI::ShutdownApi()
{
    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

void OpenGLAPI::PollEvents() const
{
    
    glfwSetCursorPosCallback(window, MouseInput::MouseCallback);
    glfwSetScrollCallback(window, MouseInput::ScrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void OpenGLAPI::SwapBuffers() const
{
    glfwSwapBuffers(window);
}

bool OpenGLAPI::ShouldClose() const
{
    return glfwWindowShouldClose(window);
}

void OpenGLAPI::SetWindowSize(int width, int height) const
{
    glfwSetWindowSize(window, width, height);
}

void OpenGLAPI::SetWindowTitle(const char *title) const
{
    glfwSetWindowTitle(window, title);
}

void *OpenGLAPI::GetNativeHandle() const
{
    return static_cast<void*>(window);
}

void OpenGLAPI::StartFrame() const
{
    EDITOR_ONLY(
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, fboWidth, fboHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    )
    IN_GAME_ONLY(
        glfwGetFramebufferSize(window, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, *width, *height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.8f, 0.8f, 0.6f, 1.0f);
    )
}

void OpenGLAPI::EndFrame() const
{
    EDITOR_ONLY(
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Go back to default framebuffer
        glViewport(0, 0, 1920, 1080); // Reset to default screen size
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
    )
}
OpenGLAPI::~OpenGLAPI()
{
    Shutdown();
}
