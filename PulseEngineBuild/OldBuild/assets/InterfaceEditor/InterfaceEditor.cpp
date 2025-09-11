
#include "InterfaceEditor.h"
#include "PulseEngine/core/SceneLoader/SceneLoader.h"
#include "PulseEngineEditor/InterfaceEditor/TopBar.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/CustomScripts/ScriptsLoader.h"
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"

InterfaceEditor::InterfaceEditor(PulseEngineBackend* engine)
{    
    topbar = new TopBar();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; 

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark(); 

    ImGui_ImplGlfw_InitForOpenGL(engine->GetWindowContext()->GetGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
    
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.ScrollbarRounding = 6.0f;
    style.ChildRounding = 6.0f;
    
    style.FrameBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;
    style.TabBorderSize = 1.0f;
    
    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.FramePadding = ImVec2(8.0f, 6.0f);
    style.ItemSpacing = ImVec2(10.0f, 8.0f);
    style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
    
    // Couleurs
    style.Colors[ImGuiCol_Text]                   = ImVec4(0.96f, 0.97f, 1.00f, 1.00f);  // Texte plus lumineux
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.64f, 1.00f);  // Texte désactivé plus doux
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.08f, 0.09f, 0.11f, 1.00f);  // Fond très sombre
    style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.12f, 0.13f, 0.15f, 1.00f);  // Fond des enfants un peu plus clair
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.06f, 0.06f, 0.07f, 0.94f);  // Fond popup un peu plus sombre
    style.Colors[ImGuiCol_Border]                = ImVec4(0.25f, 0.25f, 0.27f, 0.60f);  // Bordures plus discrètes
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);  // Pas d'ombre de bordure
    
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.16f, 0.17f, 0.19f, 1.00f);  // Fond des cadres
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.20f, 0.21f, 0.23f, 1.00f);  // Survol des cadres
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.24f, 0.25f, 0.27f, 1.00f);  // Cadres actifs
    
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);  // Fond du titre
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.14f, 0.15f, 0.16f, 1.00f);  // Titre actif
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);  // Titre réduit
    
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.13f, 0.13f, 0.14f, 1.00f);  // Fond barre de menu
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.08f, 0.08f, 0.08f, 0.53f);  // Fond des barres de défilement
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);  // Barre de défilement
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);  // Survol barre de défilement
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);  // Actif barre de défilement
    
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.30f, 0.60f, 1.00f, 1.00f);  // Coche bleue plus lumineuse
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.30f, 0.60f, 1.00f, 1.00f);  // Curseur du slider
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.38f, 0.65f, 1.00f, 1.00f);  // Curseur actif du slider
    
    style.Colors[ImGuiCol_Button]                = ImVec4(0.22f, 0.23f, 0.27f, 1.00f);  // Fond des boutons
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.28f, 0.30f, 0.33f, 1.00f);  // Survol des boutons
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.33f, 0.35f, 0.38f, 1.00f);  // Bouton actif
    
    style.Colors[ImGuiCol_Header]                = ImVec4(0.24f, 0.25f, 0.27f, 1.00f);  // Fond des en-têtes
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.30f, 0.32f, 0.35f, 1.00f);  // Survol des en-têtes
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.36f, 0.38f, 0.41f, 1.00f);  // En-têtes actifs
    
    style.Colors[ImGuiCol_Separator]             = ImVec4(0.40f, 0.40f, 0.43f, 0.60f);  // Séparateur discret
    style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.45f, 0.45f, 0.48f, 1.00f);  // Survol séparateur
    style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.50f, 0.50f, 0.54f, 1.00f);  // Séparateur actif
    
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.60f, 0.98f, 0.20f);  // Grip de redimensionnement
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.60f, 0.98f, 0.50f);  // Survol grip
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.60f, 0.98f, 0.80f);  // Actif grip
    
    style.Colors[ImGuiCol_Tab]                   = ImVec4(0.18f, 0.19f, 0.20f, 1.00f);  // Fond des onglets
    style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.28f, 0.30f, 0.33f, 1.00f);  // Survol des onglets
    style.Colors[ImGuiCol_TabActive]             = ImVec4(0.24f, 0.26f, 0.29f, 1.00f);  // Onglet actif
    style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.18f, 0.18f, 0.18f, 1.00f); // Onglet non sélectionné
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f); // Onglet non actif
    
}

void InterfaceEditor::Render(PulseEngineBackend *engine)
{        
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    GenerateSceneDataWindow(engine);

    EntityAnalyzerWindow();


    Viewport(engine);

    topbar->UpdateBar(engine);
    // Rendu de la frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void InterfaceEditor::Viewport(PulseEngineBackend *engine)
{
    ImGui::Begin("Viewport");

    ImGui::Image((ImTextureID)(intptr_t)engine->fboTexture, ImVec2(engine->fboWidth, engine->fboHeight), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

void InterfaceEditor::EntityAnalyzerWindow()
{
    ImGui::Begin("Entity analyzer");

    if (selectedEntity)
    {
        ImGui::Text("Selected Entity: %s", selectedEntity->GetName().c_str());
        ImGui::Separator();

        PulseEngine::Vector3 position = selectedEntity->GetPosition();
        PulseEngine::Vector3 rotation = selectedEntity->GetRotation();
        PulseEngine::Vector3 scale = selectedEntity->GetScale();
        if (ImGui::TreeNode("Transform"))
        {
            // Position
            ImGui::Text("Position:");
            if (ImGui::DragFloat3("##Position", &(position.x), 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                selectedEntity->SetPosition(position);
            }

            // Rotation
            ImGui::Text("Rotation:");
            if (ImGui::DragFloat3("##Rotation", &(rotation.x), 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                selectedEntity->SetRotation(rotation);
            }

            // Scale
            ImGui::Text("Scale:");
            if (ImGui::DragFloat3("##Scale", &(scale.x), 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                selectedEntity->SetScale(scale);
            }

            ImGui::TreePop();
        }
        for (auto& s : selectedEntity->GetScripts())
        {
            std::cout << "Script name: " << s->GetName() << std::endl;
            if(!s->isEntityLinked) ImGui::Text("Only on this entity !");
                if (ImGui::TreeNode(s->GetName()))
                {
                    for (auto& var : s->GetExposedVariables())
                    {
                        std::cout << "Variable name: " << var.name << std::endl;
                        std::string label = var.name + "##" + s->GetName(); 
                    
                        switch (var.type)
                        {
                            case ExposedVariable::Type::INT:
                                ImGui::InputInt(label.c_str(), reinterpret_cast<int*>(var.ptr));
                                break;
                            case ExposedVariable::Type::FLOAT:
                                ImGui::InputFloat(label.c_str(), reinterpret_cast<float*>(var.ptr));
                                break;
                            case ExposedVariable::Type::BOOL:
                                ImGui::Checkbox(label.c_str(), reinterpret_cast<bool*>(var.ptr));
                                break;
                            case ExposedVariable::Type::STRING:
                                break;
                        }
                    }
                    ImGui::TreePop();
                }

        }

        static bool isSelected = false;
        if (ImGui::Button("Add script"))
        {
            isSelected = true;
        }
        if(isSelected)
        {
            if (ImGui::BeginCombo("Choose a script", "Select a script"))
            {
                for (const auto& [name, createFunc] : ScriptsLoader::scriptMap)
                {
                    if (ImGui::Selectable(name.c_str(), isSelected))
                    {
                        IScript* newScript = ScriptsLoader::GetScriptFromCallName(name);
                        if (newScript)
                        {
                            selectedEntity->AddScript(newScript); 
                        }
                        isSelected = false;
                    }
                }
                ImGui::EndCombo();
            }
        }
        

    }
    else
    {
        ImGui::Text("No entity selected.");
    }

    ImGui::End();
}

void InterfaceEditor::GenerateSceneDataWindow(PulseEngineBackend *engine)
{
    ImGui::Begin("Scene Manager");

    ImGui::Text("Scene Management");
    ImGui::Separator();

    if (ImGui::Button("Clear Scene", ImVec2(-1, 0))) // Full-width button
    {
        selectedEntity = nullptr;
        engine->ClearScene();
    }

    ImGui::Spacing();

    // Get all files from the Scenes directory
    std::vector<std::string> sceneFiles = SceneLoader::GetSceneFiles("PulseEngineEditor/Scenes");

    for (const auto &fileName : sceneFiles)
    {
        if (ImGui::Button(fileName.c_str(), ImVec2(-1, 0))) // Full-width button
        {
            SceneLoader::LoadScene(fileName, engine);
        }
        ImGui::Spacing();
    }

    ImGui::Spacing();

    static bool openPopup = false;
    static char sceneName[128] = ""; // Buffer for scene name input

    // Save Scene button
    if (ImGui::Button("Save Scene", ImVec2(-1, 0))) // Full-width button
    {
        openPopup = true;
        ImGui::OpenPopup("Save Scene");
    }

    // Popup
    if (ImGui::BeginPopupModal("Save Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Enter scene name:");
        ImGui::InputText("##SceneName", sceneName, IM_ARRAYSIZE(sceneName));

        ImGui::Spacing();
        if (ImGui::Button("Save", ImVec2(120, 0)))
        {
            // SceneLoader::SaveSceneToFile(sceneName, engine);
            ImGui::CloseCurrentPopup();
            openPopup = false;
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            openPopup = false;
        }

        ImGui::EndPopup();
    }

    ImGui::Spacing();

    ImGui::Text("Scene Entities:");
    // Create a scrollable zone for displaying entities
    ImGui::BeginChild("Scene Entities", ImVec2(0, -1), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (size_t i = 0; i < engine->entities.size(); i++)
    {
        if(!engine->entities[i]) continue;
        std::string entityLabel = "Entity: " + engine->entities[i]->GetName() + "_"  + std::to_string(i) +  "##" + std::to_string(i);
        std::string deleteButtonLabel = "X##" + std::to_string(i);

        if (ImGui::Button(entityLabel.c_str()))
        {
            selectedEntity = engine->entities[i];
        }

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::GetStyle().FramePadding.x * 3 - 5); // Move cursor to the right
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f)); // Red background
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f)); // Brighter red on hover
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f)); // Darker red when active
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 6.0f)); // Smaller padding for square button

        if (ImGui::Button(deleteButtonLabel.c_str()))
        {
            if (selectedEntity == engine->entities[i])
            {
                selectedEntity = nullptr;
            }
            engine->DeleteEntity(engine->entities[i]);
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        ImGui::Separator();
    }

    ImGui::EndChild();

    ImGui::End();
}
