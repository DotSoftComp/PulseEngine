#include "PulseEngine/API/EngineApi.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "PulseEngine/core/GUID/GuidReader.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/core/Material/Material.h"


Entity *PulseEngine::GameEntity::Instantiate(const std::string &path, PulseEngine::Vector3 position, PulseEngine::Vector3 rotation, PulseEngine::Vector3 scale)
{
    Entity* entity = nullptr;

    //try to get base entity from the path name serialized into a GUID
    std::size_t guid = GenerateGUIDFromPath(path);
    entity = GuidReader::GetEntityFromGuid(guid);
    if(entity)
    {    
        //base data we have through parameter
        entity->SetName(path);
        entity->SetPosition(position);
        entity->SetRotation(rotation);
        entity->SetScale(scale);
        entity->SetGuid(guid);
        entity->SetMaterial(MaterialManager::loadMaterial(std::string(ASSET_PATH) + "Materials/cube.mat"));
    }

    PulseEngineBackend::GetInstance()->entities.push_back(entity);

    return entity;
}