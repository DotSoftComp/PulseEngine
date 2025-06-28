#include "Entity.h"
#include "PulseEngine/core/Meshes/Mesh.h"
#include "shader.h"
#include "PulseEngine/core/Material/Material.h"
#include "Common/common.h"
#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/core/Physics/Collider/BoxCollider.h"
#include "PulseEngine/core/Physics/Collider/Collider.h"
#include "PulseEngine/core/Material/Material.h"

#include <algorithm>

Entity::Entity(const std::string &name, const PulseEngine::Vector3 &position, Mesh* mesh, Material* material) :name(name), position(position), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), material(material)
{
    //actually is it possible to be nullptr, because of loadScene, we need to load an entity with a material but no mesh before reading it.
    //so mesh can nullptr, and if it is pushed back into the meshes vector, it will cause a crash.
    if(mesh) meshes.push_back(mesh);

    BaseConstructor();
}

void Entity::BaseConstructor()
{
    UpdateModelMatrix();
    collider = new BoxCollider(&this->position, &this->rotation, PulseEngine::Vector3(1.0f, 1.0f, 1.0f));
}

Entity::Entity(const std::string &name, const PulseEngine::Vector3 &position) : name(name), position(position)
{
    BaseConstructor();
}

void Entity::UpdateModelMatrix()
{
    glm::mat4 localMat = glm::mat4(1.0f); // Initialize to identity matrix
    localMat = glm::translate(localMat, glm::vec3(position.x, position.y, position.z));
    localMat = glm::rotate(localMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    localMat = glm::rotate(localMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    localMat = glm::rotate(localMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    localMat = glm::scale(localMat, glm::vec3(scale.x, scale.y, scale.z));
    
    PulseEngine::Mat4 engineMat;
    memcpy(engineMat.Ptr(), glm::value_ptr(localMat), sizeof(float) * 16);

    this->entityMatrix = engineMat;

}

void Entity::SetMaterial(Material * material) { this->material = material; }

void Entity::UpdateEntity(float deltaTime) const
{
    for(auto s : scripts)
    {
        s->OnUpdate();
    }
    for (const auto &mesh : meshes)
    {
        mesh->UpdateMesh(deltaTime);
    }

}

void Entity::DrawEntity() const
{
    glm::mat4 glmMat = glm::make_mat4(GetMatrix().Ptr());
    material->GetShader()->SetMat4("model", glmMat);
    material->GetShader()->SetFloat("specularStrength", material ? material->specular : 1.0f);
    material->GetShader()->SetVec3("objectColor", material ? glm::vec3(material->color.x, material->color.y, material->color.z) : glm::vec3(0.5f));
    std::cout << "bind of model matrix done successfully" << std::endl;
    SimplyDrawMesh();
}

void Entity::UseShader() const
{
    material->GetShader()->Use();
}

void Entity::SimplyDrawMesh() const
{
    std::cout << "number of mesh inside of entity : " << meshes.size() << std::endl;
    for (const auto &mesh : meshes)
    {
        std::cout << "rendering mesh" << std::endl;
        if (auto albedoTex = material->GetTexture("albedo"))
        {
            albedoTex->Bind(6); // Slot 6
             material->GetShader()->SetInt("albedoMap", 6); // "u_AlbedoMap" => slot 6
        }

        if (auto normalTex = material->GetTexture("normal"))
        {
            normalTex->Bind(1); // Slot 1
             material->GetShader()->SetInt("normalMap", 1); // "u_NormalMap" => slot 1
        }
        mesh->Draw(material->GetShader()->getProgramID());
    }
}

void Entity::DrawMeshWithShader(GLuint shaderProgram) const
{
    for (const auto &mesh : meshes)
    {
        mesh->Draw(shaderProgram);
    }
}

void Entity::AddScript(IScript *script)
{
    if(!script) return;
    scripts.push_back(script);
    script->OnStart();
}

void Entity::RemoveScript(IScript* script)
{
    auto it = std::find(scripts.begin(), scripts.end(), script);
    
    if (it != scripts.end()) 
    {
        scripts.erase(it);
    }
}

