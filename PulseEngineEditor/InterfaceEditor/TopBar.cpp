/**
 * @file TopBar.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "TopBar.h"

#include <cstdlib>

// #include "json.hpp"

#include "imgui/imgui.h"
#include "PulseEngine/core/PulseEngineBackend.h"
#include "PulseEngine/core/Meshes/primitive/Primitive.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/core/Material/Material.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "PulseEngine/core/GUID/GuidReader.h"
#include "PulseEngine/core/SceneLoader/SceneLoader.h"
#include "PulseEngineEditor/InterfaceEditor/InterfaceEditor.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "PulseEngine/core/Lights/PointLight/PointLight.h"
#include "PulseEngine/core/Lights/DirectionalLight/DirectionalLight.h"
#include "PulseEngine/core/FileManager/FileManager.h"

/**
 * @brief Update the top bar of the editor interface. It's actually the render of the bar in ImGui
 * 
 * @param[in] engine pointer to the actual backend, needed to perform modification on entities, lighting, etcetc
 * @param[in] editor the interface, because the topbar isn't part of the interface module, but need to rely on it sometimes.
 */
void TopBar::UpdateBar(PulseEngineBackend* engine, InterfaceEditor* editor)
{
    static bool requestOpenNewMapPopup = false;
    static char newMapName[128] = "";

    // === MENU BAR ===
    if (ImGui::BeginMainMenuBar())
    {
        // === FILE MENU ===
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New map"))
            {
                requestOpenNewMapPopup = true;
                strcpy(newMapName, "");
            }
            if (ImGui::MenuItem("Build game"))
            {     
                ///
                system("echo === Generating folders for Window ===");           
                system("if not exist Build mkdir Build");
                system("if not exist \"Build/assets\" mkdir \"Build/assets\"");
                system("if not exist \"Build/Logs\" mkdir \"Build/Logs\"");
                std::cout << "=== Copying assets For window ===" << std::endl;
                system("xcopy PulseEngineEditor \"Build/assets\" /E /I /Y");    
                std::cout << "=== Copying PulseEngine.dll For window ===" << std::endl;
                system("xcopy dist\\PulseEngine.dll \"Build\" /Y");
                std::cout << "=== Copying executable for window ===" << std::endl;

                system("xcopy dist\\game.exe \"Build\" /Y");

                
                nlohmann::json_abi_v3_12_0::json engineConfig = FileManager::OpenEngineConfigFile(engine);
                std::string gameName = engineConfig["GameData"]["Name"].get<std::string>();
                std::string renameCmd = "rename \"Build\\game.exe\" \"" + gameName + ".exe\"";
                system(renameCmd.c_str());



            }

            if (ImGui::MenuItem("Open"))
            {
                // TODO: Add open map logic
            }

            if (ImGui::MenuItem("Save"))
            {
                SceneLoader::SaveSceneToFile(engine->actualMapName, engine);
            }

            if (ImGui::MenuItem("Exit"))
            {
                // TODO: Add exit logic
            }

            ImGui::EndMenu();
        }

        // === EDIT MENU ===
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo")) { /* TODO: Undo logic */ }
            if (ImGui::MenuItem("Redo")) { /* TODO: Redo logic */ }

            if (ImGui::BeginMenu("Add"))
            {
                if (ImGui::MenuItem("Cube"))
                {
                    int counter = 0;
                    std::string baseName = "Cube";
                    std::string finalName = baseName;
                
                    // Find a unique name
                    bool nameExists = true;
                    while (nameExists)
                    {
                        nameExists = false;
                        for (const auto& ent : engine->entities)
                        {
                            if (ent->GetName() == finalName)
                            {
                                nameExists = true;
                                counter++;
                                finalName = baseName + " (" + std::to_string(counter) + ")";
                                break;
                            }
                        }
                    }

                    GuidReader::InsertIntoCollection("Entities/primitiveCube.pEntity");
                
                    // Create entity with unique name
                    Entity* cube = new Entity(
                        finalName,
                        PulseEngine::Vector3(0.0f),
                        Primitive::Cube(),
                        MaterialManager::loadMaterial(std::string(ASSET_PATH) + "Materials/cube.mat")
                    );
                    cube->SetGuid(GenerateGUIDFromPath("Entities/primitiveCube.pEntity"));
                    cube->SetMuid(GenerateGUIDFromPathAndMap(finalName, engine->actualMapPath));
                    engine->entities.push_back(cube);
                }
                if (ImGui::MenuItem("Point light"))
                {
                    engine->lights.push_back(new PointLight(
                        PulseEngine::Vector3(0.0f, 5.0f, 0.0f),
                        PulseEngine::Color(1.0f, 1.0f, 1.0f),
                        5.0f,
                        5.0f,
                        50.0f
                    ));

                }
                if (ImGui::MenuItem("Directional light"))
                {
                    engine->lights.push_back(new DirectionalLight(
                        1.0f,
                        50.0f,
                        glm::vec3(0.0f,0.0f,0.0f),
                        PulseEngine::Vector3(0.0f, 0.0f, 0.0f),
                        PulseEngine::Color(1.0f, 1.0f, 1.0f),
                        1.0f,
                        10.0f
                    ));

                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        // === VIEW MENU ===
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Scene")) { editor->windowStates["viewport"] = !editor->windowStates["viewport"]; }
            if (ImGui::MenuItem("Entity Analyzer")) { editor->windowStates["EntityAnalyzer"] = !editor->windowStates["EntityAnalyzer"]; }
            if (ImGui::MenuItem("Engine Config")) { editor->windowStates["EngineConfig"] = !editor->windowStates["EngineConfig"]; }
            if (ImGui::MenuItem("Scene Data")) { editor->windowStates["SceneData"] = !editor->windowStates["SceneData"]; }
            if (ImGui::MenuItem("Asset Manager")) { editor->windowStates["assetManager"] = !editor->windowStates["assetManager"]; }

            ImGui::EndMenu();
        }
        // === HELP MENU ===
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About")) { /* TODO: About popup */ }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // === NEW MAP POPUP ===
    if (requestOpenNewMapPopup)
    {
        ImGui::OpenPopup("NewMapPopup");
        requestOpenNewMapPopup = false;
    }

    if (ImGui::BeginPopupModal("NewMapPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Enter new map name:");
        ImGui::InputText("##mapname", newMapName, IM_ARRAYSIZE(newMapName));

        if (ImGui::Button("Create", ImVec2(120, 0)))
        {
            std::string mapPath = "Scenes/";
            mapPath += newMapName;

            engine->actualMapPath = mapPath;
            engine->actualMapName = newMapName;

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
