#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/API/EngineApi.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/core/Physics/Collider/BoxCollider.h"
#include "ComponentPierre.h"

void ComponentPierre::OnStart()
{
    // Your initialization code here
}

void ComponentPierre::OnUpdate()
{

}

void ComponentPierre::OnRender()
{
}

const char* ComponentPierre::GetName() const
{
    return "PulseScriptComponentPierre";
}

// Register the script
extern "C" __declspec(dllexport) IScript* PulseScriptComponentPierre()
{
    return new ComponentPierre();
}

