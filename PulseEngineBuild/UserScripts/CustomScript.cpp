#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "CustomScript.h"
#include <iostream>
#include <windows.h>
#include "PulseEngine/API/EngineApi.h"
#include "PulseEngine/core/Math/Transform/Transform.h"

void CustomScript::OnStart()
{
    // Initialization logic if needed
}

void CustomScript::OnUpdate()
{
    spawnTimer -= 0.1f;

    PulseEngine::Vector3 position = parent->GetPosition();
    PulseEngine::Vector3 rotation = parent->GetRotation();

    // Movement controls
    if (GetAsyncKeyState('I') & 0x8000) position.z += 0.1f;
    if (GetAsyncKeyState('K') & 0x8000) position.z -= 0.1f;
    if (GetAsyncKeyState('J') & 0x8000) position.x -= 0.1f;
    if (GetAsyncKeyState('L') & 0x8000) position.x += 0.1f;
    if (GetAsyncKeyState('P') & 0x8000) position.y += 0.1f;
    if (GetAsyncKeyState('M') & 0x8000) position.y -= 0.1f;

    // Entity spawning
    if (GetAsyncKeyState('O') & 0x8000)
    {
        if (spawnTimer <= 0.0f && spawnedEntity == nullptr)
        {
            spawnTimer = spawnDelay;

            PulseEngine::Transform transform;
            transform.position = parent->GetPosition();
            transform.rotation = parent->GetRotation();

            PulseEngine::Vector3 spawnPos = transform.position + transform.GetForward() * 2.0f;

            spawnedEntity = PulseEngine::GameEntity::Instantiate(
                "Entities/primitiveCube.pEntity",
                spawnPos,
                parent->GetRotation(),
                PulseEngine::Vector3(1.0f, 1.0f, 1.0f)
            );

            std::cout << "Spawned an entity at position: "
                      << spawnedEntity->GetPosition().x << ", "
                      << spawnedEntity->GetPosition().y << ", "
                      << spawnedEntity->GetPosition().z << std::endl;
        }
    }

    // Update spawned entity position
    if (spawnedEntity != nullptr)
    {
        PulseEngine::Transform transform;
        transform.position = parent->GetPosition();
        transform.rotation = parent->GetRotation();
        transform.rotation.x *= -1;
        transform.rotation.y *= -1;
        transform.rotation.z *= -1;

        PulseEngine::Vector3 newPos = transform.position + transform.GetForward() * 2.0f;
        spawnedEntity->SetPosition(newPos);
    }

    // Apply rotation
    rotation.x += 1.0f * life;

    parent->SetPosition(position);
    parent->SetRotation(rotation);
}

const char* CustomScript::GetName() const
{
    return "PulseScriptA";
}

extern "C" __declspec(dllexport) IScript* PulseScriptA()
{
    return new CustomScript();
}
