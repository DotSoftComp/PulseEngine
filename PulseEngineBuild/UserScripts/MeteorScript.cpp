#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "MeteorScript.h"
#include <iostream>
#include <windows.h>
#include "PulseEngine/API/EngineApi.h"
#include "PulseEngine/core/Math/Transform/Transform.h"
#include "PulseEngine/core/Physics/Collider/BoxCollider.h"
#include "BulletScript.h"

void MeteorScript::OnStart()
{
    // Initialization logic if needed
    origin = parent->GetPosition();
    rotation = PulseEngine::Vector3(- 90 + rand() % 180,- 90 + rand() % 180,- 90 + rand() % 180);
    speed = rand() % 5;
    timer = -10 + rand() % 20;
}


void MeteorScript::OnUpdate()
{
    timer += PulseEngineInstance->GetDeltaTime();
    parent->SetPosition(origin + (PulseEngine::Vector3(0.0f, 0.3f, 0.0f) * speed * cosf(timer)));
    parent->SetRotation(rotation * (timer / 10.0f));

    for(auto col : parent->collider->othersCollider)
    {
        for(auto script : col->parent->GetScripts())
        {
            if(dynamic_cast<BulletScript*>(script))
            {
                parent->SetScale(PulseEngine::Vector3(0.01f));
                col->parent->SetScale(PulseEngine::Vector3(0.01f));
            }
        }
    }
    
}

const char* MeteorScript::GetName() const
{
    return "PulseScriptAMeteorScript";
}

extern "C" __declspec(dllexport) IScript* PulseScriptAMeteorScript()
{
    return new MeteorScript();
}
