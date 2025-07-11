#ifndef SCENELOADER_H
#define SCENELOADER_H

#include "Common/common.h"
#include "Common/dllExport.h"
#include "json.hpp"

class PulseEngineBackend;

struct ExposedVariable;
struct aiScene;

class PULSE_ENGINE_DLL_API SceneLoader
{
    public:
        ///====== SceneLoader ======
        /// This function is responsible of the loading of the scene, for the editor, and in game.
        /// The scene is loaded from a json file, with entities, and scripts attach to them.
        /// The scripts saved will have all the exposed variables saved, from the value set in the editor.
        static void LoadScene(const std::string &mapName, PulseEngineBackend *backend);
        static std::vector<std::string> GetSceneFiles(const std::string& directory);

        static const aiScene* LoadSceneFromAssimp(std::string path);

        ///====== SaveScene ======
        /// This function is responsible of the saving of the scene, for the editor.
        /// The scene is saved to a json file, with entities, and scripts attach to them.
        /// The scripts saved will have all the exposed variables saved, from the value set in the editor.
        static void SaveSceneToFile(const std::string &mapName, PulseEngineBackend *backend);

        static void SaveEntities(Entity *const &entity, nlohmann::json_abi_v3_12_0::json &sceneData);
        static void SaveBaseDataEntity(Entity *const &entity, nlohmann::json_abi_v3_12_0::json &entityData);
        static void LoadEntityScript(const nlohmann::json_abi_v3_12_0::json &script, Entity *entity);
        static void ExtractExposedVariable(const nlohmann::json_abi_v3_12_0::json &var, std::vector<ExposedVariable *> &exposedVariables);
        static Entity* LoadEntityBaseData(const nlohmann::json_abi_v3_12_0::json &entityData);
        
};

#endif