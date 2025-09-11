#include "PulseEngine/API/EngineApi.h"
#include "PulseEngine/API/GameEntity.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "PulseEngine/core/GUID/GuidReader.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/core/Material/Material.h"
#include "PulseEngine/CustomScripts/IScripts.h"

Entity *PulseEngine::GameEntity::Instantiate(const std::string &path, PulseEngine::Vector3 position, PulseEngine::Vector3 rotation, PulseEngine::Vector3 scale)
{
    Entity* entity = new Entity("TempEntity", PulseEngine::Vector3(0.0f), nullptr, MaterialManager::loadMaterial("Materials/cube.mat"));

    nlohmann::json_abi_v3_12_0::json jsonData;
    std::ifstream pathContent(std::string(ASSET_PATH) + path);
    if(!pathContent.is_open())
    {
        EDITOR_ERROR("Path couldn't be open : " + std::string(ASSET_PATH) + path);
        return nullptr;
    }

    pathContent >> jsonData;
    entity = GuidReader::GetEntityFromJson(jsonData, entity);
    std::cout << "no error parsing entity" << std::endl;
    if(entity)
    {    
        //base data we have through parameter
        entity->SetName(path);
        entity->SetPosition(position);
        entity->SetRotation(rotation);
        entity->SetScale(scale);
        // entity->SetMaterial(MaterialManager::loadMaterial("Materials/cube.mat"));
        for(IScript* script : entity->GetScripts())
        {
            script->OnStart();
        }
        PulseEngineInstance->entities.push_back(entity);
    }
    else
    {
        EDITOR_ERROR("No entity could have been instantiated from path: " + std::string(ASSET_PATH) + path);
    }


    return entity;
}


std::vector<Entity *> PulseEngine::GameEntity::GetAllEntitiesByTag(const std::string &tag)
{
    std::vector<Entity*> entitiesWithTag;
    for(Entity* entity : PulseEngineInstance->entities)
    {
        if(entity->HasTag(tag))
        {
            entitiesWithTag.push_back(entity);
        }
    }
    return entitiesWithTag;
}