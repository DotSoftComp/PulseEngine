#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/API/EngineApi.h"
#include "Player.h"

void Player::OnStart()
{
    // Your initialization code here
    input.bindAction("jump", ' ');
}

void Player::OnUpdate()
{
    // Your update code here
    if(input.isActionDown("jump"))
    {
        owner->SetPosition(owner->GetPosition() + PulseEngine::Vector3(0.0f, 1.0f * PulseEngineInstance->GetDeltaTime(), 0.0f));
    }
}

void Player::OnRender()
{
    // Your render code here
}

const char* Player::GetName() const
{
    return "PulseScriptPlayer";
}

// Register the script
extern "C" __declspec(dllexport) IScript* PulseScriptPlayer()
{
    return new Player();
}

