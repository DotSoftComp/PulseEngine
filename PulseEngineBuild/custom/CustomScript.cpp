#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "CustomScript.h"
#include <iostream>

void CustomScript::OnStart()
{
}

void CustomScript::OnUpdate()
{
    parent->SetPosition(parent->GetPosition() + PulseEngine::Vector3(0.001f));
}

const char* CustomScript::GetName() const
{
    return "CustomScriptA";
}

extern "C" __declspec(dllexport) IScript* CreateCustomScriptA()
{
    return new CustomScript();
}

std::vector<ExposedVariable> CustomScript::GetExposedVariables()
{
    return exposedVariables;
}
