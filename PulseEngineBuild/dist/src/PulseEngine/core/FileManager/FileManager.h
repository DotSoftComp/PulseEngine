/**
 * @file FileManager.h
 * @author Dorian LEXTERIAQUE (dlexteriaque@gmail.com)
 * @brief Filemanager to handle file operations in the Pulse Engine, with a cross-platform approach.
 * @version 0.1
 * @date 2025-06-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Common/common.h"
#include "Common/dllExport.h"
#include "json.hpp"

class PulseEngineBackend;

class PULSE_ENGINE_DLL_API FileManager
{
private:
    FileManager() = default;
    ~FileManager() = default;

public:
    /**
     * Returns the collection type based on the file extension.
     */
    static std::string GetCollectionByExtension(const std::string& fileName);
    /**
    * Loads engine configuration from disk into a JSON object.
    */
    static nlohmann::json_abi_v3_12_0::json OpenEngineConfigFile(PulseEngineBackend* engine);
    /**
    * Saves engine configuration from a JSON object to disk.
    */
    static void SaveEngineConfigFile(PulseEngineBackend* engine, const nlohmann::json_abi_v3_12_0::json& engineConfig);

};


#endif