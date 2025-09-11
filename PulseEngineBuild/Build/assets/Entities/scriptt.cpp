#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/API/EngineApi.h"
#include "scriptt.h"

void scriptt::OnStart()
{
    input.bindAction("jump", ' ');
}

void scriptt::OnUpdate()
{
    if(input.wasActionPressed("jump"))
    {
        //jump logic of the player
    }
    
}

void scriptt::OnRender()
{
    // Your update code here
}

const char* scriptt::GetName() const
{
    return "PulseScriptscriptt";
}

// Register the script
extern "C" __declspec(dllexport) IScript* PulseScriptscriptt()
{
    return new scriptt();
}

