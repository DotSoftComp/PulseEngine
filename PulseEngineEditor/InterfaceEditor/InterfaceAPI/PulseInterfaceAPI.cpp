#include "PulseInterfaceAPI.h"
#include "imgui/imgui.h"
#include "PulseEngine/core/PulseEngineBackend.h"
#include "PulseEngineEditor/InterfaceEditor/InterfaceEditor.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/Meshes/Mesh.h"
#include "PulseEngine/core/Material/Material.h"
#include "PulseEngine/core/Material/Texture.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/core/GUID/GuidReader.h"
#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/ModuleLoader/IModuleInterface/IModuleInterface.h"
#include "shader.h"
#include "json.hpp"

#include <memory>
#include <unordered_map>
#include <fstream>

void PulseInterfaceAPI::OpenWindow(const std::string &name)
{
    ImGui::Begin(name.c_str());
}

void PulseInterfaceAPI::CloseWindow()
{
    ImGui::End();
}

void PulseInterfaceAPI::WriteText(const std::string &text)
{
    ImGui::Text(text.c_str());    
}

void PulseInterfaceAPI::Image(int user_texture_id, const PulseEngine::Vector2& image_size, const PulseEngine::Vector2& uv0, const PulseEngine::Vector2& uv1)
{
    ImGui::Image((ImTextureID)(intptr_t)user_texture_id, ImVec2(image_size.x, image_size.y), ImVec2(uv0.x, uv0.y), ImVec2(uv1.x, uv1.y));
}

void PulseInterfaceAPI::AddSpace(int amount)
{
    for (unsigned int i = 0; i < amount; i++)
    {
        ImGui::Spacing();
    }
    
}

bool PulseInterfaceAPI::Button(const std::string &name, const PulseEngine::Vector2 &size)
{
    return ImGui::Button(name.c_str(), ImVec2(size.x, size.y));
}

void PulseInterfaceAPI::Separator()
{
    ImGui::Separator();
}

void PulseInterfaceAPI::SameLine()
{
    ImGui::SameLine();
}

void PulseInterfaceAPI::PushStyleColor(PulseEngineStyle syle, const PulseEngine::Vector3 &color, const float &alpha)
{
    ImGui::PushStyleColor((ImGuiCol)((int)syle), ImVec4(color.x, color.y, color.z, alpha));
}

void PulseInterfaceAPI::PopStyleColor(int amount)
{
    ImGui::PopStyleColor(amount);
}

void PulseInterfaceAPI::RenderCameraToInterface(PulseEngine::Vector2* previewData, Camera* camera, const std::string& windowName, const PulseEngine::Vector2& imageSize, std::vector<Entity*> entitiesToRender, Shader* shader)
{
    // Create or bind a framebuffer/render target
    unsigned int previewFBO = previewData->x;
    unsigned int previewTexture = previewData->y;
    static int previewWidth = imageSize.x, previewHeight = imageSize.y;

    if (previewFBO == 0 || previewWidth != imageSize.x || previewHeight != imageSize.y)
    {
        previewWidth = imageSize.x;
        previewHeight = imageSize.y;
    
        if (previewFBO == 0)
        {
            glGenFramebuffers(1, &previewFBO);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, previewFBO);
    
        if (previewTexture == 0)
        {
            glGenTextures(1, &previewTexture);
        }
        glBindTexture(GL_TEXTURE_2D, previewTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, previewWidth, previewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, previewTexture, 0);
    
        static unsigned int rbo = 0;
        if (rbo == 0)
            glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, previewWidth, previewHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Preview framebuffer not complete!" << std::endl;
    
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    glBindFramebuffer(GL_FRAMEBUFFER, previewFBO);
    glViewport(0, 0, previewWidth, previewHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    PulseEngineInstance->SpecificRender(camera, previewFBO, entitiesToRender, imageSize, shader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    previewData->x = previewFBO;
    previewData->y = previewTexture;

    PulseInterfaceAPI::Image(previewTexture, imageSize, PulseEngine::Vector2(0, 1), PulseEngine::Vector2(1, 0));
}

Entity* PulseInterfaceAPI::GetSelectedEntity()
{
    return PulseEngineInstance->editor->GetSelectedEntity();
}

bool PulseInterfaceAPI::IsCurrentWindowFocused()
{
    // Returns true if the current ImGui window is focused
    return ImGui::IsWindowFocused();
}

PulseEngine::Vector2 PulseInterfaceAPI::MouseIn()
{
    ImVec2 mousePos = ImGui::GetIO().MousePos;
    return PulseEngine::Vector2(mousePos.x, mousePos.y);
}

PulseEngine::Vector2 PulseInterfaceAPI::MouseDelta()
{
    ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;
    return PulseEngine::Vector2(mouseDelta.x, mouseDelta.y);
}

void PulseInterfaceAPI::AddFunctionToFileClickedCallbacks(std::function<void(const ClickedFileData &)> callback)
    {
        PulseEngineInstance->editor->fileClickedCallbacks.push_back(callback);
    }

void PulseInterfaceAPI::BeginChild(const std::string & name, const PulseEngine::Vector2 & size, bool border)
{
    ImGui::BeginChild(name.c_str(), ImVec2(size.x, size.y), border);
}

void PulseInterfaceAPI::EndChild()
{
    ImGui::EndChild();
}

bool PulseInterfaceAPI::BeginCombo(const std::string & label, const std::string & previewValue)
{
    return ImGui::BeginCombo(label.c_str(), previewValue.c_str());
}

void PulseInterfaceAPI::EndCombo()
{
    ImGui::EndCombo();
}

bool PulseInterfaceAPI::Selectable(const std::string & label, bool selected, const PulseEngine::Vector2 & size)
{
    return ImGui::Selectable(label.c_str(), selected, ImGuiSelectableFlags_None, ImVec2(size.x, size.y));
}

void PulseInterfaceAPI::AddTransformModifier(Entity *entity, const std::string &modifierName)
{
    
    PulseEngine::Vector3 position = entity->GetPosition();
    PulseEngine::Vector3 rotation = entity->GetRotation();
    PulseEngine::Vector3 scale = entity->GetScale();
    if (ImGui::TreeNode("Transform"))
        {
            // Position
            ImGui::Text("Position:");
            if (ImGui::DragFloat3("##Position", &(position.x), 0.01f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                entity->SetPosition(position);
            }

            // Rotation
            ImGui::Text("Rotation:");
            if (ImGui::DragFloat3("##Rotation", &(rotation.x), 0.01f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                entity->SetRotation(rotation);
            }

            // Scale
            ImGui::Text("Scale:");
            if (ImGui::DragFloat3("##Scale", &(scale.x), 0.01f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
                entity->SetScale(scale);
            }

            ImGui::TreePop();
        }
}

void PulseInterfaceAPI::AddTransformModifierForMesh(Mesh *mesh, const std::string &modifierName)
{
        
    if (ImGui::TreeNode("Transform"))
        {
            // Position
            ImGui::Text("Position:");
            if (ImGui::DragFloat3("##Position", &(mesh->position.x), 0.01f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
            }

            // Rotation
            ImGui::Text("Rotation:");
            if (ImGui::DragFloat3("##Rotation", &(mesh->rotation.x), 0.01f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
            }

            // Scale
            ImGui::Text("Scale:");
            if (ImGui::DragFloat3("##Scale", &(mesh->scale.x), 0.01f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
            {
            }

            ImGui::TreePop();
        }
}

bool PulseInterfaceAPI::StartTreeNode(const std::string &name, bool *open)
{
    bool isOpen = false;
    if (open)
    {
        if (*open)
        {
            isOpen = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen, "%s", name.c_str());
        }
        else
        {
            isOpen = ImGui::TreeNode(name.c_str());
        }
    }
    else
    {
        isOpen = ImGui::TreeNode(name.c_str());
    }

    if (open)
    {
        *open = isOpen;
    }

    return isOpen;
}


void PulseInterfaceAPI::EndTreeNode()
{
    ImGui::TreePop();
}

void PulseInterfaceAPI::AddMaterialPreview(Material*& material, const PulseEngine::Vector2 &imageSize, const std::string &name)
{
    std::string childName = "MaterialPreviewChild###" + name;
    ImGui::BeginChild(childName.c_str(), ImVec2(0, 0), true);
    if (material == nullptr )
    {
        ImGui::Text("No material to preview");
        MaterialPicker(material);
        return;
    }

    ImGui::PushID(name.c_str());

    ImGui::BeginGroup(); // Groupe principal : box globale

    // Trouver la première texture valide (pour l'afficher à gauche)
    const auto& textures = material->GetAllTextures();
    std::shared_ptr<Texture> firstTexture = nullptr;
    for (const auto& [type, texture] : textures)
    {
        if (texture)
        {
            firstTexture = texture;
            
            break;
        }
    }

    ImGui::BeginGroup(); // Groupe à gauche pour image
    if (firstTexture)
    {
        PulseInterfaceAPI::Image(firstTexture->id, imageSize, PulseEngine::Vector2(0, 1), PulseEngine::Vector2(1, 0));
    }
    else
    {
        ImGui::Text("No texture preview");
    }
    ImGui::EndGroup();

    ImGui::SameLine(); // Passer à droite, à côté de l'image

    ImGui::BeginGroup(); // Groupe à droite pour infos texte
    // Nom du material
    ImGui::Text("%s", material->GetName().c_str());

    MaterialPicker(material);

    ImGui::EndGroup();

    ImGui::EndGroup(); // Fin du groupe principal

    // Tree node pour afficher toutes les textures en détail
    if (ImGui::TreeNode("Textures"))
    {
        for (const auto& [type, texture] : textures)
        {
            if (texture)
            {
                ImGui::Text("%s:", type.c_str());
                PulseInterfaceAPI::Image(texture->id, imageSize, PulseEngine::Vector2(0, 1), PulseEngine::Vector2(1, 0));
            }
            else
            {
                ImGui::Text("%s: No texture assigned", type.c_str());
            }
        }
        ImGui::TreePop();
    }

    ImGui::PopID();
    ImGui::EndChild();
}

void PulseInterfaceAPI::MaterialPicker(Material *&material)
{
    if (ImGui::BeginCombo("##MaterialSelector", material ? material->GetName().c_str() : "Select Material"))
    {
        for (const auto pr : GuidReader::GetAllAvailableFiles("guidCollectionMaterials.puid"))
        {
            if (PulseInterfaceAPI::Selectable(pr.second, false))
            {
                material = MaterialManager::loadMaterial(pr.second);
                if(material) material->guid = pr.first;
            }
        }
        ImGui::EndCombo();
    }
}

PulseEngine::Vector2 PulseInterfaceAPI::GetActualWindowSize()
{
    ImVec2 size = ImGui::GetWindowSize();
    return PulseEngine::Vector2(size.x, size.y);
}

bool PulseInterfaceAPI::DragFloat(const char *label, float *value, float speed, float minVal, float maxVal, const char *format)
{
    return ImGui::DragFloat(label, value, speed, minVal, maxVal, format);
}

bool PulseInterfaceAPI::DragFloat3(const char *label, float *values, float speed, float minVal, float maxVal, const char *format)
{
    return ImGui::DragFloat3(label, values, speed, minVal, maxVal, format);
}

void PulseInterfaceAPI::ChangeWindowState(IModuleInterface* script, bool state)
{
    PulseEngineInstance->editor->windowStates[script->GetName()] = state;
}

void PulseInterfaceAPI::ShowContextMenu(const char* popupId, const std::vector<ContextMenuItem>& items)
{
    if (ImGui::BeginPopup(popupId))
    {
        for (const auto& item : items)
        {
            if (ImGui::MenuItem(item.label.c_str()))
            {
                if (item.onClick) item.onClick();
            }
        }
        ImGui::EndPopup();
    }
}

void PulseInterfaceAPI::OpenContextMenu(const char *popupId)
{
    ImGui::OpenPopup(popupId);
}
