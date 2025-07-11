/**
 * @file PulseEngineBackend.h
 * @author Dorian LEXTERIAQUE (dlexteriaque@gmail.com)
 * @brief Where all the magic happens, the main class of the Pulse Engine.
 * @brief This class is responsible for initializing the engine, managing the main loop, and rendering the scene.
 * @brief It also handles the window context, input processing, and scene management.
 * @todo remove ALL THE OPENGL AND USE PULSE ENGINE CLASSES !!!!
 * @version 0.1
 * @date 2025-06-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef PULSEENGINE_H
#define PULSEENGINE_H

#include <string>
#include "Common/common.h"
#include "Common/dllExport.h"
#include "json.hpp"

#define GUID_COLLECTION_PATH std::string(ASSET_PATH) + "Guid/"
#define DEFAULT_SHADOW_MAP_RES 2048

class WindowContext;
class Entity;
class LightData;
class Camera;
class InterfaceEditor;
class LightManager;
class CoroutineManager;

class IGraphicsAPI;

/**
 * @brief PulseEngineBackend is the main class of the Pulse Engine.
 * 
 */
class PULSE_ENGINE_DLL_API PulseEngineBackend
{
public:
    friend InterfaceEditor;
    friend LightManager;

    PulseEngineBackend();
    ~PulseEngineBackend() {}

    void LoadModel(std::string path);

    // === main functions Engine ===
    int Initialize(); 
    bool IsRunning(); 
    void PollEvents(); 
    void Update(); 
    void Render(); 
    void RenderShadow(); 
    void Shutdown(); 

    void ClearScene();
    void DeleteEntity(Entity* entity);

    // === getters ===
    WindowContext* GetWindowContext() { return windowContext; }

    // === static functions ===
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static Camera* GetActiveCamera() {return activeCamera;}


    std::vector<Entity*> entities;
    std::vector<LightData*> lights;

    std::string actualMapPath = "Scenes/defaultMap.pmap";
    std::string actualMapName = "defaultMap.pmap";

    std::string GetWindowName(const std::string& location) ;
    void SetWindowName(const std::string& location);

    // === getters ===
    const std::string& GetEngineName() const { return engine; }
    const std::string& GetEngineVersion() const { return version; }
    const std::string& GetDevMonth() const { return devMonth; }
    const std::string& GetCompanyName() const { return company; }

    const void SetGameName(const std::string& name) { gameName = name; }
    const std::string& GetGameName() const { return gameName; }

    const void SetGameVersion(const std::string& version) { gameVersion = version; }
    const std::string& GetGameVersion() const { return gameVersion; }
    
    IGraphicsAPI* graphicsAPI = nullptr;
    CoroutineManager* coroutineManager = nullptr;
private:
    WindowContext* windowContext = nullptr;

    int width = 1920;
    int height = 1080;
    std::string gameName = "NULL";
    std::string gameVersion = "0.0.1";

    std::string engine = "Pulse Engine";
    std::string version = "V0.0.7";
    std::string devMonth = "July 2025";
    std::string company = "Pulse Software";

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float mapLoading = 0.0f;

    static Camera* activeCamera;

    PulseEngine::Mat4 view;
    glm::mat4 projection;

    Shader* shadowShader;
    Shader* debugShader;

    nlohmann::json_abi_v3_12_0::json engineConfig;


    void ProcessInput(GLFWwindow* window);
    glm::vec3 CalculateLighting(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& viewPos, const LightData& light);
    bool IsRenderable(Entity* entity) const;
};

#endif
