#include "TopBar.h"
#include "imgui/imgui.h"
#include "PulseEngine/core/PulseEngineBackend.h"
#include "PulseEngine/core/Meshes/primitive/Primitive.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/core/Material/Material.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "PulseEngine/core/GUID/GuidReader.h"
#include "PulseEngine/core/SceneLoader/SceneLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void TopBar::UpdateBar(PulseEngineBackend* engine)
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


                ImGui::EndMenu();
            }

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
