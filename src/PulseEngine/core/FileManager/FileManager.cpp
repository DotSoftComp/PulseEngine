#include "FileManager.h"
#include "PulseEngine/core/PulseEngineBackend.h"

#include <fstream>
#include <algorithm>

using json = nlohmann::json;

static bool EndsWith(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string FileManager::GetCollectionByExtension(const std::string& fileName)
{
    std::string collectionType = "guidCollection";

    if (EndsWith(fileName, ".mat"))
    {
        collectionType += "Materials";
    }
    else if (EndsWith(fileName, ".pmesh"))
    {
        collectionType += "Meshes";
    }
    else if (EndsWith(fileName, ".pEntity"))
    {
        collectionType += "Entities";
    }

    collectionType += ".puid";
    return collectionType;
}

json FileManager::OpenEngineConfigFile(PulseEngineBackend* engine)
{
    const std::string configPath = std::string(ASSET_PATH) + "Engine-config.ini";
    json engineConfig;
    std::ifstream configFile(configPath);

    if (!configFile.is_open())
    {
        // Default engine config
        engineConfig["GameData"]["Name"]             = "Pulse game name";
        engineConfig["GameData"]["FirstScene"]        = "Scenes/CZEFGZEOUFHDZOEIDYO.pmap";
        engineConfig["GameData"]["DefaultResolution"] = "1920x1080";
        engineConfig["GameData"]["Fullscreen"]        = false;
        engineConfig["GameData"]["VSync"]             = true;

        engineConfig["Engine"]["Name"]     = engine->GetEngineName();
        engineConfig["Engine"]["Version"]  = engine->GetEngineVersion();
        engineConfig["Engine"]["DevMonth"] = engine->GetDevMonth();
        engineConfig["Engine"]["Author"]   = engine->GetCompanyName();
        engineConfig["Engine"]["License"]  = "None";
        

        std::ofstream outFile(configPath);
        if (outFile.is_open())
        {
            outFile << engineConfig.dump(4);
            outFile.close();
        }
        return engineConfig;
    }

    configFile >> engineConfig;
    configFile.close();
    return engineConfig;
}

void FileManager::SaveEngineConfigFile(PulseEngineBackend* engine, const json& engineConfig)
{
    const std::string configPath = std::string(ASSET_PATH) + "Engine-config.ini";
    std::ofstream outFile(configPath);
    if (outFile.is_open())
    {
        outFile << engineConfig.dump(4);
        outFile.close();
    }
}
