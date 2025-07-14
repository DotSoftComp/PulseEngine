/**
 * @file EngineApi.h
 * @author Dorian LEXTERIAQUE (dlexteriaque@gmail.com)
 * @brief The API of the Pulse Engine to use all the functionnality without touching the source code.
 * @version 0.1
 * @date 2025-07-12
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef ENGINEAPI_H
#define ENGINEAPI_H

#include "Common/common.h"
#include "Common/dllExport.h"

#include "PulseEngine/core/PulseEngineBackend.h"

class Entity;

namespace PulseEngine
{
    class PULSE_ENGINE_DLL_API GameEntity
    {
        public:
        static Entity* Instantiate(const std::string& path, PulseEngine::Vector3 position = PulseEngine::Vector3(0.0f, 0.0f, 0.0f), PulseEngine::Vector3 rotation = PulseEngine::Vector3(0.0f, 0.0f, 0.0f), PulseEngine::Vector3 scale = PulseEngine::Vector3(1.0f, 1.0f, 1.0f));
    };
}


#endif