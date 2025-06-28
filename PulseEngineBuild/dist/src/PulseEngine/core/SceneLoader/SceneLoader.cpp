#include "SceneLoader.h"
#include "PulseEngine/core/GUID/GuidReader.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/PulseEngineBackend.h"
#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/CustomScripts/ScriptsLoader.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "PulseEngine/core/Meshes/primitive/Primitive.h"
#include "PulseEngine/core/Meshes/primitive/Primitive.h"

#include <iostream>
#include <assimp/Importer.hpp>      // Assimp::Importer
#include <assimp/scene.h>           // aiScene
#include <assimp/postprocess.h>     // postprocessing flags

#pragma region SceneLoader
void SceneLoader::LoadScene(const std::string &mapName, PulseEngineBackend* backend)
{
    std::ifstream scene(mapName);
    if (!scene.is_open())
    {
        std::cout << "Couldn't open map " << mapName << std::endl;
        return;
    }
    backend->ClearScene();

    nlohmann::json sceneData;
    scene >> sceneData;

    backend->SetWindowName(sceneData["sceneName"]);

    for (const auto& entityData : sceneData["entities"])
    {        

        std::cout << "creating a new entity" << std::endl;

        Entity* entity = LoadEntityBaseData(entityData);


        if(entity)
        {
            for(auto& script : entityData["Scripts"])
            {
                LoadEntityScript(script, entity);
            }
            std::cout << "adding the entity to the backend" << std::endl;
            std::cout << "entity guid: " << entity->GetGuid() << std::endl;
            std::cout << "entity muid: " << entity->GetMuid() << std::endl;
            backend->entities.push_back(entity);
        }
        else
        {
            std::cout << "entity is not valid !" << std::endl;
        }
    }

    std::cout << "Scene " << mapName << " loaded successfully." << std::endl;
}

void SceneLoader::LoadEntityScript(const nlohmann::json_abi_v3_12_0::json &script, Entity *entity)
{
    std::string scriptName = script["name"].get<std::string>();
    std::vector<ExposedVariable *> exposedVariables;
    for (const auto &var : script["exposedVariables"])
    {
        ExtractExposedVariable(var, exposedVariables);
    }
    IScript *scriptInstance = ScriptsLoader::GetScriptFromCallName(scriptName);
    if (scriptInstance)
    {
        scriptInstance->isEntityLinked = false;
        scriptInstance->parent = entity;
        for (const auto &var : exposedVariables)
        {
            scriptInstance->AddExposedVariable(*var);
            void *dst = scriptInstance->GetVariableByName(var->name);
            if (dst)
            {
                switch (var->type)
                {
                case ExposedVariable::Type::FLOAT:
                    *reinterpret_cast<float *>(dst) = *reinterpret_cast<float *>(var->ptr);
                    break;
                case ExposedVariable::Type::INT:
                    *reinterpret_cast<int *>(dst) = *reinterpret_cast<int *>(var->ptr);
                    break;
                case ExposedVariable::Type::BOOL:
                    *reinterpret_cast<bool *>(dst) = *reinterpret_cast<bool *>(var->ptr);
                    break;
                case ExposedVariable::Type::STRING:
                    *reinterpret_cast<std::string *>(dst) = *reinterpret_cast<std::string *>(var->ptr);
                    break;
                }
            }

            std::cout << "Exposed variable name: " + var->name << std::endl;
        }
        entity->AddScript(scriptInstance);
        std::cout << "Script " << scriptName << " loaded." << std::endl;
    }
    else
    {
        std::cout << "Script " << scriptName << " not found." << std::endl;
    }
}

void SceneLoader::ExtractExposedVariable(const nlohmann::json_abi_v3_12_0::json &var, std::vector<ExposedVariable *> &exposedVariables)
{
    ExposedVariable::Type type = static_cast<ExposedVariable::Type>(var["type"].get<int>());
    std::string name = var["name"].get<std::string>();
    void *ptr = nullptr;

    switch (type)
    {
    case ExposedVariable::Type::INT:
        ptr = new int(var["value"].get<int>());
        break;
    case ExposedVariable::Type::FLOAT:
        ptr = new float(var["value"].get<float>());
        break;
    case ExposedVariable::Type::BOOL:
        ptr = new bool(var["value"].get<bool>());
        break;
    case ExposedVariable::Type::STRING:
        ptr = new std::string(var["value"].get<std::string>());
        break;
    }

    exposedVariables.push_back(new ExposedVariable(name, type, ptr));
}

Entity* SceneLoader::LoadEntityBaseData(const nlohmann::json_abi_v3_12_0::json &entityData)
{
    PulseEngine::Vector3 position;
    PulseEngine::Vector3 rotation;
    PulseEngine::Vector3 scale;
    position.x = entityData["position"][0].get<float>();
    position.y = entityData["position"][1].get<float>();
    position.z = entityData["position"][2].get<float>();

    rotation.x = entityData["rotation"][0].get<float>();
    rotation.y = entityData["rotation"][1].get<float>();
    rotation.z = entityData["rotation"][2].get<float>();

    scale.x = entityData["scale"][0].get<float>();
    scale.y = entityData["scale"][1].get<float>();
    scale.z = entityData["scale"][2].get<float>();
    std::string name = entityData["name"].get<std::string>();

    std::size_t guid = entityData["guid"].get<std::size_t>();
    std::size_t muid = entityData["muid"].get<std::size_t>();
    nlohmann::json entityDocData;

    Entity* entity = GuidReader::GetEntityFromGuid(guid);
    if (!entity)
    {
        std::cout << "entity not valid" << std::endl;
        return nullptr;
    };
    entity->SetGuid(guid);
    entity->SetMuid(muid);
    entity->SetPosition(position);
    entity->SetRotation(rotation);
    entity->SetScale(scale);
    entity->SetName(name);
    entity->SetMaterial(MaterialManager::loadMaterial(std::string(ASSET_PATH) + "Materials/cube.mat"));
    return entity;
}

std::vector<std::string> SceneLoader::GetSceneFiles(const std::string &directory)
{
    std::vector<std::string> sceneFiles;
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".pmap")
        {
            // Normalize path separators and remove everything before "/Scenes/"
            std::string path = entry.path().generic_string(); // uses '/' as separator
            std::size_t pos = path.find("/Scenes/");
            if (pos != std::string::npos)
            {
                sceneFiles.push_back(path.substr(pos));
            }
            else
            {
                // fallback: just use the filename
                sceneFiles.push_back("/Scenes/" + entry.path().filename().string());
            }
        }
    }
    return sceneFiles;
}

const aiScene* SceneLoader::LoadSceneFromAssimp(std::string path)
{    
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenNormals|
        aiProcess_CalcTangentSpace
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Erreur Assimp: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    std::cout << "Modèle chargé avec succès : " << path << std::endl;
    std::cout << "Nombre de meshes : " << scene->mNumMeshes << std::endl;
    std::cout << "Nombre de matériaux : " << scene->mNumMaterials << std::endl;
    std::cout << "Nombre de textures : " << scene->mNumTextures << std::endl;

    // Affichage du premier mesh pour exemple
    if (scene->mNumMeshes > 0)
    {
        aiMesh* mesh = scene->mMeshes[0];
        std::cout << "Premier mesh contient " << mesh->mNumVertices << " sommets." << std::endl;
    }
    return scene;
}

#pragma endregion

#pragma region SaveScene
void SceneLoader::SaveSceneToFile(const std::string &mapName, PulseEngineBackend *backend)
{
    nlohmann::json sceneData;

    // Set the scene name
    sceneData["sceneName"] = mapName;

    // Save entities
    for (const auto& entity : backend->entities)
    {
        SaveEntities(entity, sceneData);
    }

    // Write to file
    std::ofstream sceneFile(std::string(ASSET_PATH) + "Scenes/" + std::string(mapName) + ".pmap");
    if (!sceneFile.is_open())
    {
        std::cout << "Couldn't open file to save map " << mapName << std::endl;
        return;
    }

    sceneFile << sceneData.dump(4); // Pretty print with 4 spaces
    sceneFile.close();
}

void SceneLoader::SaveEntities(Entity *const &entity, nlohmann::json_abi_v3_12_0::json &sceneData)
{
    nlohmann::json entityData;

    SaveBaseDataEntity(entity, entityData);

    nlohmann::json scriptsData = nlohmann::json::array();
    for (const auto &script : entity->GetScripts())
    {
        nlohmann::json scriptData;
        scriptData["name"] = script->GetName();
        scriptData["exposedVariables"] = nlohmann::json::array();
        for (const auto &var : script->GetExposedVariables())
        {
            nlohmann::json varDt;
            varDt["name"] = var.name;
            varDt["type"] = var.type;
            switch (var.type)
            {
            case ExposedVariable::Type::INT:
                varDt["value"] = *reinterpret_cast<int *>(var.ptr);
                break;
            case ExposedVariable::Type::FLOAT:
                varDt["value"] = *reinterpret_cast<float *>(var.ptr);
                break;
            case ExposedVariable::Type::BOOL:
                varDt["value"] = *reinterpret_cast<bool *>(var.ptr);
                break;
            case ExposedVariable::Type::STRING:
                varDt["value"] = *reinterpret_cast<std::string *>(var.ptr);
                break;
            }
            scriptData["exposedVariables"].push_back(varDt);
        }

        // Add the script name to the JSON array
        if (!script->isEntityLinked)
            scriptsData.push_back(scriptData);
    }

    // Add the scripts to the entity data
    entityData["Scripts"] = scriptsData;

    sceneData["entities"].push_back(entityData);
}

void SceneLoader::SaveBaseDataEntity(Entity *const &entity, nlohmann::json_abi_v3_12_0::json &entityData)
{
    // Save position
    PulseEngine::Vector3 position = entity->GetPosition();
    entityData["position"] = {position.x, position.y, position.z};

    // Save rotation
    PulseEngine::Vector3 rotation = entity->GetRotation();
    entityData["rotation"] = {rotation.x, rotation.y, rotation.z};

    PulseEngine::Vector3 scale = entity->GetScale();
    entityData["scale"] = {scale.x, scale.y, scale.z};
    // Save name
    entityData["name"] = entity->GetName();

    // Save GUID
    entityData["guid"] = entity->GetGuid();
    entityData["muid"] = entity->GetMuid();
}

#pragma endregion