/**
 * @file GuidReader.h
 * @author Dorian LEXTERIAQUE (dlexteriaque@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef GUIDREADER_H
#define GUIDREADER_H

#include "Common/common.h"
#include "Common/dllExport.h"

class Entity;
class Mesh;

class PULSE_ENGINE_DLL_API GuidReader
{
    public:
        static Entity* GetEntityFromGuid(std::size_t Guid);
        static Mesh* GetMeshFromGuid(std::size_t guid);
        
        static Entity *GetEntityFromJson(nlohmann::json_abi_v3_12_0::json &entityData, Entity *entity);
        static std::size_t InsertIntoCollection(const std::string& filePath);
        static std::vector<std::pair<std::string, std::string>> GetAllAvailableFiles(const std::string& guidFile = ".pEntity");
};

#endif