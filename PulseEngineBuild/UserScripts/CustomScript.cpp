#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "CustomScript.h"
#include <iostream>
#include <windows.h>
#include "PulseEngine/API/EngineApi.h"
#include "PulseEngine/core/Math/Transform/Transform.h"
#include "PulseEngine/core/Physics/Collider/BoxCollider.h"
#include "BulletScript.h"
#include "MeteorScript.h"

void CustomScript::OnStart()
{
    
    
}



void CustomScript::OnUpdate()
{
    if(!doOnce)
    {
        doOnce = true;
    for (unsigned int i = 0; i < 100; i++)
    {
        Entity* nwEnt = PulseEngine::GameEntity::Instantiate(
                "Entities/primitiveCube.pEntity",
                PulseEngine::Vector3( -25 + (rand() % 50), -25 + (rand() % 50), -25 + (rand() % 50)),
                PulseEngine::Vector3(10 + (rand() % 500),10 + (rand() % 500),10 + (rand() % 500)),
                PulseEngine::Vector3(0.3f * (rand() % 5))
            );

        nwEnt->collider->SetSize(nwEnt->GetScale());

        MeteorScript* meteor = new MeteorScript();
        meteor->parent = nwEnt;
        nwEnt->AddScript(meteor);

    }

    }
    spawnTimer -= 0.1f;
    // Entity spawning
    if (GetAsyncKeyState('O') & 0x8000)
    {
        
        if (spawnTimer <= 0.0f)
        {
            spawnTimer = spawnDelay;

            PulseEngine::Vector3 front(PulseEngineInstance->GetActiveCamera()->Front.x, PulseEngineInstance->GetActiveCamera()->Front.y, PulseEngineInstance->GetActiveCamera()->Front.z);
            PulseEngine::Vector3 right(PulseEngineInstance->GetActiveCamera()->Right.x, PulseEngineInstance->GetActiveCamera()->Right.y, PulseEngineInstance->GetActiveCamera()->Right.z);

            PulseEngine::Vector3 spawnPos = PulseEngineInstance->GetCameraPosition() + front * 2.0f;

            spawnedEntity = PulseEngine::GameEntity::Instantiate(
                "Entities/primitiveCube.pEntity",
                spawnPos,
                PulseEngineInstance->GetCameraRotation(),
                PulseEngine::Vector3(0.3f)
            );
            spawnedEntity->collider->SetSize(spawnedEntity->GetScale());


            BulletScript* bullet = new BulletScript();
            bullet->direction = front;
            spawnedEntity->AddScript(bullet);
            bullet->parent = spawnedEntity;
        }
    }
}

const char* CustomScript::GetName() const
{
    return "PulseScriptA";
}

extern "C" __declspec(dllexport) IScript* PulseScriptA()
{
    return new CustomScript();
}
