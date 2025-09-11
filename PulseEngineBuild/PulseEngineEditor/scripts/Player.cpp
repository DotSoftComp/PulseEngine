#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/API/EngineApi.h"
#include "Player.h"
#include "PulseEngine/core/Math/MathUtils.h"

void Player::OnStart()
{
    // Your initialization code here
    PulseEngineInstance->inputSystem->bindAction("up", 'Z');
    PulseEngineInstance->inputSystem->bindAction("down", 'S');
    PulseEngineInstance->inputSystem->bindAction("left", 'Q');
    PulseEngineInstance->inputSystem->bindAction("right", 'D');
    playerCamera = PulseEngineInstance->GetActiveCamera();
}

void Player::OnUpdate()
{

    left += PulseEngineInstance->inputSystem->isActionDown("left") ? 0.5f : PulseEngineInstance->inputSystem->isActionDown("right") ? -0.5f : 0.0f;
    top += PulseEngineInstance->inputSystem->isActionDown("up") ? 0.5f : PulseEngineInstance->inputSystem->isActionDown("down") ? -0.5f : 0.0f;
    playerCamera->Position = PulseEngine::MathUtils::RotateAround(PulseEngine::Vector3(0.0f,0.0f,-2.0f), top, left, 10.0f);
    playerCamera->Front = (PulseEngine::Vector3(0.0f,0.0f,-2.0f) - playerCamera->Position).Normalized();
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

