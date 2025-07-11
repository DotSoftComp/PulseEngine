#include "PulseEngineBackend.h"
#include "Common/common.h"
#include "camera.h"
#include "PulseEngine/core/Inputs/Mouse.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/Material/Material.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "json.hpp"
#include <fstream>
#include "PulseEngine/core/Meshes/primitive/Primitive.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/core/GUID/GuidReader.h"
#include "PulseEngine/core/SceneLoader/SceneLoader.h"
#include "PulseEngine/core/Lights/Lights.h"
#include "PulseEngine/core/FileManager/FileManager.h"
#include "PulseEngine/core/Lights/DirectionalLight/DirectionalLight.h"
#include "PulseEngine/core/Lights/PointLight/PointLight.h"
#include "PulseEngine/CustomScripts/ScriptsLoader.h"
#include "PulseEngine/core/Graphics/OpenGLAPI/OpenGLApi.h"
#include "PulseEngine/core/WindowContext/WindowContext.h"
#include "PulseEngine/core/Graphics/IGraphicsApi.h"
#include "PulseEngine/core/Physics/Collider/Collider.h"
#include "PulseEngine/core/Physics/Collider/BoxCollider.h"
#include "PulseEngine/core/Lights/LightManager.h"
#include "PulseEngine/core/Physics/CollisionManager.h"
#include "PulseEngine/core/coroutine/CoroutineManager.h"
#include "PulseEngine/core/coroutine/Coroutine.h"



#include "Common/dllExport.h"

Camera* PulseEngineBackend::activeCamera = new Camera();


PulseEngineBackend::PulseEngineBackend() { }

int PulseEngineBackend::Initialize()
{
    std::cout << "init" << std::endl;
    windowContext = new WindowContext();
    activeCamera = new Camera();

    // load the graphic API based on the platform
    //some platform can have multiple graphic API possible.
    graphicsAPI = dynamic_cast<IGraphicsAPI*>(ModuleLoader::GetModuleFromPath("Modules/OpenGLApi.dll"));

    if(graphicsAPI == nullptr)
    {
        std::cerr << "Error while initializing graphics API." << std::endl;
        return -1;
    }
    graphicsAPI->InitializeApi(GetWindowName("editor").c_str(), &width, &height, this);
    windowContext->SetGLFWWindow(static_cast<GLFWwindow*>(graphicsAPI->GetNativeHandle()));

    // coroutine manager, give the possibility to add async tasks to the engine
    coroutineManager = new CoroutineManager();


    // lights.push_back(new DirectionalLight(1.0f, 10.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f, 3.0f, 0.0f), glm::vec3(1.0f), 10, 1));
    // lights.back()->InitShadowMap(DEFAULT_SHADOW_MAP_RES);

    shadowShader = new Shader(std::string(ASSET_PATH) + "shaders/depthMap/Depth.vert", std::string(ASSET_PATH) + "shaders/depthMap/Depth.frag");
    debugShader = new Shader(std::string(ASSET_PATH) +"shaders/debug.vert", std::string(ASSET_PATH) + "shaders/debug.frag");
    
    // === insert base item to the collection ===
    GuidReader::InsertIntoCollection("Entities/simpleActor.pEntity");    
    // GuidReader::InsertIntoCollection("models/primitiveCube.pmesh");    
    
    
    EDITOR_ONLY(
        ScriptsLoader::LoadDLL();
    )

 
    engineConfig = FileManager::OpenEngineConfigFile(this);

    std::string firstScene = engineConfig["GameData"]["FirstScene"];
    SceneLoader::LoadScene(std::string(ASSET_PATH) + firstScene, this);

    return 0;
}



void PulseEngineBackend::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

std::string PulseEngineBackend::GetWindowName(const std::string &location)
{
    if(gameName != "NULL") return std::string(gameName) + " - " + gameVersion;
    else return engine + " | " + version +  " | " + location +  " | " + devMonth + " | " + company + " <" + graphicsAPI->GetApiName() + ">";
}

void PulseEngineBackend::SetWindowName(const std::string &location)
{
    graphicsAPI->SetWindowTitle(GetWindowName(location).c_str());
}

bool PulseEngineBackend::IsRunning()
{
    return !graphicsAPI->ShouldClose();
}

void PulseEngineBackend::PollEvents()
{
    graphicsAPI->PollEvents();
    ProcessInput(static_cast<GLFWwindow*>(graphicsAPI->GetNativeHandle()));
}

void PulseEngineBackend::Update()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    view = GetActiveCamera()->GetViewMatrix();
    projection = glm::perspective(glm::radians(GetActiveCamera()->Zoom), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
    mapLoading -= deltaTime;
    for (auto& light : lights) 
    {
        light->RecalculateLightSpaceMatrix();
    }

    IN_GAME_ONLY(
    for (size_t i = 0; i < entities.size(); ++i)
    {
        Entity* entityA = entities[i];
        entityA->UpdateEntity(deltaTime);
        for (size_t j = i + 1; j < entities.size(); ++j)
        {
            Entity* entityB = entities[j];
            CollisionManager::ManageCollision(dynamic_cast<Collider*>(entityA->collider), dynamic_cast<Collider*>(entityB->collider));                
        }
    }
    )

    coroutineManager->UpdateAll(deltaTime);
}

void PulseEngineBackend::Render()
{
    graphicsAPI->StartFrame();

    for (Entity* entity : entities)
    {
        if (!IsRenderable(entity)) continue;
        Shader* shader = entity->GetMaterial()->GetShader();

        shader->Use();
        //here we need to use specific shader setter for each graphic api.
        /**
         * @todo make SetMat4() and others from glm:: to PulseEngine::
         * 
         */
        #ifdef WINDOW_PULSE_EXPORT
        shader->SetMat4("projection", projection);
        shader->SetMat4("view", glm::mat4(view[0][0], view[0][1], view[0][2], view[0][3],
                          view[1][0], view[1][1], view[1][2], view[1][3],
                          view[2][0], view[2][1], view[2][2], view[2][3],
                          view[3][0], view[3][1], view[3][2], view[3][3]));
        shader->SetVec3("viewPos", glm::vec3(GetActiveCamera()->Position.x, GetActiveCamera()->Position.y, GetActiveCamera()->Position.z));
        #endif
        LightManager::BindLightsToShader(shader, this, entity);

        entity->DrawEntity();
    }

    graphicsAPI->EndFrame();
}



void PulseEngineBackend::RenderShadow()
{    
    shadowShader->Use();
    for (int i = 0; i < lights.size(); ++i)
    {
        lights[i]->RenderShadowMap(*shadowShader, *this);
    }
}

void PulseEngineBackend::Shutdown()
{    
    graphicsAPI->ShutdownApi();
}

void PulseEngineBackend::ClearScene()
{
    for(auto ent : entities)
    {
        delete ent;
    }
    entities.clear();
}
void PulseEngineBackend::DeleteEntity(Entity *entity)
{
    auto it = std::find(entities.begin(), entities.end(), entity);
    if (it != entities.end())
    {
        delete *it;
        entities.erase(it);
    }
}

void PulseEngineBackend::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) activeCamera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) activeCamera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) activeCamera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) activeCamera->ProcessKeyboard(RIGHT, deltaTime);
}

glm::vec3 PulseEngineBackend::CalculateLighting(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec3 &viewPos, const LightData& light)
{
#ifndef WINDOW_PULSE_EXPORT
    // Calculate the direction from the object to the light
    glm::vec3 lightDir = glm::normalize(glm::vec3(light.GetPosition().x, light.GetPosition().y, light.GetPosition().z) - position);

    // Diffuse lighting: Lambertian reflectance (dot product of normal and light direction)
    float diff = glm::max(glm::dot(normal, lightDir), 0.0f);

    // Specular lighting: Blinn-Phong reflection model
    glm::vec3 reflectDir = glm::reflect(-lightDir, normal);
    glm::vec3 viewDir = glm::normalize(viewPos - position);
    float spec = pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), 32); // 32 is the shininess

    // Apply attenuation (optional for point lights)
    float distance = glm::length(glm::vec3(light.GetPosition().x, light.GetPosition().y, light.GetPosition().z) - position);
    float attenuation = 1.0f / (1.0f + light.attenuation * distance * distance);

    // Final color calculation (ambient + diffuse + specular)
    glm::vec3 ambientColor = glm::vec3(light.GetColor().r, light.GetColor().g, light.GetColor().b);
    glm::vec3 ambient = 0.1f * ambientColor; // Ambient light
    glm::vec3 diffuse = diff * ambientColor * light.intensity;
    glm::vec3 specular = spec * ambientColor * light.intensity;

    // Combine all lighting effects, considering attenuation for point lights
    glm::vec3 finalColor = (ambient + diffuse + specular) * attenuation;
    
    return finalColor;
    #endif
    return  glm::vec3(1.0f, 1.0f, 1.0f); // Default color if not in export mode
}

bool PulseEngineBackend::IsRenderable(Entity *entity) const
{
    return entity != nullptr && entity->GetMaterial() != nullptr && entity->GetMaterial()->GetShader() != nullptr;
}
