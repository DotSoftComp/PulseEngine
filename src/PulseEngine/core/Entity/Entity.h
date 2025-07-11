/**
 * @file Entity.cpp
 * @author Dorian LEXTERIAQUE (dlexteriaque@gmail.com)
 * @details 
 * It's the start of everything : everything is an Entity.
 * From light, to skeleton, to mesh, to camera, to player, to enemy, to NPC, to anything you want.
 * This is the base class for all entities in the Pulse Engine.
 * This permit an easy init, update, render, and modification by the engine on basic rules.
 * each object that could be rendered, updated, or modified by the engine should inherit from this class.
 * @todo We should remove all use of openGL directly here and use classes and struct from the game Engine instead.
 * @version 0.1
 * @date 2025-06-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef ENTITY_H
#define ENTITY_H

#include "Common/common.h"
#include "Common/dllExport.h"

#include <string>
#include <vector>
#include <iostream> // Temporary: consider wrapping with logging macros.

class Mesh;
class Shader;
class Material;
class IScript;
class Collider;
class BoxCollider;

/**
 * @class Entity
 * @brief Represents a scene object with transformation, rendering, and behavior.
 * 
 * Entities are the core unit of composition in the scene and may hold meshes,
 * materials, scripts, and colliders.
 */
class PULSE_ENGINE_DLL_API Entity
{
public:
    // ------------------------------------------------------------------------
    // Constructors & Destructor
    // ------------------------------------------------------------------------

    /**
     * @brief Constructs an entity with a mesh and material.
     * @param name The entity's name.
     * @param position The entity's initial world position.
     * @param mesh Pointer to the mesh to render.
     * @param material Pointer to the material for rendering.
     */
    Entity(const std::string &name, const PulseEngine::Vector3 &position, Mesh *mesh, Material *material);

    void BaseConstructor();

    /**
     * @brief Constructs an entity with only a position and name.
     */
    Entity(const std::string& name, const PulseEngine::Vector3& position);

    virtual ~Entity() { }

    // ------------------------------------------------------------------------
    // Transform Setters
    // ------------------------------------------------------------------------

    /// Sets the world position and updates the model matrix.
    void SetPosition(const PulseEngine::Vector3& position) { this->position = position; UpdateModelMatrix();}

    /// Sets the rotation and updates the model matrix.
    void SetRotation(const PulseEngine::Vector3& rotation) { this->rotation = rotation; UpdateModelMatrix();}

    /// Sets the scale and updates the model matrix.
    void SetScale(const PulseEngine::Vector3& scale) {this->scale = scale; UpdateModelMatrix();}

    void SetMaterial(Material* material);

    // ------------------------------------------------------------------------
    // Update & Rendering
    // ------------------------------------------------------------------------

    /**
     * @brief Updates the entity's behavior/scripts.
     * @param deltaTime Time elapsed since the last frame.
     */
    void UpdateEntity(float deltaTime) const;

    /**
     * @brief Draws the entity using its current mesh/material/shader.
     */
    void DrawEntity() const;

    /**
     * @brief Binds the shader used by the entity (if any).
     */
    void UseShader() const;

    /**
     * @brief Draws the mesh without applying material/shader logic.
     */
    void SimplyDrawMesh() const;

    /**
     * @brief Specifically draws the mesh using a provided shader program, usefull for shadow or others drawing style
     * @todo Consider removing GLuint and use a custom "int" for modularity between each graphics API.
     * @param shaderProgram 
     */
    void DrawMeshWithShader(GLuint shaderProgram) const;

    // ------------------------------------------------------------------------
    // Component Addition
    // ------------------------------------------------------------------------

    /// Adds a mesh to the entity.
    void AddMesh(Mesh* mesh) {meshes.push_back(mesh); }
        
    /// Attaches a behavior script to the entity.
    void AddScript(IScript* script);

    /// Removes a previously attached script.
    void RemoveScript(IScript* script);

    // ------------------------------------------------------------------------
    // Getters
    // ------------------------------------------------------------------------

    const PulseEngine::Vector3 &GetPosition() const { return position; }
    const PulseEngine::Vector3& GetRotation() const {return rotation;}
    const PulseEngine::Vector3& GetScale() const {return scale; }
    const PulseEngine::Mat4& GetMatrix() const { return entityMatrix; }
    const std::size_t& GetGuid() const {return guid;}
    /**
     * @brief The Muid is a unique identifier for the entity within the map.
     * 
     * @return const std::size_t& The Muid of the entity.
     */
    const std::size_t& GetMuid() const {return muid;}
    std::string GetName() const {return name;}
    Material* GetMaterial() {return material; }
    std::vector<IScript*> GetScripts() {return scripts; }

    // ------------------------------------------------------------------------
    // Setters
    // ------------------------------------------------------------------------

    void SetGuid(std::size_t g) {guid = g;}
    void SetMuid(std::size_t m) {muid = m;}
    void SetName(const std::string& name) {this->name = name;}

    // ------------------------------------------------------------------------
    // Collider (public for now, consider getter/setter)
    // ------------------------------------------------------------------------
    BoxCollider* collider = nullptr;

protected:
    PulseEngine::Vector3 position;
    PulseEngine::Vector3 rotation;
    PulseEngine::Vector3 scale;


private:
    // ------------------------------------------------------------------------
    // Internal Data
    // ------------------------------------------------------------------------
    std::string name;

    PulseEngine::Mat4 entityMatrix;

    Material* material = nullptr;
    std::size_t guid = 0;
    std::size_t muid = 0;

    std::vector<Mesh*> meshes;
    std::vector<IScript*> scripts;

    /// Updates the entity's world transformation matrix.
    void UpdateModelMatrix();
};

#endif // ENTITY_H
