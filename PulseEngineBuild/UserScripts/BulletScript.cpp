#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "BulletScript.h"
#include <iostream>
#include <windows.h>
#include "PulseEngine/API/EngineApi.h"
#include "PulseEngine/core/Math/Transform/Transform.h"

void BulletScript::OnStart()
{
    // Initialization logic if needed
}


void BulletScript::OnUpdate()
{
    parent->SetPosition(parent->GetPosition() + (direction * speed));
}

const char* BulletScript::GetName() const
{
    return "PulseScriptABulletScript";
}

extern "C" __declspec(dllexport) IScript* PulseScriptABulletScript()
{
    return new BulletScript();
}
