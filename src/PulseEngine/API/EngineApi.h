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
    /**
     * @brief The GameEntity class provides static functions relative to the management of entities in the game.
     * 
     */
    class PULSE_ENGINE_DLL_API GameEntity
    {
        public:
        /**
         * @brief Instantiate an entity from a path to a file, position, rotation and scale.
         * @details The instantiation will directly call the Start() method from the entity, and also all start methods from the scripts attached to the entity.
         * @details If no entity could have been instantiated, it will return a nullptr.
         * @note By passing an Entity* to this method, by replacing path, you can instantiate an already existing entity in the world, saving ressources from loading it again.
         * 
         * @param path The path of the .pentity file. With this method you can only instantiate entities that are loaded in the engine.
         * @param position Position where the entity will be instantiated. 
         * @param rotation Rotation of the entity in degrees (pitch, yaw, roll).
         * @param scale Scale of the entity, we recommend to use the same scalar on each axis to keep the entity uniform.
         * @return Entity* The pointer to the entity instantiate in the world. You can directly modify it after the instantiation.
         */
        static Entity* Instantiate(const std::string& path, PulseEngine::Vector3 position = PulseEngine::Vector3(0.0f, 0.0f, 0.0f), PulseEngine::Vector3 rotation = PulseEngine::Vector3(0.0f, 0.0f, 0.0f), PulseEngine::Vector3 scale = PulseEngine::Vector3(1.0f, 1.0f, 1.0f));
    
        static std::vector<Entity*> GetAllEntitiesByTag(const std::string& tag);
    };
}


#endif