#include "GuidReader.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "json.hpp"
#include "PulseEngine/core/Meshes/primitive/Primitive.h"
#include "PulseEngine/core/Meshes/Mesh.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/core/FileManager/FileManager.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "PulseEngine/CustomScripts/ScriptsLoader.h"
#include "PulseEngine/CustomScripts/IScripts.h"

#include <assimp/Importer.hpp>      // Assimp::Importer
#include <assimp/scene.h>           // aiScene
#include <assimp/postprocess.h>     // postprocessing flags

Entity *GuidReader::GetEntityFromGuid(std::size_t guid)
{
    static int count = 0;
    std::string name = "Entity_" + std::to_string(count++);
    Entity* entity = new Entity(name, PulseEngine::Vector3(0.0f), nullptr, MaterialManager::loadMaterial(std::string(ASSET_PATH) + "Materials/cube.mat"));
    
    nlohmann::json guidCollection;
    std::ifstream guidColFile(std::string(ASSET_PATH) +"Guid/guidCollectionEntities.puid");
    if(!guidColFile.is_open())
    {
        std::cout << "guid entity file not open" << std::endl;
        delete entity;
         return nullptr;
    }
    guidColFile >> guidCollection;

    if(guidCollection.contains(std::to_string(guid)))
    {
        nlohmann::json entityData;
        std::ifstream entityFile(std::string(ASSET_PATH) + std::string(guidCollection[std::to_string(guid)]));
        if(!entityFile.is_open())
        {
            std::cout << "entity data file not open" << std::endl;
            delete entity;
            return nullptr;
        }
        entityFile >> entityData;
        if(entityData.contains("Meshes"))
        {
             for (const auto& mesh : entityData["Meshes"])
             {
                 std::cout << "adding mesh" << std::endl;
                 std::size_t meshGuid = 0;
                 try
                 {
                     if (mesh.is_string())
                     {
                         meshGuid = std::stoull(mesh.get<std::string>());
                     }
                     else if (mesh.is_number_unsigned())
                     {
                         meshGuid = mesh.get<std::size_t>();
                     }
                     else
                     {
                         std::cerr << "Invalid mesh GUID format." << std::endl;
                         continue;
                     }
        
                     Mesh* msh = GetMeshFromGuid(meshGuid);
                     if (msh) entity->AddMesh(msh);
                 }
                 catch (const std::exception& e)
                 {
                     std::cerr << "Error parsing mesh guid: " << e.what() << std::endl;
                 }
             }
        }
        if(entityData.contains("Scripts"))
        {
            for (const auto& script : entityData["Scripts"])
            {
                IScript* scriptLoaded = ScriptsLoader::GetScriptFromCallName(script);
                if(!scriptLoaded) 
                {
                    std::cout << "Script " << script << " couldn't be loaded for " << guid << std::endl;
                    continue;
                }
                scriptLoaded->isEntityLinked = true;
                scriptLoaded->parent = entity;
                entity->AddScript(scriptLoaded);
                
            }
        }
        std::cout << "returning entity" << std::endl;
        return entity;
    }
    else
    {
        delete entity;
        return nullptr;
    }
}

Mesh* GuidReader::GetMeshFromGuid(std::size_t guid)
{
    return Primitive::Cube();
    std::string path = "";
    std::string meshPath = "";
    Assimp::Importer* importer = new Assimp::Importer();

    nlohmann::json guidCollection;
    std::ifstream guidColFile(std::string(ASSET_PATH) +"Guid/guidCollectionMeshes.puid");
    if(!guidColFile.is_open())
    {
        std::cout << "guid entity file not open" << std::endl;
         return nullptr;
    }
    guidColFile >> guidCollection;
    if(guidCollection.contains(std::to_string(guid)))
    {
        path = guidCollection[std::to_string(guid)].get<std::string>();
        if (path.empty())
        {
            std::cerr << "Path for GUID " << guid << " is empty." << std::endl;
            return nullptr;
        }
        path = std::string(ASSET_PATH) + path;
        std::ifstream file(path);
        if(file.is_open())
        {
            nlohmann::json fileData;
            file >> fileData;
            if(fileData.contains("MeshPath"))
            {
                meshPath = fileData["MeshPath"].get<std::string>();
                if(meshPath.empty())
                {
                    std::cerr << "Mesh path for GUID " << guid << " is empty." << std::endl;
                    return nullptr;
                }
                meshPath = std::string(ASSET_PATH) + meshPath;
            }
            else
            {
                std::cerr << "MeshPath not found in JSON for GUID " << guid << "." << std::endl;
                return nullptr;
            }
        }
    }
    else
    {
        std::cerr << "GUID " << guid << " not found in collection." << std::endl;
        return nullptr;
    }
    //onced shared system implemented, we will use importer->ReadFileFromMemory() instead
    // it will be needed to have a method in our shared system that sent back a vector of char from the .pak
    //the importer will read the object from the .pak like this :
    //std::vector<char> fileData = LoadFromPak("assets/model.fbx");
    //Assimp::Importer importer;
    //
    //const aiScene* scene = importer.ReadFileFromMemory(
    //    fileData.data(),                   // pointer to memory
    //    fileData.size(),                   // size
    //    aiProcess_Triangulate |
    //    aiProcess_FlipUVs |
    //    aiProcess_GenNormals |
    //    aiProcess_CalcTangentSpace,
    //    "fbx"                              // file extension/hint
    //);
    const aiScene* scene = importer->ReadFile(
        meshPath,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Erreur Assimp: " << importer->GetErrorString() << std::endl;
        return nullptr;
    }

    std::cout << "Modèle chargé avec succès : " << path << std::endl;
    Mesh* msh = Mesh::LoadFromAssimp(scene->mMeshes[0], scene); // OK: importer toujours vivant ici

    msh->importer = importer;
    return msh;
}

std::size_t GuidReader::InsertIntoCollection(const std::string &filePath)
{
    std::string collectionType = FileManager::GetCollectionByExtension(filePath);
    std::size_t guid = GenerateGUIDFromPath(filePath);

    std::string collectionPath = GUID_COLLECTION_PATH + collectionType;
    std::cout << collectionPath << std::endl;

    // Load existing collection
    nlohmann::json jsonData;

    std::ifstream inFile(collectionPath);
    if (inFile.is_open()) {
        // If file exists, read the existing JSON data
        inFile >> jsonData;
        inFile.close();
    } else {
        // If file doesn't exist, initialize an empty JSON object
        std::cerr << "File not found, creating new collection file." << std::endl;
        jsonData = nlohmann::json::object();  // Initialize an empty JSON object
    }

    // Insert or update entry
    jsonData[std::to_string(guid)] = filePath;

    // Rewrite the entire file (create or update the file)
    std::ofstream outFile(collectionPath);
    if (outFile.is_open()) {
        outFile << jsonData.dump(4); // 4 = pretty print with indentation
        outFile.close();
    } else {
        std::cerr << "Failed to open GUID collection file for writing." << std::endl;
    }

    return guid;
}
