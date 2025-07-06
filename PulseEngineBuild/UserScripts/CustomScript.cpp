#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "CustomScript.h"
#include <iostream>
#include <windows.h>

void CustomScript::OnStart()
{
}

void CustomScript::OnUpdate()
{
    PulseEngine::Vector3 position = parent->GetPosition();
    PulseEngine::Vector3 rotation = parent->GetRotation();
    if (GetAsyncKeyState('I') & 0x8000)
        position.z += 0.1f;
    if (GetAsyncKeyState('K') & 0x8000)
        position.z -= 0.1f;
    if (GetAsyncKeyState('J') & 0x8000)
        position.x -= 0.1f;
    if (GetAsyncKeyState('L') & 0x8000)
        position.x += 0.1f;
    if (GetAsyncKeyState('P') & 0x8000)
        position.y += 0.1f;
    if (GetAsyncKeyState('M') & 0x8000)
        position.y -= 0.1f;

    rotation.y += 1.0f * life;
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
